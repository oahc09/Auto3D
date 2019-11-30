#include "Debug/Log.h"
#include "Debug/Profiler.h"
#include "IO/Stream.h"
#include "Core/ObjectResolver.h"
#include "Resource/JSONFile.h"
#include "Renderer/Renderer.h"
#include "RegisteredBox/RegisteredBox.h"
#include "Physics/PhysicsWorld.h"

#include "Scene.h"
#include "SpatialNode.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

Scene::Scene() :
    _nextNodeId(1),
	_physicsWorld(nullptr),
	_skybox(nullptr)
{
    // Register self to allow finding by ID
    AddNode(this);

    DefineLayer(LAYER_DEFAULT, "Default");
    DefineTag(TAG_NONE, "None");

	// Register scene to scene system use to render
	GModuleManager::Get().RegisteredBoxModule()->RegisterScene(this);
}

Scene::~Scene()
{
    // Node destructor will also remove children. But at that point the node<>_id maps have been destroyed 
    // so must tear down the scene tree already here
    RemoveAllChildren();
    RemoveNode(this);
    assert(_nodes.IsEmpty());
}

void Scene::RegisterObject()
{
    RegisterFactory<Scene>();
    CopyBaseAttributes<Scene, Node>();
    RegisterAttribute("layerNames", &Scene::LayerNamesAttr, &Scene::SetLayerNamesAttr);
    RegisterAttribute("tagNames", &Scene::TagNamesAttr, &Scene::SetTagNamesAttr);
}

void Scene::Save(Stream& dest)
{
    PROFILE(SaveScene);
    
    InfoString("Saving scene to " + dest.FName());
    
    dest.WriteFileID("SCNE");
    Node::Save(dest);
}

bool Scene::Load(Stream& source)
{
    PROFILE(LoadScene);
    
    InfoString("Loading scene from " + source.FName());
    
    FString fileId = source.ReadFileID();
    if (fileId != "SCNE")
    {
        ErrorString("File is not a binary scene file");
        return false;
    }

    FStringHash ownType = source.Read<FStringHash>();
    unsigned ownId = source.Read<unsigned>();
    if (ownType != GetTypeStatic())
    {
        ErrorString("Mismatching type of scene root node in scene file");
        return false;
    }

    Clear();

    FObjectResolver resolver;
    resolver.StoreObject(ownId, this);
    Node::Load(source, resolver);
    resolver.Resolve();

    return true;
}

bool Scene::LoadJSON(const JSONValue& source)
{
    PROFILE(LoadSceneJSON);
    
    FStringHash ownType(source["type"].GetString());
    unsigned ownId = (unsigned)source["id"].GetNumber();

    if (ownType != GetTypeStatic())
    {
        ErrorString("Mismatching type of scene root node in scene file");
        return false;
    }

    Clear();

    FObjectResolver resolver;
    resolver.StoreObject(ownId, this);
    Node::LoadJSON(source, resolver);
    resolver.Resolve();

    return true;
}

bool Scene::LoadJSON(Stream& source)
{
    InfoString("Loading scene from " + source.FName());
    
    JSONFile json;
    bool success = json.Load(source);
    LoadJSON(json.Root());
    return success;
}

bool Scene::SaveJSON(Stream& dest)
{
    PROFILE(SaveSceneJSON);
    
    InfoString("Saving scene to " + dest.FName());
    
    JSONFile json;
    Node::SaveJSON(json.Root());
    return json.Save(dest);
}

Node* Scene::Instantiate(Stream& source)
{
    PROFILE(Instantiate);
    
    FObjectResolver resolver;
    FStringHash childType(source.Read<FStringHash>());
    unsigned childId = source.Read<unsigned>();

    Node* child = CreateChild(childType);
    if (child)
    {
        resolver.StoreObject(childId, child);
        child->Load(source, resolver);
        resolver.Resolve();
    }

    return child;
}

Node* Scene::InstantiateJSON(const JSONValue& source)
{
    PROFILE(InstantiateJSON);
    
    FObjectResolver resolver;
    FStringHash childType(source["type"].GetString());
    unsigned childId = (unsigned)source["id"].GetNumber();

    Node* child = CreateChild(childType);
    if (child)
    {
        resolver.StoreObject(childId, child);
        child->LoadJSON(source, resolver);
        resolver.Resolve();
    }

    return child;
}

Node* Scene::InstantiateJSON(Stream& source)
{
    JSONFile json;
    json.Load(source);
    return InstantiateJSON(json.Root());
}

void Scene::Clear()
{
    RemoveAllChildren();
    _nextNodeId = 1;
}

Node* Scene::FindNode(unsigned id) const
{
    auto it = _nodes.Find(id);
    return it != _nodes.End() ? it->_second : nullptr;
}

TVector<ACamera*>& Scene::GetAllCamera()
{
	return _cameras;
}

void Scene::AddNode(Node* node)
{
    if (!node || node->ParentScene() == this)
        return;

    while (_nodes.Contains(_nextNodeId))
    {
        ++_nextNodeId;
        if (!_nextNodeId)
            ++_nextNodeId;
    }

    Scene* oldScene = node->ParentScene();
    if (oldScene)
    {
        unsigned oldId = node->Id();
        oldScene->_nodes.Erase(oldId);
    }
    _nodes[_nextNodeId] = node;
    node->SetScene(this);
    node->SetId(_nextNodeId);

    ++_nextNodeId;

    // If node has children, add them to the scene as well
    if (node->NumChildren())
    {
        const TVector<TSharedPtr<Node> >& children = node->Children();
        for (auto it = children.Begin(); it != children.End(); ++it)
            AddNode(*it);
    }
}

void Scene::RemoveNode(Node* node)
{
    if (!node || node->ParentScene() != this)
        return;

    _nodes.Erase(node->Id());
    node->SetScene(nullptr);
    node->SetId(0);
    
    // If node has children, remove them from the scene as well
    if (node->NumChildren())
    {
        const TVector<TSharedPtr<Node> >& children = node->Children();
        for (auto it = children.Begin(); it != children.End(); ++it)
            RemoveNode(*it);
    }
}

void Scene::SetPhysicsWorld(PhysicsWorld* physicsWorld)
{
	_physicsWorld = physicsWorld;
}

void Scene::SetSkyBox(ASkyBox* skybox)
{
	_skybox = skybox;
}

PhysicsWorld* Scene::GetPhysicsWorld()
{
	if (_physicsWorld)
	{
		return _physicsWorld;
	}
	WarningString("Physics world failed to read, may be not created");
	return nullptr;
}

ASkyBox* Scene::GetSkyBox()
{
	if (_skybox)
	{
		return _skybox;
	}
	WarningString("SkyBox failed to read, may be not created");
	return nullptr;
}

void Scene::SetupShadowMap(size_t num, int size)
{
	// The scene creates a shadow map by default
	GModuleManager::Get().RendererModule()->SetupShadowMaps(num, size, EImageFormat::D16);

}

void Scene::SetLayerNamesAttr(JSONValue names)
{
    _layerNames.Clear();
    _layers.Clear();

    const JSONArray& array = names.GetArray();
    for (size_t i = 0; i < array.Size(); ++i)
    {
        const FString& name = array[i].GetString();
        _layerNames.Push(name);
        _layers[name] = (unsigned char)i;
    }
}

JSONValue Scene::LayerNamesAttr() const
{
    JSONValue ret;

    ret.SetEmptyArray();
    for (auto it = _layerNames.Begin(); it != _layerNames.End(); ++it)
        ret.Push(*it);
    
    return ret;
}

void Scene::SetTagNamesAttr(JSONValue names)
{
    _tagNames.Clear();
    _tags.Clear();

    const JSONArray& array = names.GetArray();
    for (size_t i = 0; i < array.Size(); ++i)
    {
        const FString& name = array[i].GetString();
        _tagNames.Push(name);
        _tags[name] = (unsigned char)i;
    }
}

JSONValue Scene::TagNamesAttr() const
{
    JSONValue ret;

    ret.SetEmptyArray();
    for (auto it = _tagNames.Begin(); it != _tagNames.End(); ++it)
        ret.Push(*it);

    return ret;
}

void RegisterSceneLibrary()
{
    static bool registered = false;
    if (registered)
        return;
    registered = true;

    Node::RegisterObject();
    Scene::RegisterObject();
    ASpatialNode::RegisterObject();
}

}

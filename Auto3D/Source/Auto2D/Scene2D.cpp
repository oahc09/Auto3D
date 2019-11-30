#include "Debug/Log.h"
#include "Debug/Profiler.h"
#include "IO/Stream.h"
#include "Core/ObjectResolver.h"
#include "Scene/SpatialNode.h"
#include "Resource/JSONFile.h"
#include "RegisteredBox/RegisteredBox.h"
#include "Core/Modules/ModuleManager.h"
#include "Auto2D/PhysicsWorld2D.h"

#include "Scene2D.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

Scene2D::Scene2D() :
	_nextNodeId(1)
{
	// Register self to allow finding by ID
	AddNode(this);

	DefineLayer(LAYER_DEFAULT, "Default");
	DefineTag(TAG_NONE, "None");
	GModuleManager::Get().RegisteredBoxModule()->RegisterScene2D(this);
}
Scene2D::~Scene2D()
{
	// ANode destructor will also remove children. But at that point the node<>_id maps have been destroyed 
   // so must tear down the scene tree already here
	RemoveAllChildren();
	RemoveNode(this);
	assert(_nodes.IsEmpty());
}

void Scene2D::RegisterObject()
{
	RegisterFactory<Scene2D>();
	CopyBaseAttributes<Scene2D, Node2D>();
	RegisterAttribute("layerNames", &Scene2D::LayerNamesAttr, &Scene2D::SetLayerNamesAttr);
	RegisterAttribute("tagNames", &Scene2D::TagNamesAttr, &Scene2D::SetTagNamesAttr);
}


void Scene2D::Save(FStream& dest)
{
	PROFILE(SaveScene);

	InfoString("Saving scene to " + dest.FName());

	dest.WriteFileID("SCNE");
	Node2D::Save(dest);
}

bool Scene2D::Load(FStream& source)
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
	Node2D::Load(source, resolver);
	resolver.Resolve();

	return true;
}

bool Scene2D::LoadJSON(const FJSONValue& source)
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
	Node2D::LoadJSON(source, resolver);
	resolver.Resolve();

	return true;
}

bool Scene2D::LoadJSON(FStream& source)
{
	InfoString("Loading scene from " + source.FName());

	AJSONFile json;
	bool success = json.Load(source);
	LoadJSON(json.Root());
	return success;
}

bool Scene2D::SaveJSON(FStream& dest)
{
	PROFILE(SaveSceneJSON);

	InfoString("Saving scene to " + dest.FName());

	AJSONFile json;
	Node2D::SaveJSON(json.Root());
	return json.Save(dest);
}

Node2D* Scene2D::Instantiate(FStream& source)
{
	PROFILE(Instantiate);

	FObjectResolver resolver;
	FStringHash childType(source.Read<FStringHash>());
	unsigned childId = source.Read<unsigned>();

	Node2D* child = CreateChild(childType);
	if (child)
	{
		resolver.StoreObject(childId, child);
		child->Load(source, resolver);
		resolver.Resolve();
	}

	return child;
}

Node2D* Scene2D::InstantiateJSON(const FJSONValue& source)
{
	PROFILE(InstantiateJSON);

	FObjectResolver resolver;
	FStringHash childType(source["type"].GetString());
	unsigned childId = (unsigned)source["id"].GetNumber();

	Node2D* child = CreateChild(childType);
	if (child)
	{
		resolver.StoreObject(childId, child);
		child->LoadJSON(source, resolver);
		resolver.Resolve();
	}

	return child;
}

Node2D* Scene2D::InstantiateJSON(FStream& source)
{
	AJSONFile json;
	json.Load(source);
	return InstantiateJSON(json.Root());
}

void Scene2D::Clear()
{
	RemoveAllChildren();
	_nextNodeId = 1;
}

Node2D* Scene2D::FindNode(unsigned id) const
{
	auto it = _nodes.Find(id);
	return it != _nodes.End() ? it->_second : nullptr;
}

const THashMap<unsigned, Node2D*>& Scene2D::GetAllNode() const
{
	return _nodes;
}

TVector<Camera2D*>& Scene2D::GetAllCamera()
{
	return _cameras;
}

void Scene2D::AddNode(Node2D* node)
{
	if (!node || node->ParentScene2D() == this)
		return;

	while (_nodes.Contains(_nextNodeId))
	{
		++_nextNodeId;
		if (!_nextNodeId)
			++_nextNodeId;
	}

	Scene2D* oldScene = node->ParentScene2D();
	if (oldScene)
	{
		unsigned oldId = node->Id();
		oldScene->_nodes.Erase(oldId);
	}
	_nodes[_nextNodeId] = node;
	node->SetScene2D(this);
	node->SetId(_nextNodeId);

	++_nextNodeId;

	// If node has children, add them to the scene as well
	if (node->NumChildren())
	{
		const TVector<TSharedPtr<Node2D> >& children = node->Children();
		for (auto it = children.Begin(); it != children.End(); ++it)
			AddNode(*it);
	}
}

void Scene2D::RemoveNode(Node2D* node)
{
	if (!node || node->ParentScene2D() != this)
		return;

	_nodes.Erase(node->Id());
	node->SetScene2D(nullptr);
	node->SetId(0);

	// If node has children, remove them from the scene as well
	if (node->NumChildren())
	{
		const TVector<TSharedPtr<Node2D> >& children = node->Children();
		for (auto it = children.Begin(); it != children.End(); ++it)
			RemoveNode(*it);
	}
}

void Scene2D::SetPhysicsWorld(PhysicsWorld2D* physicsWorld)
{
	_physicsWorld = physicsWorld;
}

PhysicsWorld2D* Scene2D::GetPhysicsWorld()
{
	if (_physicsWorld)
	{
		return _physicsWorld;
	}
	WarningString("Physics world failed to read, may be not created");
	return nullptr;
}

void Scene2D::SetLayerNamesAttr(FJSONValue names)
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

FJSONValue Scene2D::LayerNamesAttr() const
{
	FJSONValue ret;

	ret.SetEmptyArray();
	for (auto it = _layerNames.Begin(); it != _layerNames.End(); ++it)
		ret.Push(*it);

	return ret;
}

void Scene2D::SetTagNamesAttr(FJSONValue names)
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

FJSONValue Scene2D::TagNamesAttr() const
{
	FJSONValue ret;

	ret.SetEmptyArray();
	for (auto it = _tagNames.Begin(); it != _tagNames.End(); ++it)
		ret.Push(*it);

	return ret;
}

}
#pragma once

#include "Core/Object/Object.h"

namespace Auto3D
{

class AWorld;
class FObjectResolver;

static const unsigned short NF_ENABLED = 0x1;
static const unsigned short NF_TEMPORARY = 0x2;
static const unsigned short NF_SPATIAL = 0x4;
static const unsigned short NF_SPATIAL_PARENT = 0x8;
static const unsigned short NF_WORLD_TRANSFORM_DIRTY = 0x10;
static const unsigned short NF_BOUNDING_BOX_DIRTY = 0x20;
static const unsigned short NF_OCTREE_UPDATE_QUEUED = 0x40;
static const unsigned short NF_GEOMETRY = 0x80;
static const unsigned short NF_LIGHT = 0x100;
static const unsigned short NF_CASTSHADOWS = 0x200;
static const unsigned char LAYER_DEFAULT = 0x0;
static const unsigned char TAG_NONE = 0x0;
static const unsigned LAYERMASK_ALL = 0xffffffff;

/// Base class for scene nodes.
class AUTO_API ANode : public AObject
{
    DECLARE_CLASS(ANode, AObject)

public:
    /// Construct.
    ANode();
    /// Destruct. Destroy any child nodes.
    virtual ~ANode();

    /// Return unique _id within the scene, or 0 if not in a scene.
    unsigned Id() const { return _id; }
    /// Set name. Is not required to be unique within the scene.
    void SetName(const FString& newName);
    /// Set node's layer. Usage is subclass specific, for example rendering nodes selectively. Default is 0.
    void SetLayer(unsigned char newLayer);
    /// Set node's layer by name. The layer name must have been registered to the scene root beforehand.
    void SetLayerName(const FString& newLayerName);
    /// Set node's tag, which can be used to search for specific nodes.
    void SetTag(unsigned char newTag);
    /// Set node's tag by name. The tag name must have been registered to the scene root beforehand.
    void SetTagName(const FString& newTagName);
    /// Set enabled status. Meaning is subclass specific.
    void SetEnabled(bool enable);
    /// Set enabled status recursively in the child hierarchy.
    void SetEnabledRecursive(bool enable);
    /// Set temporary mode. Temporary scene nodes are not saved.
    void SetTemporary(bool enable);
    /// Reparent the node.
    void SetParent(ANode* newParent);

    /// Create child node of specified type. A registered object factory for the type is required.
    ANode* CreateChild(FStringHash childType);
    /// Create named child node of specified type.
    ANode* CreateChild(FStringHash childType, const FString& childName);
    /// Create named child node of specified type.
    ANode* CreateChild(FStringHash childType, const char* childName);
    /// Add node as a child. Same as calling SetParent for the child node.
    void AddChild(ANode* child);
    /// Remove child node. Will delete it if there are no other strong references to it.
    void RemoveChild(ANode* child);
    /// Remove child node by index.
    void RemoveChild(size_t index);
    /// Remove all child nodes.
    void RemoveAllChildren();
    /// Remove self immediately. As this will delete the node (if no other strong references exist) no operations on the node are permitted after calling this.
    void RemoveSelf();
    /// Create child node of the specified type, template version.
    template <typename _Ty> _Ty* CreateChild() { return static_cast<_Ty*>(CreateChild(_Ty::GetTypeHashStatic())); }
    /// Create named child node of the specified type, template version.
    template <typename _Ty> _Ty* CreateChild(const FString& childName) { return static_cast<_Ty*>(CreateChild(_Ty::GetTypeHashStatic(), childName)); }
    /// Create named child node of the specified type, template version.
    template <typename _Ty> _Ty* CreateChild(const char* childName) { return static_cast<_Ty*>(CreateChild(_Ty::GetTypeHashStatic(), childName)); }

    /// Return name.
    const FString& GetName() const { return _name; }
    /// Return layer.
    unsigned char GetLayer() const { return _layer; }
    /// Return layer name, or empty if not registered in the scene root.
    const FString& GetLayerName() const;
    /// Return bitmask corresponding to layer.
    unsigned GetLayerMask() const { return 1 << _layer; }
    /// Return tag.
    unsigned char GetTag() const { return _tag; }
    /// Return tag name, or empty if not registered in the scene root.
    const FString& GetTagName() const;
    /// Return enabled status.
    bool IsEnabled() const { return TestFlag(NF_ENABLED); }
    /// Return whether is temporary.
    bool IsTemporary() const { return TestFlag(NF_TEMPORARY); }
    /// Return parent node.
    ANode* Parent() const { return _parent; }
    /// Return the scene that the node belongs to.
    AWorld* GetWorld() const { return _scene; }
    /// Return number of immediate child nodes.
    size_t NumChildren() const { return _children.Size(); }
    /// Return number of immediate child nodes that are not temporary.
    size_t NumPersistentChildren() const;
    /// Return immediate child node by index.
    ANode* Child(size_t index) const { return index < _children.Size() ? _children[index].Get() : nullptr; }
    /// Return all immediate child nodes.
    const TVector<TSharedPtr<ANode> >& Children() const { return _children; }
    /// Return child nodes recursively.
    void AllChildren(TVector<ANode*>& result) const;
    /// Return first child node that matches name.
    ANode* FindChild(const FString& childName, bool recursive = false) const;
    /// Return first child node that matches name.
    ANode* FindChild(const char* childName, bool recursive = false) const;
    /// Return first child node of specified type.
    ANode* FindChild(FStringHash childType, bool recursive = false) const;
    /// Return first child node that matches type and name.
    ANode* FindChild(FStringHash childType, const FString& childName, bool recursive = false) const;
    /// Return first child node that matches type and name.
    ANode* FindChild(FStringHash childType, const char* childName, bool recursive = false) const;
    /// Return first child node that matches layer mask.
    ANode* FindChildByLayer(unsigned layerMask, bool recursive = false) const;
    /// Return first child node that matches tag.
    ANode* FindChildByTag(unsigned char tag, bool recursive = false) const;
    /// Return first child node that matches tag name.
    ANode* FindChildByTag(const FString& tagName, bool recursive = false) const;
    /// Return first child node that matches tag name.
    ANode* FindChildByTag(const char* tagName, bool recursive = false) const;
    /// Find child nodes of specified type.
    void FindChildren(TVector<ANode*>& result, FStringHash childType, bool recursive = false) const;
    /// Find child nodes that match layer mask.
    void FindChildrenByLayer(TVector<ANode*>& result, unsigned layerMask, bool recursive = false) const;
    /// Find child nodes that match tag.
    void FindChildrenByTag(TVector<ANode*>& result, unsigned char tag, bool recursive = false) const;
    /// Find child nodes that match tag name.
    void FindChildrenByTag(TVector<ANode*>& result, const FString& tagName, bool recursive = false) const;
    /// Find child nodes that match tag name.
    void FindChildrenByTag(TVector<ANode*>& result, const char* tagName, bool recursive = false) const;
    /// Return first child node of specified type, template version.
    template <typename _Ty> _Ty* FindChild(bool recursive = false) const { return static_cast<_Ty*>(FindChild(_Ty::GetTypeHashStatic(), recursive)); }
    /// Return first child node that matches type and name, template version.
    template <typename _Ty> _Ty* FindChild(const FString& childName, bool recursive = false) const { return static_cast<_Ty*>(FindChild(_Ty::GetTypeHashStatic(), childName, recursive)); }
    /// Return first child node that matches type and name, template version.
    template <typename _Ty> _Ty* FindChild(const char* childName, bool recursive = false) const { return static_cast<_Ty*>(FindChild(_Ty::GetTypeHashStatic(), childName, recursive)); }
    /// Find child nodes of specified type, template version.
    template <typename _Ty> void FindChildren(TVector<_Ty*>& result, bool recursive = false) const { return FindChildren(reinterpret_cast<TVector<ANode*>&>(result), _Ty::GetTypeHashStatic(), recursive); }
    
    /// Set bit flag. Called internally.
    void SetFlag(unsigned short bit, bool set) const { if (set) _flags |= bit; else _flags &= ~bit; }
    /// Test bit flag. Called internally.
    bool TestFlag(unsigned short bit) const { return (_flags & bit) != 0; }
    /// Return bit flags. Used internally eg. by octree queries.
    unsigned short Flags() const { return _flags; }
    /// Assign node to a new scene. Called internally.
    void SetScene(AWorld* newScene);
    /// Assign new _id. Called internally.
    void SetId(unsigned newId);

protected:
    /// Handle being assigned to a new parent node.
	virtual void OnParentSet(ANode* newParent, ANode* oldParent) {}
    /// Handle being assigned to a new scene.
    virtual void OnWorldSet(AWorld* newScene, AWorld* oldScene) {}
    /// Handle the enabled status changing.
    virtual void OnSetEnabled(bool newEnabled) {}
protected:
    /// Parent node.
    ANode* _parent;
    /// Parent scene (If in the scene)
    AWorld* _scene;
    /// Child nodes.
    TVector<TSharedPtr<ANode> > _children;
    /// Id within the scene.
    unsigned _id;
    /// %ANode name.
    FString _name;
    /// %ANode flags. Used to hold several boolean values (some subclass-specific) to reduce memory use.
    mutable unsigned short _flags;
    /// Layer number.
    unsigned char _layer;
    /// Tag number.
    unsigned char _tag;
};

}

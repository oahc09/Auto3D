#pragma once
#include "LevelBehaviorObject.h"
#include "Node.h"

namespace Auto3D {

const static char* SCENE_ATTACH = "Scene_attach";
const static char* GEOMETRY_ATTACH = "GeoMetry_attach";

class Component : public LevelBehaviorObject
{
	REGISTER_OBJECT_ABSTRACT_CLASS(Component, LevelBehaviorObject)
public:
	/**
	* @brief : Construct
	*/
	explicit Component(SharedPtr<Ambient> ambient);
	/**
	* @brief : Get game object ptr
	*/
	SharedPtr<Node> GetNode();
	/**
	* @brief : Get game object ptr const
	*/
	SharedPtr<Node> GetNode() const;
	/**
	* @brief : Mount component for gameobject
	*/
	void SetNode(SharedPtr<Node> node);
	/**
	* @brief : Return vector3 with gameObject position
	*/
	Vector3 GetPosition();
	/**
	* @brief : Return int with scene id
	*/
	const int GetLevelID();
	/**
	* @brief : Return current scene node
	*/
	SharedPtr<Node> GetCurrentSceneNode();
	/**
	* @brief : This component is automatically invoked when registering to a node
	*/
	virtual void Init()
	{
		_levelID = GetNode()->GetLevelID();
	}
	/**
	* @brief : Destory this component
	*/
	virtual void Destory() {}
private:
	/// WeakPtr get this component node
	WeakPtr<Node> _node;
};

}
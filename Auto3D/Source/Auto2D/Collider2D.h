#pragma once
#include "Node2D.h"
#include <Box2D.h>

namespace Auto3D{
	
class RigidBody2D;
class PhysicsWorld2D;

class AUTO_API Collider2D : public Node2D
{
	REGISTER_OBJECT_CLASS(Collider2D,Node2D)
public:
	/// Construct
	Collider2D();
	/// Destructor
	virtual ~Collider2D();
	/// Register object factory.
	static void RegisterObject();
	/// Update the new collision shape to the RigidBody.
	void NotifyRigidBody(bool updateMass = true);

	void ReleaseShape();
	/// This function is called when the parent node of this class is assigned.
	virtual void ParentCallBack() override;
protected:
	/// Physics world form this collider
	WeakPtr<PhysicsWorld2D> _physicsWorld;
	/// Rigid body.
	WeakPtr<RigidBody2D> _rigidBody;
	/// Fixture def.
	b2FixtureDef _fixtureDef;
	///// Box2D fixture.
	b2Fixture* _fixture;
};

}


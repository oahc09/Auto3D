#include "ColliderCapsule.h"
#include "PhysicsUtils.h"
#include "PhysicsWorld.h"


namespace Auto3D
{

AColliderCapsule::AColliderCapsule() :
	_radius(1.0f),
	_height(1.0f)
{
	_shapeType = EShapeType::CAPSULE;
	
}

AColliderCapsule::~AColliderCapsule()
{
}

void AColliderCapsule::RegisterObject()
{
	RegisterFactory<AColliderCapsule>();
}

void AColliderCapsule::SetSize(float radius, float height)
{
	_radius = radius;
	_height = height;
	Resize(_radius, _height);
}

void AColliderCapsule::SetRadius(float radius)
{
	_radius = radius;
	Resize(_radius, _height);
}

void AColliderCapsule::SetHeight(float height)
{
	_height = height;
	Resize(_radius, _height);
}

void AColliderCapsule::Resize(float radius, float height)
{
	ReleaseShape();

	_shape.Reset();
	_shape = new btCapsuleShape(radius, height);

	NotifyRigidBody();
}

}
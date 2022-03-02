#include "PhysicsXObject.h"
#include "../CSC8503Common/Transform.h"
#include "../CSC8503Common/CollisionVolume.h"
#include "PhysXConvert.h"
using namespace NCL;
using namespace CSC8503;
PhysicsXObject::PhysicsXObject(PxTransform& _transform, PxShape& _volume) :
	transform(_transform), volume(_volume)
{

}
PhysicsXObject::~PhysicsXObject()
{
}

void PhysicsXObject::AddForce(const Vector3& force) {
	rb->addForce(PhysXConvert::Vector3ToPxVec3(force));
}

void PhysicsXObject::AddTorque(const Vector3& torque) {
	rb->addTorque(PhysXConvert::Vector3ToPxVec3(torque));
}


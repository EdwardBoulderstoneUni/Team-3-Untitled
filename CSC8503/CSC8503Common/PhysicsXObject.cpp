#include "PhysicsXObject.h"
#include "../CSC8503Common/CollisionVolume.h"
#include "../../include/PhysX/PxRigidDynamic.h"
using namespace NCL;
using namespace CSC8503;
#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}

PhysicsXObject::PhysicsXObject(Transform _trans, PxGeometry* _volume, bool isDynamic)
{
	transform.p = PhysXConvert::Vector3ToPxVec3(_trans.GetPosition());
	transform.q = PhysXConvert::QuatToPxQuat(_trans.GetOrientation());
	volume = _volume;
	dynamic = isDynamic;
}

PhysicsXObject::~PhysicsXObject()
{
	PX_RELEASE(rb);
}

void PhysicsXObject::setGlobalPose(const Transform& pos)
{
	PxTransform transform;
	transform.p = PhysXConvert::Vector3ToPxVec3(pos.GetPosition());
	transform.q = PhysXConvert::QuatToPxQuat(pos.GetOrientation());
	rb->setGlobalPose(transform);
}

void PhysicsXObject::AddForce(const Vector3& force) {
	if(!isDynamic())return;
	rb->is < PxRigidDynamic >()->addForce(PhysXConvert::Vector3ToPxVec3(force));
}

void PhysicsXObject::AddTorque(const Vector3& torque) {
	if (!isDynamic())return;
	rb->is < PxRigidDynamic >()->addTorque(PhysXConvert::Vector3ToPxVec3(torque));
}

void PhysicsXObject::ClearForces()
{
	/*rb->clearForce();*/
}

void PhysicsXObject::ClearTorque()
{
	/*rb->clearTorque();*/
}

void PhysicsXObject::SetLinearVelocity(const Vector3& v)
{
	if (!isDynamic())return;
	PxVec3 pxV = PhysXConvert::Vector3ToPxVec3(v);
	rb->is < PxRigidDynamic >()->setLinearVelocity(pxV);
}

void PhysicsXObject::SetAngularVelocity(const Vector3& v)
{
	if (!isDynamic())return;
	PxVec3 pxV = PhysXConvert::Vector3ToPxVec3(v);
	rb->is < PxRigidDynamic >()->setAngularVelocity(pxV);
}

bool PhysicsXObject::isDynamic()
{
	return dynamic;
}

bool PhysicsXObject::isInScene()
{
	return rb == nullptr ? false: true;
}


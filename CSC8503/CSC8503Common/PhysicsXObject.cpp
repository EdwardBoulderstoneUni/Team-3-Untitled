#include "PhysicsXObject.h"
#include "../../include/PhysX/PxRigidDynamic.h"
#include "../CSC8503Common/PhysXConvert.h"
using namespace NCL;
using namespace CSC8503;
#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}
PhysicsXObject::PhysicsXObject()
{
	properties = PhyProperties();
	rb = nullptr;
	controller = nullptr;
}

PhysicsXObject::~PhysicsXObject()
{
	PX_RELEASE(rb);
	PX_RELEASE(controller);
}
void PhysicsXObject::SetLinearVelocity(Vector3 v) {
	PxRigidDynamic* actor = rb->is<PxRigidDynamic>();
	if (!actor)return;
	actor->setLinearVelocity(PhysXConvert::Vector3ToPxVec3(v));
}




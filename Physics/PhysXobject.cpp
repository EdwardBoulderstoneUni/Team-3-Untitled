#include "PhysXobject.h"

using namespace physx;

PhysXobject::PhysXobject(PxRigidActor* act, PxMaterial* m) {
	actor = act;
	material = m;
	if (actor->is<PxRigidDynamic>()) {
		PxRigidDynamic* body = (PxRigidDynamic*)act;
		body->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);		//Enable CCD so high v objects don't move through each other
	}
}

PhysXobject::~PhysXobject() {
	if (actor != NULL) {
		actor->release();
		material->release();
	}
}

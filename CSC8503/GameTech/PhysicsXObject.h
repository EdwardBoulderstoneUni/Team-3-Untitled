#pragma once
#include "../../include/PhysX/PxPhysicsAPI.h"
using namespace physx;
class PhysicsXObject {
public:
	PhysicsXObject(PxTransform& _transform, PxShape& _volume);
	~PhysicsXObject();
	const PxTransform& GetTransform() { return transform; }
	PxShape& GetVolume() { return volume; }
	void SetRigidDynamic(PxRigidDynamic* actor) {
		dynamic = actor;
	}
	void SetGravity(bool status) {
		if(!status)dynamic->setActorFlag(PxActorFlag::eDISABLE_GRAVITY,true);
	}
protected:
	PxTransform& transform;
	PxShape& volume;
	PxRigidDynamic* dynamic = nullptr;
};
		
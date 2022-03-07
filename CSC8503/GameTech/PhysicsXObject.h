#pragma once
#include "../CSC8503Common/Transform.h"
#include "PhysXAPI.h"
#include "PhysXConvert.h"

using namespace physx;
using namespace NCL::Maths;
using namespace NCL::CSC8503;

class PhysicsXObject {
public:
	PhysicsXObject(Transform _trans, PxGeometry* _volume, bool isDynamic=true);
	~PhysicsXObject();

	void SetRigActor(PxRigidActor* actor) { rb = actor; }

	void SetGravity(bool status) {
		if(!status)rb->setActorFlag(PxActorFlag::eDISABLE_GRAVITY,true);
	}

	void setGlobalPose(const Transform& pos);

	float GetInverseMass() const{}
	PxGeometry* GetVolume() const{ return volume; }
	void AddForce(const Vector3& force);

	void AddTorque(const Vector3& torque);

	
	void ClearForces();
	void ClearTorque();
	void SetLinearVelocity(const Vector3& v);

	void SetAngularVelocity(const Vector3& v);

	bool isDynamic();
	bool isInScene();
	
protected:
	bool dynamic;
	PxTransform transform;
	PxGeometry* volume;
	PxRigidActor* rb;
};
		
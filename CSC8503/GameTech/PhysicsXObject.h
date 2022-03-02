#pragma once
#include "../../include/PhysX/PxPhysicsAPI.h"
#include "../../Common/Vector3.h"
using namespace physx;
using namespace NCL::Maths;
class PhysicsXObject {
public:
	PhysicsXObject(PxTransform& _transform, PxShape& _volume);
	~PhysicsXObject();

	const PxTransform& GetTransform() { return transform; }
	PxShape& GetVolume() { return volume; }
	PxRigidBody* GetRigidBody() { return rb; }

	void SetRigidBody(PxRigidBody* actor) {
		rb = actor;
	}

	void SetGravity(bool status) {
		if(!status)rb->setActorFlag(PxActorFlag::eDISABLE_GRAVITY,true);
	}


	float GetInverseMass() const
	{
		return rb->getInvMass();
	}

	//void ApplyAngularImpulse(const Vector3& force);
	//void ApplyLinearImpulse(const Vector3& force);

	void AddForce(const Vector3& force);

	//void AddForceAtPosition(const Vector3& force, const Vector3& position);

	void AddTorque(const Vector3& torque);

	
	//void ClearForces();

	//void SetLinearVelocity(const Vector3& v)
	//{
	//	linearVelocity = v;
	//}

	//void SetAngularVelocity(const Vector3& v)
	//{
	//	angularVelocity = v;
	//}


protected:
	PxTransform& transform;
	PxShape& volume;
	PxRigidBody* rb;
};
		
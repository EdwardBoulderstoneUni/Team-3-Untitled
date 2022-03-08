#pragma once
#include "../CSC8503Common/Transform.h"
#include "../../include/PhysX/geometry/PxGeometry.h"
#include "../../include/PhysX/PxRigidActor.h"
#include "PhysXConvert.h"

using namespace physx;
using namespace NCL::Maths;
using namespace NCL::CSC8503;

class PhysicsXObject {
public:
	PhysicsXObject() {}
	PhysicsXObject(Transform _trans, PxGeometry* _volume, bool isDynamic=true);
	~PhysicsXObject();

	void SetRigActor(PxRigidActor* actor) { rb = actor; }
	void SetGravity(bool status) {
		
	}
	void SetMass(float _Mass) {Mass = _Mass; }
	void SetTransform(const Transform& pos);
	void setVolume(PxGeometry* _volume);
	void setDynaimc(const bool _dynamic);
	void SetLinearVelocity(const Vector3& v);
	void SetAngularVelocity(const Vector3& v);

	float GetMass() const { return Mass; }
	PxGeometry* GetVolume() const{ return volume; }

	void AddForce(const Vector3& force);
	void AddTorque(const Vector3& torque);

	void ClearForces();
	void ClearTorque();

	bool isDynamic();
	bool isInScene();
	
protected:
	bool dynamic;
	float Mass;
	PxTransform transform;
	PxGeometry* volume;
	PxRigidActor* rb;
};
		
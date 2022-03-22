#pragma once
#include "../CSC8503Common/Transform.h"
#include "../../include/PhysX/geometry/PxGeometry.h"
#include "../../include/PhysX/PxRigidActor.h"
#include "../../include/PhysX/characterkinematic/PxController.h"
using namespace physx;
using namespace NCL::Maths;
using namespace NCL::CSC8503;
struct PhyProperties
{
	PhyProperties() {
		Mass = 0.0f;
		transform = PxTransform();
		volume = nullptr;
		type = None;
	}
	enum Type
	{
		Dynamic,
		Static,
		Character,
		None
	};

	float Mass;
	PxTransform transform;
	PxGeometry* volume;
	Type type;
	Vector3 positionOffset;
};
class PhysicsXObject {
public:
	PhysicsXObject();
	~PhysicsXObject();
	void SetLinearVelocity(Vector3 v);
	void CMove(PxVec3 disp);
	void CTrans(PxExtendedVec3 pos);
	PhyProperties properties;
	PxRigidActor* rb;
	PxController* controller;
};

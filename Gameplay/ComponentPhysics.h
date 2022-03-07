#pragma once
#include "Component.h"
#include "../CSC8503/CSC8503Common/Transform.h"
#include "../CSC8503/CSC8503Common/PhysicsXObject.h"
class ComponentPhysics : public Component {
public:
	enum ShapeType
	{
		Box,
		Capsule,
		Sphere,
		Cylinder,
		Plane,
		Mesh
	};
	enum MotionType {
		Static,
		Dynamic
	};
	void Init() override;
	void Update() override;
	float mass;
	bool useGravity=true;
	Vector3 center;
	Quaternion orientation;
	ShapeType shapeType;
	Vector3 size;
	MotionType motionType;
	PhysicsXObject* phyObj;
private:
};
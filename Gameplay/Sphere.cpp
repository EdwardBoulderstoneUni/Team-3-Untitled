#include "Sphere.h"

NCL::CSC8503::Sphere::Sphere()
{
}

NCL::CSC8503::Sphere::~Sphere()
{
}

void NCL::CSC8503::Sphere::SetUp()
{
	auto physics = new ComponentPhysics();
	physics->phyObj = GetPhysicsXObject();

	PhyProperties properties = PhyProperties();
	properties.type = PhyProperties::Dynamic;
	properties.transform = PhysXConvert::TransformToPxTransform(GetTransform());
	properties.Mass = 500.0f;
	Vector3 scale = GetTransform().GetScale();
	properties.volume = new PxSphereGeometry(scale.x);

	physics->phyObj->properties = properties;
	PushComponent(physics);
}

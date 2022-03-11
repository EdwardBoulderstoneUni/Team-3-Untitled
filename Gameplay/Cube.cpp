#include "Cube.h"

NCL::CSC8503::Cube::Cube()
{
}

NCL::CSC8503::Cube::~Cube()
{
}

void NCL::CSC8503::Cube::SetUp()
{
	auto physics = new ComponentPhysics();
	physics->phyObj = GetPhysicsXObject();

	PhyProperties properties = PhyProperties();
	properties.type = PhyProperties::Dynamic;
	properties.transform = PhysXConvert::TransformToPxTransform(GetTransform());
	properties.Mass = 10.0f;
	Vector3 scale=GetTransform().GetScale();
	properties.volume = new PxBoxGeometry(PhysXConvert::Vector3ToPxVec3(scale/2));
	
	physics->phyObj->properties = properties;
	PushComponent(physics);
}

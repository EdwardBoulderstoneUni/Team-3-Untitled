#include "Floor.h"

NCL::CSC8503::Floor::Floor()
{

}

NCL::CSC8503::Floor::~Floor()
{
}

void NCL::CSC8503::Floor::SetUp()
{
	auto physics = new ComponentPhysics();
	physics->phyObj = GetPhysicsXObject();

	PhyProperties properties = PhyProperties();
	properties.type = PhyProperties::Static;
	properties.transform = PhysXConvert::TransformToPxTransform(GetTransform());
	properties.Mass = 10.0f;
	Vector3 scale = GetTransform().GetScale();
	properties.volume = new PxBoxGeometry(PhysXConvert::Vector3ToPxVec3(scale));

	type = GameObjectType_floor;

	physics->phyObj->properties = properties;
	PushComponet(physics);
}

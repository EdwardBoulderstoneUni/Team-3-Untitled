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
	physics->motionType = ComponentPhysics::Static;
	physics->center = this->GetTransform().GetPosition();
	physics->orientation = this->GetTransform().GetOrientation();
	physics->size = this->GetTransform().GetScale();
	physics->shapeType = ComponentPhysics::Box;
	physics->mass = 10.0f;
	physics->phyObj = GetPhysicsXObject();

	PushComponet(physics);
}

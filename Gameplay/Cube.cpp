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
	physics->motionType = ComponentPhysics::Dynamic;
	physics->center = this->GetTransform().GetPosition();
	physics->orientation = this->GetTransform().GetOrientation();
	physics->size = this->GetTransform().GetScale();
	physics->shapeType = ComponentPhysics::Box;
	physics->mass = 10.0f;
	physics->phyObj = GetPhysicsXObject();

	PushComponet(physics);
}

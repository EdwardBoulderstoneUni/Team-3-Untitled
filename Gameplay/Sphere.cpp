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
	physics->motionType = ComponentPhysics::Dynamic;
	physics->center = this->GetTransform().GetPosition();
	physics->orientation = this->GetTransform().GetOrientation();
	physics->size = this->GetTransform().GetScale();
	physics->shapeType = ComponentPhysics::Sphere;
	physics->mass = 10.0f;
	physics->phyObj = GetPhysicsXObject();

	PushComponet(physics);
}

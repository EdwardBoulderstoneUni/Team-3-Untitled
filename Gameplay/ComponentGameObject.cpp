#include "ComponentGameObject.h"

NCL::CSC8503::ComponentGameObject::ComponentGameObject()
{

}

NCL::CSC8503::ComponentGameObject::~ComponentGameObject()
{
	for (auto i : components) {
		if (i == nullptr)continue;
		delete i;
	}
		
}

ComponentCamera* NCL::CSC8503::ComponentGameObject::GetComponentCamera()
{
	return (ComponentCamera*)components[Component::Camera];
}

ComponentInput* NCL::CSC8503::ComponentGameObject::GetComponentInput()
{
	return (ComponentInput* )components[Component::Input];
}

ComponentPhysics* NCL::CSC8503::ComponentGameObject::GetComponentPhysics()
{
	return (ComponentPhysics*)components[Component::Physics];
}

void NCL::CSC8503::ComponentGameObject::Update(float dt)
{
	UpdateComponents(dt);
}

void NCL::CSC8503::ComponentGameObject::InitAllComponent()
{
	SetUp();
	InitComponents();
}

void NCL::CSC8503::ComponentGameObject::PushComponent(Component* newCom)
{
	switch (newCom->type)
	{
	case Component::Camera:
		components[Component::Camera] = newCom;
		break;
	case Component::Input:
		components[Component::Input] = newCom;
		break;
	case Component::Physics:
		components[Component::Physics] = newCom;
		break;
	default:
		break;
	}
}

void NCL::CSC8503::ComponentGameObject::UpdateComponents(float dt)
{
	for (auto i : components) {
		if (i == nullptr)continue;
		i->Update(dt);
	}
		
}

void NCL::CSC8503::ComponentGameObject::InitComponents()
{
	for (auto i : components) {
		if (i == nullptr)continue;
		i->Init();
	}
}


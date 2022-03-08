#include "ComponetGameObject.h"

NCL::CSC8503::ComponetGameObject::ComponetGameObject()
{

}

NCL::CSC8503::ComponetGameObject::~ComponetGameObject()
{
	for (auto i : components) {
		if (i == nullptr)continue;
		delete i;
	}
		
}

ComponentCamera* NCL::CSC8503::ComponetGameObject::GetComponentCamera()
{
	return (ComponentCamera*)components[Component::Camera];
}

ComponentInput* NCL::CSC8503::ComponetGameObject::GetComponentInput()
{
	return (ComponentInput* )components[Component::Input];
}

ComponentPhysics* NCL::CSC8503::ComponetGameObject::GetComponentPhysics()
{
	return (ComponentPhysics*)components[Component::Physics];
}

void NCL::CSC8503::ComponetGameObject::Update(float dt)
{
	UpdateComponents(dt);
}

void NCL::CSC8503::ComponetGameObject::InitAllComponet()
{
	SetUp();
	InitComponents();
}

void NCL::CSC8503::ComponetGameObject::PushComponet(Component* newCom)
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

void NCL::CSC8503::ComponetGameObject::UpdateComponents(float dt)
{
	for (auto i : components) {
		if (i == nullptr)continue;
		i->Update(dt);
	}
		
}

void NCL::CSC8503::ComponetGameObject::InitComponents()
{
	for (auto i : components) {
		if (i == nullptr)continue;
		i->Init();
	}
}


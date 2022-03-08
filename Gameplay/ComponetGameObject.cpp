#include "ComponetGameObject.h"

NCL::CSC8503::ComponetGameObject::ComponetGameObject()
{
	
}

NCL::CSC8503::ComponetGameObject::~ComponetGameObject()
{
	for (auto i : components)
		delete i;
}

ComponentCamera* NCL::CSC8503::ComponetGameObject::GetComponentCamera()
{
	return (ComponentCamera*)(components.at(2));
}

ComponentInput* NCL::CSC8503::ComponetGameObject::GetComponentInput()
{
	return (ComponentInput* )(components.at(1));
}

ComponentPhysics* NCL::CSC8503::ComponetGameObject::GetComponentPhysics()
{
	return (ComponentPhysics*)(components.at(0));
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
	components.emplace_back(newCom);
}

std::vector<Component*> NCL::CSC8503::ComponetGameObject::GetComponets()
{
	return components;
}

void NCL::CSC8503::ComponetGameObject::UpdateComponents(float dt)
{
	for (auto i : components)
		i->Update(dt);
}

void NCL::CSC8503::ComponetGameObject::InitComponents()
{
	for (auto i : components)
		i->Init();
}


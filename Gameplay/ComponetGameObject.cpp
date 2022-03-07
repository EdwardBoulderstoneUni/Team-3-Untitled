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
	return nullptr;
}

ComponentInput* NCL::CSC8503::ComponetGameObject::GetComponentInput()
{
	return nullptr;
}

ComponentPhysics* NCL::CSC8503::ComponetGameObject::GetComponentPhysics()
{
	return nullptr;
}

void NCL::CSC8503::ComponetGameObject::Update()
{
	UpdateComponents();
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

void NCL::CSC8503::ComponetGameObject::UpdateComponents()
{
	for (auto i : components)
		i->Update();
}

void NCL::CSC8503::ComponetGameObject::InitComponents()
{
	for (auto i : components)
		i->Init();
}


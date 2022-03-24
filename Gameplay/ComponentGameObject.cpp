#include "ComponentGameObject.h"

NCL::CSC8503::ComponentGameObject::ComponentGameObject()
{

}

NCL::CSC8503::ComponentGameObject::~ComponentGameObject()
{
	for (auto &i : components) {
		if (i == nullptr)continue;
		else
		{
			delete i;
			i = nullptr;

		}
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
	components[newCom->type] = newCom;
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
void ComponentGameObject::RemoveComponetPhysics() {
	components[Component::Physics] = nullptr;
	physicsXObject= nullptr;
}
void ComponentGameObject::RemoveComponetCamera() {
	components[Component::Camera] = nullptr;
}
void ComponentGameObject::RemoveComponetInput() {
	components[Component::Input] = nullptr;
}
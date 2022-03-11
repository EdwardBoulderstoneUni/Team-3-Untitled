#pragma once
#include "Component.h"
#include "../Common/Window.h"
#include <functional>

class ComponentInput: public Component {
public:
	ComponentInput() : userInterface(NCL::Window::GetInterface())
	{ 
		type = ComponentType::Input;

		MovCallback = nullptr;
	}
	void Update(float dt) override;
	const UserInterface* userInterface;
	std::function<void()> ButtonCallback[max_input];
	std::function<void(NCL::Maths::Vector2 dir)> MovCallback;
	std::function<void(float dt)> UpdateCallback;
};
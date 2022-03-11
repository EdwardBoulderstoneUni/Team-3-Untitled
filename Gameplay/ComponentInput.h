#pragma once
#include "Component.h"
#include "../Common/UserInterface.h"
#include <functional>
class ComponentInput: public Component {
public:
	ComponentInput() { 
		type = ComponentType::Input; 
		userInterface = nullptr;
		MovCallback = nullptr;
	}
	void Init() override;
	void Update(float dt) override;
	UserInterface* userInterface;
	std::function<void()> ButtonCallback[max_input];
	std::function<void(NCL::Maths::Vector2 dir)> MovCallback;
	std::function<void(float dt)> UpdateCallback;
private:
};
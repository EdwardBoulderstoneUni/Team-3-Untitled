#pragma once
#include "Component.h"
#include "../Common/UserInterface.h"
#include <functional>
class ComponentInput: public Component {
public:
	ComponentInput() : user_interface(nullptr), mov_callback(nullptr)
	{ 
		type = Input;
	}
	void Update(float dt) override;
	const UserInterface* user_interface;
	std::function<void()> button_callback[max_input];
	std::function<void(NCL::Maths::Vector2 dir)> mov_callback;
	std::function<void(float dt)> update_callback;
};
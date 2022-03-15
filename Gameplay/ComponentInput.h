#pragma once
#include "Component.h"
#include "../Common/UserController.h"
#include <functional>
class ComponentInput: public Component {
public:
	ComponentInput(){ 
		type = Input;
	}
	void Update(float dt) override;
	UserController* user_interface;
	std::function<void(float dt)> updateCallback;
};
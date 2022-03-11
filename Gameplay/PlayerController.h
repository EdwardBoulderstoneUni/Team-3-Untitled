#pragma once
#include "../Common/UserController.h"
class PlayerController:public UserController
{public:
	Input get_inputs() override;
	void update(const float dt) override;
private:
	bool buttons[max_input] = { false };
	NCL::Maths::Vector2 look_direction = NCL::Maths::Vector2();
	NCL::Maths::Vector2 movement_direction = NCL::Maths::Vector2();
};


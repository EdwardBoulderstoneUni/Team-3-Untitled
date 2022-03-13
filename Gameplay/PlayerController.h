#pragma once
#include "../Common/UserController.h"
using namespace NCL::Maths;
class PlayerController :public UserController
{public:
	Input get_inputs() override;
	void update(const float dt) override;
private:
	bool buttons[max_input] = { false };
	Vector2 look_direction =Vector2();
	Vector2 movement_direction = Vector2();
};


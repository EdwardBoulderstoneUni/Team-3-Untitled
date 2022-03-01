#pragma once
#include "InputHandler.h"
#include "..//Common/Vector2.h"
using namespace NCL;
using namespace Maths;
struct Input;
class UserInput
{
public:
	explicit UserInput(bool is_keyboard = true);
	float get_pitch() const;
	float get_yaw() const;
	bool attack_down() const;
	bool attack_pressed() const;
	void update();

private:
	InputHandler input_handler_;
	Input user_input_;
	Input prior_user_input_;
};


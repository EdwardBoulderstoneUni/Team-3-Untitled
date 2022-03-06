#pragma once
#include "UserController.h"
#include "Vector2.h"
struct Input;
class UserInterface
{
public:
	explicit UserInterface(UserController* control_device);
	NCL::Maths::Vector2 get_movement() const;
	NCL::Maths::Vector2 get_look_direction() const;
	bool button_down(Buttons button) const;
	bool button_pressed(Buttons button) const;
	void update(float dt);

private:
	UserController* input_handler_;
	Input user_input_;
	Input prior_user_input_;
};


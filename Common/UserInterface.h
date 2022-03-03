#pragma once
#include "UserController.h"
struct Input;
class UserInterface
{
public:
	explicit UserInterface(const UserController& control_device);
	float get_pitch() const;
	float get_yaw() const;
	bool button_down(Buttons button) const;
	bool button_pressed(Buttons button) const;
	void update();

private:
	UserController input_handler_;
	Input user_input_;
	Input prior_user_input_;
};


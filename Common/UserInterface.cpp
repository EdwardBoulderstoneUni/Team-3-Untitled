#include "UserInterface.h"
#include "ControllerInput.h"
#include "MouseAndKeyboardInput.h"

UserInterface::UserInterface(const bool is_keyboard)
{
	if (is_keyboard)
		input_handler_ = static_cast<UserController>(MouseAndKeyboardInput());
	else
		input_handler_ = static_cast<UserController>(ControllerInput());
}

float UserInterface::get_pitch() const
{
	return user_input_.pitch;
}

float UserInterface::get_yaw() const
{
	return user_input_.yaw;
}

bool UserInterface::button_down(const Buttons button) const
{
	return button != max_input && user_input_.buttons[button];
}

bool UserInterface::button_pressed(const Buttons button) const
{
	return button != max_input && user_input_.buttons[button] && !prior_user_input_.buttons[button];
}

void UserInterface::update()
{
	prior_user_input_ = user_input_;
	user_input_ = input_handler_.get_inputs();
}

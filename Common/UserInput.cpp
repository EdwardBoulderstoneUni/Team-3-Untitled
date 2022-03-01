#include "UserInput.h"
#include "ControllerInput.h"
#include "MouseAndKeyboardInput.h"

UserInput::UserInput(const bool is_keyboard)
{
	if (is_keyboard)
		input_handler_ = MouseAndKeyboardInput();
	else
		input_handler_ = ControllerInput();
}

float UserInput::get_pitch() const
{
	return user_input_.pitch;
}

float UserInput::get_yaw() const
{
	return user_input_.yaw;
}

bool UserInput::attack_down() const
{
	return user_input_.attack;
}

bool UserInput::attack_pressed() const
{
	return user_input_.attack && !prior_user_input_.attack;
}

void UserInput::update()
{
	prior_user_input_ = user_input_;
	user_input_ = input_handler_.get_inputs();
}

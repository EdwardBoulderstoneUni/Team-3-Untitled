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

bool UserInput::pause_down() const
{
	return user_input_.pause;
}
bool UserInput::pause_pressed() const
{
	return user_input_.pause && !prior_user_input_.pause;
}
bool UserInput::jump_down() const
{
	return user_input_.jump;
}
bool UserInput::jump_pressed() const
{
	return user_input_.jump && !prior_user_input_.jump;
}
bool UserInput::sprint_down() const
{
	return user_input_.sprint;
}
bool UserInput::sprint_pressed() const
{
	return user_input_.sprint && !prior_user_input_.sprint;
}

void UserInput::update()
{
	prior_user_input_ = user_input_;
	user_input_ = input_handler_.get_inputs();
}

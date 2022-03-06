#include "UserInterface.h"
#include "ControllerInput.h"
UserInterface::UserInterface(UserController* control_device) : input_handler_(control_device) {}
NCL::Maths::Vector2 UserInterface::get_movement() const
{
	return user_input_.movement_direction;
}

NCL::Maths::Vector2 UserInterface::get_look_direction() const
{
	return user_input_.look_direction;
}

bool UserInterface::button_down(const Buttons button) const
{
	return button != max_input && user_input_.buttons[button];
}

bool UserInterface::button_pressed(const Buttons button) const
{
	return button != max_input && user_input_.buttons[button] && !prior_user_input_.buttons[button];
}

void UserInterface::update(const float dt)
{
	prior_user_input_ = user_input_;
	user_input_ = input_handler_->get_inputs();
	input_handler_->update(dt);
}

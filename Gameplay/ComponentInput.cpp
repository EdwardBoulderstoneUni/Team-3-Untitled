#include "ComponentInput.h"

void ComponentInput::Update(const float dt) {
	if (user_interface->button_down(jump))
		button_callback[jump]();
	if (user_interface->button_down(dash))
		button_callback[dash]();
	if (user_interface->button_down(reload))
		button_callback[reload]();
	if (user_interface->get_movement() != NCL::Maths::Vector2(0,0))
		mov_callback(user_interface->get_movement());
	if (user_interface->button_down(attack))
		button_callback[attack]();
	update_callback(dt);
}

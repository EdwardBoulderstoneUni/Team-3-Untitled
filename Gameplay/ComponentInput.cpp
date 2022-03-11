#include "ComponentInput.h"
void ComponentInput::Update(float dt) {
	if (userInterface->button_down(jump))
		ButtonCallback[jump]();
	if (userInterface->button_down(dash))
		ButtonCallback[dash]();
	if (userInterface->button_down(reload))
		ButtonCallback[reload]();
	if (userInterface->get_movement() != NCL::Maths::Vector2(0,0))
		MovCallback(userInterface->get_movement());
	if (userInterface->button_down(attack))
		ButtonCallback[attack]();
	UpdateCallback(dt);
	userInterface->update(dt);
}

#include "ComponentInput.h"
#include "Player.h"
void ComponentInput::Init() {
}

void ComponentInput::Update(float dt) {
	if (userInterface->button_down(jump))
		Callback[jump]();
	if (userInterface->button_down(dash))
		Callback[dash]();
	if (userInterface->button_down(reload))
		Callback[reload]();
	if (userInterface->get_movement() != Vector2(0,0))
		MovCallback(userInterface->get_movement());
	if (userInterface->button_down(attack))
		Callback[attack]();
	Callback[idle]();
	userInterface->update(dt);
}

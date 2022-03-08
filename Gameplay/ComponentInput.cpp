#include "ComponentInput.h"
#include "Player.h"
void ComponentInput::Init() {
}

void ComponentInput::Update(float dt) {
	if (userInterface->button_down(jump))
		Callback[jump]();
	if (userInterface->button_down(dash))
		Callback[dash]();

	if (userInterface->get_movement() != Vector2(0,0))
		Callback[move]();

	userInterface->update(dt);


}

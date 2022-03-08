#include "ComponentInput.h"
#include "Player.h"
void ComponentInput::Init() {
	
}

void ComponentInput::Update(float dt) {
	if (userInterface->button_down(jump))
		Callback[jump]();
	if (userInterface->button_down(dash))
		Callback[dash]();
	userInterface->update(dt);
}

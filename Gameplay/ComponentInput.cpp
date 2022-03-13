#include "ComponentInput.h"

void ComponentInput::Update(const float dt) {
	user_interface->update(dt);
	updateCallback(dt);
}

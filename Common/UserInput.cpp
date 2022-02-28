#include "UserInput.h"

Vector2 UserInput::getMoveAxis()
{
	return userInput.moveAxis;
}

Vector2 UserInput::getLookAxis()
{
	return userInput.lookAxis;
}

bool UserInput::attack_pressed()
{
	return userInput.attack;
}

void UserInput::getInput()
{
	userInput = inputHandler.getInputs();
}

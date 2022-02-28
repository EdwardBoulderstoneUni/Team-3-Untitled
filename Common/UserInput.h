#pragma once
#include "..//Common/Vector2.h"
using namespace NCL;
using namespace Maths;
class UserInput
{
public:
	UserInput(InputHandler inputHandler);
	struct Input {
		bool attack = false;
		Vector2 moveAxis = Vector2();
		Vector2 lookAxis = Vector2();
	};
	Vector2 getMoveAxis();
	Vector2 getLookAxis();
	bool attack_pressed();

private:
	void getInput();
	InputHandler inputHandler;
	Input userInput;
};


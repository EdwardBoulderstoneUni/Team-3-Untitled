#include "Keyboard.h"
#include <string>

using namespace NCL;

Keyboard::Keyboard()
{
	//Initialise the arrays to false!
	memset(keyStates, 0, static_cast<int>(KeyboardKeys::MAXVALUE) * sizeof(bool));
	memset(holdStates, 0, static_cast<int>(KeyboardKeys::MAXVALUE) * sizeof(bool));
}

/*
Updates variables controlling whether a keyboard key has been
held for multiple frames.
*/
void Keyboard::UpdateFrameState(float msec)
{
	memcpy(holdStates, keyStates, static_cast<int>(KeyboardKeys::MAXVALUE) * sizeof(bool));
}

void Keyboard::Sleep()
{
	isAwake = false;
	memset(keyStates, 0, static_cast<int>(KeyboardKeys::MAXVALUE) * sizeof(bool));
	memset(holdStates, 0, static_cast<int>(KeyboardKeys::MAXVALUE) * sizeof(bool));
}

void Keyboard::Wake()
{
	isAwake = true;
}

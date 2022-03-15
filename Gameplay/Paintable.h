#pragma once
#include "Component.h"

enum Colour{};
class Paintable : Component
{

public:
	Paintable();

	Colour getColour();
protected:
	Colour colour_;
};


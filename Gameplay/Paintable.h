#pragma once
#include "Component.h"

enum Colour{};
class Paintable : Component
{

public:
	Paintable();
	void paint();
	Colour getColour();
protected:
	Colour colour_;
};


#pragma once
#include "Component.h"

enum Colour{};
class PaintableSurface : Component
{

public:
	PaintableSurface();

	Colour getColour();
protected:
	Colour colour_;

};


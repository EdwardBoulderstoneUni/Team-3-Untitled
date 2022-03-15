#pragma once

enum Colour{};
class Paintable
{

public:
	Paintable();

	Colour getColour();
protected:
	Colour colour_;
};


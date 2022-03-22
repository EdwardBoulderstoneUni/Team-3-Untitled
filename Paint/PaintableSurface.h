#pragma once
#include "../Common/Vector3.h"
#include "../Gameplay/Component.h"
class PaintableRenderObject;

enum Colour {};
class PaintableSurface : Component
{

public:
	PaintableSurface();

	Colour GetColour();
	PaintableRenderObject* GetRenderObject();
protected:
	NCL::Maths::Vector3 colour_;
	PaintableRenderObject* render_object_;

};


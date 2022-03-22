#pragma once
#include "PaintableSurface.h"


namespace NCL
{
	namespace Rendering {
		class ShaderBase;
	}
}

class PaintManager
{
public:
	PaintManager* init();
	void paint(PaintableSurface surface, NCL::Maths::Vector3 position, float radius = 1.0f, float hardness = 0.5f, float strength = 0.5f, NCL::Maths::Vector3 colour = NCL::Maths::Vector3(1, 0, 0));
	static NCL::Rendering::ShaderBase* GetPaintableObjectShader();
protected:
	static PaintManager* instance_;
	PaintManager();
	NCL::Rendering::ShaderBase* paintable_object_shader_;
	NCL::Rendering::ShaderBase* paint_instance_shader_;
};



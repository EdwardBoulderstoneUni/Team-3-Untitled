#pragma once
#include "PaintableRenderObject.h"
#include "PaintableSurface.h"
#include "../Common/ShaderBase.h"
#include "../Common/ShaderManager.h"
#include "../Common/Window.h"

class PaintManager
{
public:
	static PaintManager* init();
	static void paint(PaintableSurface surface, NCL::Maths::Vector3 position, float radius = 1.0f, float hardness = 0.5f, float strength = 0.5f, NCL::Maths::Vector3 colour = NCL::Maths::Vector3(1, 0, 0));
	static NCL::Rendering::ShaderBase* GetPaintableObjectShader();
protected:
	static PaintManager* instance_;
	PaintManager();
	NCL::Rendering::ShaderBase* paintable_object_shader_;
	NCL::Rendering::ShaderBase* paint_instance_shader_;
};



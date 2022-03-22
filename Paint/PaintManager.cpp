#include "PaintManager.h"
#include "../Common/ShaderBase.h"

PaintManager* PaintManager::init()
{
	if (!instance_)
		instance_ = new PaintManager();

	return instance_;
}

void PaintManager::paint(PaintableSurface surface, NCL::Maths::Vector3 position, float radius, float hardness,
	float strength, NCL::Maths::Vector3 colour)
{
	
}

NCL::Rendering::ShaderBase* PaintManager::GetPaintableObjectShader()
{
	return instance_->paintable_object_shader_;
}

PaintManager::PaintManager()
{
	paintable_object_shader_ = ShaderManager::GetPaintableObjectShader();
	paint_instance_shader_ = ShaderManager::GetPaintInstanceShader();
	if (!instance_)
		instance_ = this;
}

#include "PaintManager.h"

PaintManager* PaintManager::init()
{
	if (!instance_)
		instance_ = new PaintManager();

	return instance_;
}

void PaintManager::paint(PaintableSurface surface, NCL::Maths::Vector3 position, float radius, float hardness,
	float strength, NCL::Maths::Vector3 colour)
{
	const auto paint_render_object = surface.GetRenderObject();
	const auto renderer = NCL::Window::GetRenderer();

	auto paint_dest = paint_render_object->get_paint_dest();
	auto mask = paint_render_object->get_mask();
	renderer->bind_shader(instance_->paint_instance_shader_);

	renderer->bind_shader_property("mainTex", mask);
	renderer->bind_shader_property("paintCenter;", position);
	renderer->bind_shader_property("radius", radius);
	renderer->bind_shader_property("hardness", hardness);
	renderer->bind_shader_property("strength", strength);
	renderer->bind_shader_property("paintColour", NCL::Vector4(colour, 1));

	renderer->render_to(paint_dest);
	renderer->blit(paint_dest, mask);
}

NCL::Rendering::ShaderBase* PaintManager::GetPaintableObjectShader()
{
	return instance_->paintable_object_shader_;
}

PaintManager::PaintManager() :
	paintable_object_shader_(NCL::Rendering::ShaderManager::GetInstance()->GetShader("PaintableObjectShader")),
	paint_instance_shader_(NCL::Rendering::ShaderManager::GetInstance()->GetShader("PaintInstanceShader"))
{
	if (!instance_)
		instance_ = this;
}

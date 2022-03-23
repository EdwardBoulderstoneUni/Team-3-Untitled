#include "PaintManager.h"
#include "../Common/ShaderManager.h"
PaintManager* PaintManager::instance_ = nullptr;
PaintManager* PaintManager::init()
{
	if (!instance_)
		instance_ = new PaintManager();

	return instance_;
}

void PaintManager::paint(const PaintableSurface* surface, const NCL::Maths::Vector3& position, const float radius, const float hardness,
                         const float strength, const NCL::Maths::Vector3& colour)
{
	const auto paint_render_object = surface->GetRenderObject();
	const auto renderer = NCL::Window::GetRenderer();

	const auto paint_dest = paint_render_object->get_paint_dest();
	const auto mask = paint_render_object->get_mask();
	if (!instance_)
		init();
	renderer->bind_shader(instance_->paint_instance_shader_);
	paint_render_object->bind_mesh(renderer);
	renderer->bind_shader_property("mainTex", *mask);
	renderer->bind_shader_property("paintCenter;", position);
	renderer->bind_shader_property("radius", radius);
	renderer->bind_shader_property("hardness", hardness);
	renderer->bind_shader_property("strength", strength);
	renderer->bind_shader_property("paintColour", NCL::Vector4(colour, 1));

	renderer->render_to(paint_dest);
	renderer->blit(paint_dest, mask);
}


PaintManager::PaintManager() :
	paint_instance_shader_(NCL::Rendering::ShaderManager::GetInstance()->GetShader("PaintInstanceShader"))
{
	if (!instance_)
		instance_ = this;
}

#include "PaintableRenderObject.h"
#include "../Common/ShaderManager.h"
#include "../CSC8503/CSC8503Common/RenderObject.h"
PaintableRenderObject::PaintableRenderObject(NCL::CSC8503::Transform* parent_transform, NCL::MeshGeometry* mesh,
                                             NCL::TextureBase* tex, NCL::MeshMaterial* mat) : RenderObject(parent_transform, mesh, tex, NCL::Rendering::ShaderManager::GetInstance()->GetShader("PaintableObjectShader"), mat),
                                                                                              paint_instance_texture_(NCL::Window::GetRenderer()->init_blank_texture(tex->get_width(), tex->get_height())),
                                                                                              mask_(NCL::Window::GetRenderer()->init_blank_texture(tex->get_width(), tex->get_height())) { }

void PaintableRenderObject::bind_shader_values(NCL::RendererBase* renderer) const
{
	RenderObject::bind_shader_values(renderer);
	renderer->bind_shader_property("mask", *mask_);
}

NCL::TextureBase* PaintableRenderObject::get_paint_dest() const
{
	return paint_instance_texture_;
}

NCL::TextureBase* PaintableRenderObject::get_mask() const
{
	return mask_;
}

PaintableRenderObject::~PaintableRenderObject()
{
	delete paint_instance_texture_;
	delete mask_;
}
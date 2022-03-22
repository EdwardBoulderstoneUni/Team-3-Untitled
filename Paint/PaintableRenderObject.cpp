#include "PaintableRenderObject.h"
PaintableRenderObject::PaintableRenderObject(NCL::CSC8503::Transform* parent_transform, NCL::MeshGeometry* mesh,
                                             NCL::TextureBase* tex, NCL::MeshMaterial* mat) : RenderObject(parent_transform, mesh, tex, PaintManager::GetPaintableObjectShader(), mat),
	paint_instance_texture_(NCL::Window::GetRenderer()->init_blank_texture(tex->get_width(), tex->get_height())),
	mask_(NCL::Window::GetRenderer()->init_blank_texture(tex->get_width(), tex->get_height())) { }

PaintableRenderObject::~PaintableRenderObject()
{
	delete paint_instance_texture_;
	delete mask_;
}

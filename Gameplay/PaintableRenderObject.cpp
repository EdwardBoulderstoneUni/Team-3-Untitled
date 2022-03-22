#include "PaintableRenderObject.h"
#include "../Common/RendererBase.h"
using namespace NCL;
using namespace CSC8503;
PaintableRenderObject::PaintableRenderObject(Transform* parent_transform, MeshGeometry* mesh, TextureBase* tex, ShaderBase* shader, MeshMaterial* mat) :
RenderObject(parent_transform, mesh, tex, shader, mat),
mask_(NCL::Window::GetRenderer()->init_blank_texture(tex->get_width(), tex->get_height())), base_(NCL::Window::GetRenderer()->init_blank_texture(tex->get_width(), tex->get_height()))
{
	 
}

PaintableRenderObject::~PaintableRenderObject()
{
	delete mask_;
	delete base_;
}

#include "RenderObject.h"

#include "Transform.h"
#include "../../Common/MeshGeometry.h"
#include "../../OpenGLRendering/OGLRenderer.h"

using namespace NCL::CSC8503;

RenderObject::RenderObject(Transform* parent_transform, MeshGeometry* mesh, TextureBase* tex, ShaderBase* shader, MeshMaterial* mat) :
	texture_(tex), mesh_(mesh), shader_(shader), transform_(parent_transform), material_(mat)
{
	this->colour_	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
}

void RenderObject::bind_shader_values(RendererBase* renderer) const
{
	renderer->bind_shader(shader_);
	renderer->bind_shader_property("mainTex", texture_);
	renderer->bind_shader_property("modelMatrix", transform_->GetMatrix());
	renderer->bind_shader_property("objectColour", colour_);
	renderer->bind_shader_property("hasVertexColours", !mesh_->GetColourData().empty());
	renderer->bind_shader_property("hasTexture", static_cast<bool>(texture_));
}
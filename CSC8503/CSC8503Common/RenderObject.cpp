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

void RenderObject::render(RendererBase* renderer) const
{
	const auto shader_changed = renderer->BindShader(shader_);
	if (shader_changed) 
		renderer->bind_shader_defaults();
	bind_shader_values(renderer);
}

void RenderObject::bind_shader_values(RendererBase* renderer) const
{
	renderer->BindTextureToShader(texture_, "mainTex", 0);
	renderer->bind_shader_property("modelMatrix", transform_->GetMatrix());
	renderer->bind_shader_property("objectColour", colour_);
	renderer->bind_shader_property("hasVertexColours", !mesh_->GetColourData().empty());
	renderer->bind_shader_property("hasTexture", static_cast<bool>(texture_));
}

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
	renderer->bind_shader_property("modelMatrix", transform_->GetMatrix() * mesh_->GetLocalTransform());
	renderer->bind_shader_property("objectColour", colour_);
	renderer->bind_shader_property("hasVertexColours", !mesh_->GetColourData().empty());
	renderer->bind_mesh(mesh_);
	if (!material_)
	{
		renderer->bind_shader_property("mainTex", *texture_);
		renderer->bind_shader_property("hasTexture", texture_ ? 1 : 0);
	}
}

void RenderObject::render(RendererBase* renderer) const
{
	bind_shader_values(renderer);
	for (unsigned count = 0; count < mesh_->GetSubMeshCount(); ++count) {

		if (material_)
		{
			TextureBase* texture = material_->GetMaterialForLayer(count)->GetEntry("Diffuse");
			renderer->bind_shader_property("mainTex", *texture);
			renderer->bind_shader_property("hasTexture", texture ? 1 : 0);
		}
		renderer->draw_bound_mesh(count);
	}
}

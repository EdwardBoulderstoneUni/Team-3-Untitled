#pragma once
#include "../CSC8503/CSC8503Common/RenderObject.h"
#include "../Common/Window.h"
class PaintableRenderObject :
	public NCL::CSC8503::RenderObject
{
public:
	PaintableRenderObject(NCL::CSC8503::Transform* parent_transform, NCL::MeshGeometry* mesh, NCL::TextureBase* tex, NCL::MeshMaterial* mat = nullptr);
	~PaintableRenderObject();
	void bind_shader_values(NCL::RendererBase* renderer) const override;
	NCL::Vector4 get_colour_at(const NCL::Maths::Vector3& world_pos);
	NCL::TextureBase* get_paint_dest() const;
	NCL::TextureBase* get_mask() const;
protected:
	NCL::TextureBase* paint_instance_texture_;
	NCL::TextureBase* mask_;
};


#pragma once
#include "PaintManager.h"
#include "../Common/RendererBase.h"
#include "../Common/Window.h"
#include "../CSC8503/CSC8503Common/RenderObject.h"
class PaintableRenderObject :
	public NCL::CSC8503::RenderObject
{
public:
	PaintableRenderObject(NCL::CSC8503::Transform* parent_transform, NCL::MeshGeometry* mesh, NCL::TextureBase* tex, NCL::MeshMaterial* mat = nullptr);
	~PaintableRenderObject();
	NCL::TextureBase* get_paint_dest() const;
	NCL::TextureBase* get_mask() const;
protected:
	NCL::TextureBase* paint_instance_texture_;
	NCL::TextureBase* mask_;
};


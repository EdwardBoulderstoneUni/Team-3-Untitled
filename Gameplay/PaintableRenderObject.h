#pragma once
#include "../../CSC8503/CSC8503Common/RenderObject.h"
#include "../Common/Window.h"
class PaintableRenderObject :
    public NCL::CSC8503::RenderObject
{
public:
	friend class NCL::Window;
	PaintableRenderObject(NCL::CSC8503::Transform* parent_transform, NCL::MeshGeometry* mesh, NCL::TextureBase* tex, NCL::ShaderBase* shader, NCL::MeshMaterial* mat = nullptr);
	~PaintableRenderObject();
protected:
	NCL::TextureBase* mask_;
	NCL::TextureBase* base_;
};


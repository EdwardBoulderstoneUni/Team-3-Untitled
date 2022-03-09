#include "RenderObject.h"
#include "../../Common/MeshGeometry.h"

using namespace NCL::CSC8503;
using namespace NCL;

RenderObject::RenderObject(Transform* parentTransform, MeshGeometry* mesh, TextureBase* tex, ShaderBase* shader, MeshMaterial *mat) {
	this->transform	= parentTransform;
	this->mesh		= mesh;
	this->texture	= tex;
	this->shader	= shader;
	this->colour	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	this->material = mat;
	this->localTransform = Matrix4();
}

RenderObject::~RenderObject()
{
}

void NCL::CSC8503::RenderObject::SetLocalTransform(const Maths::Matrix4& mat)
{
	localTransform = mat;
}

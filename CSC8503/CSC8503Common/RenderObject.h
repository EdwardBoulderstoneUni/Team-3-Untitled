#pragma once
#include "../../Common/MeshMaterial.h"
#include "../../Common/ShaderBase.h"
#include "../../Common/TextureBase.h"
#include "../../Common/Vector4.h"

namespace NCL
{
	namespace Rendering {
		class RendererBase;
	}

	using namespace Rendering;

	class MeshGeometry;

	namespace CSC8503
	{
		class Transform;
		using namespace Maths;

		class RenderObject
		{
		public:
			RenderObject(Transform* parent_transform, MeshGeometry* mesh, TextureBase* tex, ShaderBase* shader, MeshMaterial* mat = nullptr);
			
			void bind_shader_values(RendererBase* renderer) const;

			void SetDefaultTexture(TextureBase* t)
			{
				texture_ = t;
			}

			TextureBase* GetDefaultTexture() const
			{
				return texture_;
			}

			MeshGeometry* GetMesh() const
			{
				return mesh_;
			}

			Transform* GetTransform() const
			{
				return transform_;
			}

			ShaderBase* GetShader() const
			{
				return shader_;
			}

			MeshMaterial* GetMaterial() const {
				return material_;
			}
			Vector4 colour_;
			TextureBase* texture_;
		protected:
			MeshGeometry*	mesh_;
			ShaderBase*		shader_;
			Transform*		transform_;
			MeshMaterial*	material_;
		};
	}
}

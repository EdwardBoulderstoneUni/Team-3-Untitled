#pragma once
#include "../../Common/Matrix4.h"
#include "../../Common/TextureBase.h"
#include "../../Common/ShaderBase.h"
#include "../../Common/Vector4.h"
#include "../../Common/MeshMaterial.h"

namespace NCL
{
	using namespace Rendering;

	class MeshGeometry;

	namespace CSC8503
	{
		class Transform;
		using namespace Maths;

		class RenderObject
		{
		public:
			RenderObject(Transform* parentTransform, MeshGeometry* mesh, TextureBase* tex, ShaderBase* shader, MeshMaterial* mat = nullptr);
			~RenderObject();

			void SetDefaultTexture(TextureBase* t)
			{
				texture = t;
			}

			TextureBase* GetDefaultTexture() const
			{
				return texture;
			}

			MeshGeometry* GetMesh() const
			{
				return mesh;
			}

			Transform* GetTransform() const
			{
				return transform;
			}

			ShaderBase* GetShader() const
			{
				return shader;
			}

			void SetColour(const Vector4& c)
			{
				colour = c;
			}

			Vector4 GetColour() const
			{
				return colour;
			}

			MeshMaterial* GetMaterial() const {
				return material;
			}

		protected:
			MeshGeometry*	mesh;
			TextureBase*	texture;
			ShaderBase*		shader;
			Transform*		transform;
			Vector4			colour;
			MeshMaterial*	material;
		};
	}
}

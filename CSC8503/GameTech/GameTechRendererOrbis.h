#pragma once
#include "./../../GNMRendering/PS4Renderer.h"

namespace NCL {
	namespace CSC8503
	{
		class RenderObject;
	}
	namespace PS4 {
		class GameTechRendererOrbis :
			public PS4RendererBase
		{
		public:
			GameTechRendererOrbis();
			~GameTechRendererOrbis();

			void Update(float dt)	 override;

		protected:
			void	RenderFrame()	override;

			void DrawRenderObject(NCL::CSC8503::RenderObject* o);

			float rotation;
			float translation;
			NCL::CSC8503::RenderObject* defaultObject[2];

			//default data
			PS4Shader* defaultShader;
			PS4Mesh* defaultMesh;
			PS4Texture* defaultTexture;

			//		
			NCL::Maths::Matrix4* viewProjMat;
			Gnm::Buffer	cameraBuffer;
		};
	}
}


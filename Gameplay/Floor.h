#pragma once
#include "ComponentGameObject.h"
#include "../CSC8503/CSC8503Common/AssetManager.h"
#include "../Paint/PaintableRenderObject.h"
#include "../Paint/PaintableSurface.h"
#include "../Plugins/OpenGLRendering/OGLMesh.h"
#include "../Plugins/OpenGLRendering/OGLTexture.h"
namespace NCL {
	namespace CSC8503 {
		class Floor : public ComponentGameObject
		{
		public:
			Floor();
			~Floor();
			void SetUp() override;
		};
	}
}



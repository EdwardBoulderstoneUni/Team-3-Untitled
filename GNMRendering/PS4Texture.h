#pragma once

#include "../Common/TextureBase.h"
#include <string>
#include <gnm\texture.h>
#include "OrbisAllocator.h"
namespace NCL {
	namespace PS4 {
		class PS4Texture :
			public Rendering::TextureBase, public OrbisAllocator
		{
		public:
			friend class PS4RendererBase;

			~PS4Texture();

			static PS4Texture* LoadTextureFromFile(const std::string& filename);

			const sce::Gnm::Texture& GetAPITexture() {
				return apiTexture;
			}

		protected:
			PS4Texture();
			sce::Gnm::Texture apiTexture;
		};
	}
}

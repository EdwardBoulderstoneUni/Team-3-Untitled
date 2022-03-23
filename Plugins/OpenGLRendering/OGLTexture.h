/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#pragma once
#include "../../Common/TextureBase.h"
#include "glad/glad.h"

#include <string>

namespace NCL
{
	namespace Rendering
	{
		class OGLTexture : public TextureBase
		{
		public:
			//friend class OGLRenderer;
			OGLTexture();
			OGLTexture(unsigned width, unsigned height, unsigned channels = 4, GLuint* data = nullptr);
			OGLTexture(unsigned width, unsigned height, GLint internal_format, GLint pixel_format, GLint pixel_type, GLuint* data = nullptr);
			static OGLTexture* init_shadow_texture(unsigned shadow_size);
			explicit OGLTexture(GLuint tex_to_own);
			~OGLTexture() override;


			static TextureBase* RGBATextureFromData(const char* data, int width, int height, int channels);

			static TextureBase* RGBATextureFromFilename(const std::string& name);

			static TextureBase* RGBATextureFromCompressedData(char* data, int dataLength);

			GLuint GetObjectID() const
			{
				return tex_id_;
			}

		protected:

			void init_texture(GLint internal_format, GLint pixel_format, GLint pixel_type, const GLuint* data) const;
			GLuint tex_id_;
		};
	}
}

/*
Part of Newcastle University's Game Engineering source code.

Use as you see fit!

Comments and queries to: richard-gordon.davison AT ncl.ac.uk
https://research.ncl.ac.uk/game/
*/
#include "OGLTexture.h"

#include <cassert>

#include "OGLRenderer.h"

#include "../../Common/TextureLoader.h"

using namespace NCL;
using namespace Rendering;

OGLTexture::OGLTexture()
{
	glGenTextures(1, &tex_id_);
	width_ = 0;
	height_ = 0;
}

OGLTexture::OGLTexture(const unsigned width, const unsigned height, const unsigned channels, GLuint* data) : OGLTexture()
{
	assert(channels > 0);
	assert(channels < 5);
	width_ = width;
	height_ = height;
	constexpr auto pixel_type = GL_UNSIGNED_BYTE;
	GLint internal_format;
	int pixel_format;
	switch (channels)
	{
	case(1):
		internal_format = GL_R32F;
		pixel_format = GL_R;
		break;
	case(2):
		internal_format = GL_RG32F;
		pixel_format = GL_RG;
		break;
	case(3):
		internal_format = GL_RGB32F;
		pixel_format = GL_RGB;
		break;
	case(4):
		internal_format = GL_RGBA32F;
		pixel_format = GL_RGBA;
		break;
	default:
		internal_format = 0;
		pixel_format = 0;
		break;
	}
	
	const auto data_size = static_cast<unsigned long long>(width_) * height_ * channels;
	if (!data) {
		data = new GLuint[data_size];
		for (unsigned long long i = 3; i < data_size; i++)
		{
			data[i] = 0;
		}
	}
	init_texture(internal_format, pixel_format, pixel_type, data);

	
}

OGLTexture::OGLTexture(const unsigned width, const unsigned height, const GLint internal_format,
                       const GLint pixel_format, const GLint pixel_type, GLuint* data) : OGLTexture()
{
	width_ = width;
	height_ = height;
	init_texture(internal_format, pixel_format, pixel_type, data);
}

OGLTexture::OGLTexture(const GLuint tex_to_own) : tex_id_(tex_to_own)
{
	width_ = 0;
	height_ = 0;
}

OGLTexture::~OGLTexture()
{
	glDeleteTextures(1, &tex_id_);
}

TextureBase* OGLTexture::RGBATextureFromData(const char* data, const int width, const int height, const int channels)
{
	auto tex = new OGLTexture();
	tex->width_ = width;
	tex->height_ = height;
	int dataSize = width * height * channels; //This always assumes data is 1 byte per channel

	int sourceType = GL_RGB;

	switch (channels)
	{
	case 1: sourceType = GL_RED;
		break;

	case 2: sourceType = GL_RG;
		break;
	case 3: sourceType = GL_RGB;
		break;
	case 4: sourceType = GL_RGBA;
		break;
		//default:
	}

	glBindTexture(GL_TEXTURE_2D, tex->tex_id_);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, sourceType, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}

TextureBase* OGLTexture::RGBATextureFromFilename(const std::string& name)
{
	char* texData = nullptr;
	int width = 0;
	int height = 0;
	int channels = 0;
	int flags = 0;
	TextureLoader::LoadTexture(name, texData, width, height, channels, flags);

	TextureBase* glTex = RGBATextureFromData(texData, width, height, channels);

	free(texData);

	return glTex;
}

TextureBase* OGLTexture::RGBATextureFromCompressedData(char* data, int dataLength)
{
	char* texData = nullptr;
	int width = 0;
	int height = 0;
	int channels = 0;
	int flags = 0;
	TextureLoader::LoadTextureData(data, dataLength, texData, &width, &height, &channels);

	TextureBase* glTex = RGBATextureFromData(texData, width, height, channels);

	free(texData);

	return glTex;
}

void OGLTexture::init_texture(const GLint internal_format, const GLint pixel_format, const GLint pixel_type, const GLuint* data) const
{
	glBindTexture(GL_TEXTURE_2D, tex_id_);

	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, static_cast<int>(width_), static_cast<int>(height_), 0, pixel_format, pixel_type, data);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

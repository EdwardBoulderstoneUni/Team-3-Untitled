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
	glGenTextures(1, &texID);
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
	int source_type;
	switch (channels)
	{
	case(1):
		source_type = GL_R;
		break;
	case(2):
		source_type = GL_RG;
		break;
	case(3):
		source_type = GL_RGB;
		break;
	case(4):
		source_type = GL_RGBA;
		break;
	default: 
		source_type = 0;
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

	glBindTexture(GL_TEXTURE_2D, texID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, static_cast<int>(width_), static_cast<int>(height_), 0, source_type, pixel_type, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	
}

OGLTexture::OGLTexture(GLuint texToOwn)
{
	texID = texToOwn;
	width_ = 0;
	height_ = 0;
}

OGLTexture::~OGLTexture()
{
	glDeleteTextures(1, &texID);
}

TextureBase* OGLTexture::RGBATextureFromData(char* data, int width, int height, int channels)
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

	glBindTexture(GL_TEXTURE_2D, tex->texID);

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

TextureBase* NCL::Rendering::OGLTexture::RGBATextureFromCompressedData(char* data, int dataLength)
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

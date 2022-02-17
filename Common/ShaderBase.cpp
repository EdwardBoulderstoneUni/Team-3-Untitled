#include "ShaderBase.h"
using namespace NCL;
using namespace Rendering;

ShaderBase::ShaderBase(const string& vertex, const string& fragment, const string& geometry, const string& domain,
                       const string& hull)
{
	shaderFiles[static_cast<int>(ShaderStages::SHADER_VERTEX)] = vertex;
	shaderFiles[static_cast<int>(ShaderStages::SHADER_FRAGMENT)] = fragment;
	shaderFiles[static_cast<int>(ShaderStages::SHADER_GEOMETRY)] = geometry;
	shaderFiles[static_cast<int>(ShaderStages::SHADER_DOMAIN)] = domain;
	shaderFiles[static_cast<int>(ShaderStages::SHADER_HULL)] = hull;
}

ShaderBase::~ShaderBase()
{
}

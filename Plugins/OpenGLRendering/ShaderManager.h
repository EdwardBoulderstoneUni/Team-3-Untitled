#pragma once
#include <map>
#include "OGLShader.h"
namespace NCL
{
	namespace Rendering
	{
		class OGLShader;
		class ShaderManager
		{
			static ShaderManager* m_Instance;
			std::map<const char*, OGLShader*> m_Shaders;
			ShaderManager();
			~ShaderManager();
			
		public:
			static ShaderManager* GetInstance();
			static void Init();
			static void Finish();			
			OGLShader* GetShader(const char* name);			
		};
	}
};
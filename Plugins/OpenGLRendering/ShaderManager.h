#pragma once
#include <map>
#include "../../Common/ShaderBase.h"
namespace NCL
{
	namespace Rendering
	{
		class ShaderManager
		{
			static ShaderManager* m_Instance;
			std::map<const char*, ShaderBase*> m_Shaders;
			ShaderManager();
			~ShaderManager();
			
		public:
			static ShaderManager* GetInstance();
			static void Init();
			static void Finish();			
			ShaderBase* GetShader(const char* name) const;
			void AddShader(const char* name, ShaderBase* shader);
		};
	}
};
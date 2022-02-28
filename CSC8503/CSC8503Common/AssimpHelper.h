#pragma once

namespace Assimp
{
	class Importer;
}

namespace NCL
{
	namespace Rendering
	{
		class OGLMesh;
	}
	class AssimpHelper
	{
		Assimp::Importer* m_Importer;
		static AssimpHelper* m_Instance;
		AssimpHelper();
		~AssimpHelper();
	public:
		static void Init();
		static void Clear();
		static AssimpHelper &GetInstance() { return *m_Instance; }
		Rendering::OGLMesh *ProcessFBX(const char* filePath);
	};
}
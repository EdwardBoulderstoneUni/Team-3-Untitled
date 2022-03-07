#pragma once

struct aiMesh;
struct aiScene;
struct aiTexture;

namespace Assimp
{
	class Importer;
}

namespace NCL
{
	class MeshMaterial;
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
		void ProcessFBX(const char* filePath, Rendering::OGLMesh *&outMesh, MeshMaterial *&outMaterial);
		void TryLoadMaterial(const aiScene* scene, const aiMesh* mesh, MeshMaterial *&outMaterial) const;
		void LoadTextureFromFile(const aiTexture* texture, const char*fileName, MeshMaterial*& material) const;
		void LoadTextureFromMemory(const aiTexture* texture, const char* fileName, MeshMaterial*& material) const;
	};
}
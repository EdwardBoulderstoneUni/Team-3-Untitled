#pragma once
#include <map>
namespace NCL
{
	namespace Rendering {
		class OGLMesh;
		class OGLTexture;
	}
	class AssetManager
	{
		std::map<const char*, NCL::Rendering::OGLMesh*> m_Meshes;
		std::map<const char*, NCL::Rendering::OGLTexture*> m_Textures;

		static AssetManager* m_Instance;
		AssetManager();
		~AssetManager();
		void LoadMeshes();
		void LoadTextures();
	public:
		static AssetManager* GetInstance();
		static void Init();
		static void Finish();
		NCL::Rendering::OGLMesh* GetMesh(const char*);
		NCL::Rendering::OGLTexture* GetTexture(const char*);
	};
}


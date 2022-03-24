#pragma once
#include <map>
#include <string>

namespace NCL
{
	class MeshMaterial;
	namespace Rendering {
		class OGLMesh;
		class OGLTexture;
	};
	class AssetManager
	{

		std::map<std::string, NCL::Rendering::OGLMesh*> m_Meshes;
		std::map<const char*, NCL::Rendering::OGLTexture*> m_Textures;
		std::map<std::string, NCL::MeshMaterial*> m_Materials;

		static AssetManager* m_Instance; 
		AssetManager();
		~AssetManager();
		void LoadMeshData();
		void LoadTextures();
		void LoadMaterials();
	public:
		static AssetManager* GetInstance();
		static void Init();
		static void Finish();
		NCL::Rendering::OGLMesh* GetMesh(const char*) const;
		NCL::Rendering::OGLTexture* GetTexture(const char*) const;
		NCL::MeshMaterial* GetMaterial(const char*) const;
		void UploadToGPU();
	};
}


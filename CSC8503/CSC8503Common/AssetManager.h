#pragma once
#include <map>
#include <string>

namespace NCL
{
	class MeshGeometry;
	class MeshMaterial;
	namespace Rendering {
		class TextureBase;
	}
	class AssetManager
	{

		std::map<std::string, NCL::MeshGeometry*> m_Meshes;
		std::map<const char*, NCL::Rendering::TextureBase*> m_Textures;
		std::map<std::string, NCL::MeshMaterial*> m_Materials;

		static AssetManager* m_Instance;
		AssetManager();
		~AssetManager();
		void LoadMeshes();
		void LoadTextures();
		void LoadMaterials();
	public:
		static AssetManager* GetInstance();
		static void Init();
		static void Finish();
		NCL::MeshGeometry* GetMesh(const char*) const;
		NCL::Rendering::TextureBase* GetTexture(const char*) const;
		NCL::MeshMaterial* GetMaterial(const char*) const;
	};
}


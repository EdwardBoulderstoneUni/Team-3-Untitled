#include "AssetManager.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Common/MeshGeometry.h"
#include "../../Common/TextureLoader.h"
#include <string>

namespace NCL
{
	AssetManager* AssetManager::m_Instance = nullptr;

	AssetManager::AssetManager()
	{
		LoadMeshes();
		LoadTextures();
	}
	void AssetManager::LoadMeshes()
	{
		auto loadFunc = [](const std::string& name) {
			NCL::Rendering::OGLMesh *into = new NCL::Rendering::OGLMesh(name);
			(into)->SetPrimitiveType(NCL::GeometryPrimitive::Triangles);
			(into)->UploadToGPU();
			return into;
		};
//To-Do: read file names from a text file/load all msh from assets
		NCL::Rendering::OGLMesh *mesh = loadFunc("cube.msh");
		m_Meshes.insert({ "cube", mesh });

		mesh = loadFunc("sphere.msh");
		m_Meshes.insert({ "sphere", mesh });
	}
	void AssetManager::LoadTextures()
	{
		NCL::Rendering::OGLTexture *basicTex = (NCL::Rendering::OGLTexture*)
												TextureLoader::LoadAPITexture("checkerboard.png");
		m_Textures.insert({"checkerboard", basicTex});
	}
	AssetManager* AssetManager::GetInstance()
	{
		return m_Instance;
	}

	void AssetManager::Init()
	{
		m_Instance = new AssetManager();
	}

	void AssetManager::Finish()
	{
		delete m_Instance;
	}

	AssetManager::~AssetManager()
	{
		for (auto& i : m_Meshes)
		{
			delete i.second;
		}

		for (auto& i : m_Textures)
		{
			delete i.second;
		}
	}

	NCL::Rendering::OGLMesh* AssetManager::GetMesh(const char* name)
	{
		return m_Meshes.at(name);
	}

	NCL::Rendering::OGLTexture* AssetManager::GetTexture(const char* name)
	{
		return m_Textures.at(name);
	}

};
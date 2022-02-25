
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include "AssetManager.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Common/MeshGeometry.h"
#include "../../Common/TextureLoader.h"
#include <experimental/filesystem>
#include "../../Common/Assets.h"
#include "../../Common/MeshMaterial.h"
#include "../../Common/Matrix4.h"
#include "../../include/assimp/Importer.hpp"
#include <assimp/scene.h>
#include<assimp/postprocess.h>
#include <stack>

NCL::Maths::Matrix4 GetMatrix(aiMatrix4x4 mat)
{
	return NCL::Maths::Matrix4({mat.a1,mat.b1,mat.c1,mat.d1,
								mat.a2,mat.b2,mat.c2,mat.d2,
								mat.a3,mat.b3,mat.c3,mat.d3,
								mat.a4,mat.b4,mat.c4,mat.d4 });

}
void processNode(const aiNode* rootNode, const aiScene* scene, aiMesh *&outMesh, NCL::Maths::Matrix4 &transform)
{
	std::stack<const aiNode*> nodes;
	nodes.push(rootNode);

	while (!nodes.empty())
	{
		const aiNode* node = nodes.top();
		nodes.pop();
		
		for (unsigned int j = 0; j < node->mNumMeshes; j++)
		{
			transform = GetMatrix(node->mTransformation);			
			outMesh =  scene->mMeshes[node->mMeshes[j]];
			return;
		}
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			nodes.push(node->mChildren[i]);
		}
	}	
}

namespace NCL
{
	AssetManager* AssetManager::m_Instance = nullptr;

	AssetManager::AssetManager()
	{
		m_Importer = new Assimp::Importer();
		
		
		LoadMeshes();
		LoadTextures();
		LoadMaterials();
	}
	void AssetManager::LoadMeshes()
	{
		auto loadFunc = [](const std::string& name) {
			NCL::Rendering::OGLMesh* into = new NCL::Rendering::OGLMesh(name);
			(into)->SetPrimitiveType(NCL::GeometryPrimitive::Triangles);
			(into)->UploadToGPU();
			return into;
		};
		std::string filename;
		aiMesh* mesh = nullptr;
		NCL::Maths::Matrix4 transform;

		for (const auto& entry : std::experimental::filesystem::directory_iterator(Assets::MESHDIR))
		{
			if (entry.path().extension().generic_string().compare(".msh") == 0)			
			{
				filename = entry.path().filename().generic_string();
				NCL::Rendering::OGLMesh* mesh = loadFunc(filename.c_str());				
				m_Meshes.insert({filename, mesh});
			}
			if (entry.path().extension().generic_string().compare(".fbx") == 0)
			{
				filename = entry.path().filename().generic_string();
				const aiScene* scene = m_Importer->ReadFile(entry.path().generic_string().c_str(), aiProcess_Triangulate  | aiProcess_GenNormals);
				
				processNode(scene->mRootNode, scene, mesh, transform);
				NCL::Rendering::OGLMesh* into = new NCL::Rendering::OGLMesh(mesh, transform);
				(into)->SetPrimitiveType(NCL::GeometryPrimitive::Triangles);
				(into)->UploadToGPU();
				m_Meshes.insert({ filename, into });
			}
		}

	}
	void AssetManager::LoadTextures()
	{
		NCL::Rendering::OGLTexture *basicTex = (NCL::Rendering::OGLTexture*)
												TextureLoader::LoadAPITexture("checkerboard.png");
		m_Textures.insert({"checkerboard", basicTex});
	}
	void AssetManager::LoadMaterials()
	{
		std::string filename;
		for (const auto& entry : std::experimental::filesystem::directory_iterator(Assets::MESHDIR))
		{
			if (entry.path().extension().generic_string().compare(".mat") == 0)
			{
				filename = entry.path().filename().generic_string();
				NCL::MeshMaterial* material = new NCL::MeshMaterial(filename.c_str());
				material->LoadTextures();
				m_Materials.insert({filename, material});
			}
		}
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

		for (auto& i : m_Materials)
		{
			delete i.second;
		}

		delete m_Importer;
	}

	NCL::Rendering::OGLMesh* AssetManager::GetMesh(const char* name)
	{
		return m_Meshes.at(name);
	}

	NCL::Rendering::OGLTexture* AssetManager::GetTexture(const char* name)
	{
		return m_Textures.at(name);
	}

	NCL::MeshMaterial* AssetManager::GetMaterial(const char*name)
	{
		return m_Materials.at(name);
	}

	Assimp::Importer* AssetManager::GetAssetImporter()
	{
		return m_Importer;
	}

};
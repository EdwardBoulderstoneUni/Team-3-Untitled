#include "AssimpHelper.h"
#include "../../include/assimp/Importer.hpp"
#include <assimp/scene.h>
#include<assimp/postprocess.h>
#include <stack>
#include <vector>
#include "../../Common/Matrix4.h"
#include "../../Common/MeshMaterial.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"

#define GET_NCL_MATRIX4(mat)	NCL::Maths::Matrix4({ mat.a1,mat.b1,mat.c1,mat.d1,\
								mat.a2,mat.b2,mat.c2,mat.d2,\
								mat.a3,mat.b3,mat.c3,mat.d3,\
								mat.a4,mat.b4,mat.c4,mat.d4 });

namespace  NCL
{
	AssimpHelper* AssimpHelper::m_Instance = nullptr;
	AssimpHelper::AssimpHelper()
	{
		m_Importer = new Assimp::Importer();
	}
	AssimpHelper::~AssimpHelper()
	{
		delete m_Importer;
	}
	void AssimpHelper::Init()
	{
		m_Instance = new AssimpHelper();
	}
	void AssimpHelper::Clear()
	{
		delete m_Instance;
	}
	void AssimpHelper::ProcessFBX(const char* filePath, Rendering::OGLMesh*& outMesh, MeshMaterial*& outMaterial)
	{
		const aiScene* scene = m_Importer->ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);
		outMaterial = new MeshMaterial();
		NCL::Maths::Matrix4 transform;
		std::vector<aiMesh*> meshes;
		
		//traverse scene tree iteratively
		std::stack<const aiNode*> nodes;
		nodes.push(scene->mRootNode);
		
		while (!nodes.empty())
		{
			const aiNode* node = nodes.top();
			nodes.pop();

			transform = GET_NCL_MATRIX4(node->mTransformation);
			for (unsigned int j = 0; j < node->mNumMeshes; j++)
			{
				meshes.push_back(scene->mMeshes[node->mMeshes[j]]);
				TryLoadMaterial(scene, scene->mMeshes[node->mMeshes[j]], outMaterial);
			}
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				nodes.push(node->mChildren[i]);
			}
		}
		//append all meshses as submeshes to one single mesh
		outMesh = new Rendering::OGLMesh(transform);
		for (auto& mesh : meshes)
		{
			outMesh->AddSubMeshFromFBXData(static_cast<void*>(mesh));
		}
		//load materials
		outMaterial->LoadTextures();
	}

	void AssimpHelper::TryLoadMaterial(const aiScene* scene, const aiMesh* mesh, MeshMaterial*& outMaterial) const
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		aiString texture_file;
		material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texture_file);
		const aiTexture* texture = scene->GetEmbeddedTexture(texture_file.C_Str());
		if (texture != nullptr) //returned pointer is not null, read texture from memory
		{
			LoadTextureFromMemory(texture, texture_file.C_Str(), outMaterial);
		}
		else //regular file, check if it exists and read it
		{			
			LoadTextureFromFile(texture, texture_file.C_Str(), outMaterial);
		}
	}

	void AssimpHelper::LoadTextureFromMemory(const aiTexture* texture, const char *fileName, MeshMaterial*& material) const
	{
		NCL::MeshMaterialEntry materialEntry;
		if (texture->mHeight != 0)
		{
			materialEntry.AddEntry("Diffuse", fileName, Rendering::OGLTexture::RGBATextureFromData
			(reinterpret_cast<char*>(texture->pcData), texture->mWidth, texture->mHeight, 4));
		}
		else
		{
			materialEntry.AddEntry("Diffuse", fileName, Rendering::OGLTexture::RGBATextureFromCompressedData
			(reinterpret_cast<char*>(texture->pcData), texture->mWidth));
		}
		material->AddEntry(materialEntry);
	}

	void AssimpHelper::LoadTextureFromFile(const aiTexture* texture, const char* fileName, MeshMaterial*& material) const
	{
		NCL::MeshMaterialEntry materialEntry;
		string name = fileName;
		int pathIndex = name.find_last_of('\\');
		if (pathIndex != string::npos)
		{
			name = name.substr(pathIndex + 1);
		}
		materialEntry.AddEntry("Diffuse", name.c_str(), nullptr);
		material->AddEntry(materialEntry);
	}
}
#include "GameObjectGenerator.h"
#include "..//..//include/rapidjson/document.h"
#include <fstream>
#include <sstream>
#include "../../Common/Vector3.h"
#include "../../Common/Quaternion.h"
#include "../CSC8503Common/SphereVolume.h"
#include "../CSC8503Common/AABBVolume.h"
#include "../CSC8503Common/GameObject.h"
#include "AssetManager.h"
#include "..//..//Plugins/OpenGLRendering/OGLMesh.h"
#include "..//..//Plugins/OpenGLRendering/OGLTexture.h"
#include "..//..//Plugins/OpenGLRendering/ShaderManager.h"
NCL::CSC8503::GameObjectGenerator::GameObjectGenerator()
{
}

NCL::CSC8503::GameObjectGenerator::~GameObjectGenerator()
{
}

void NCL::CSC8503::GameObjectGenerator::Generate(const char* fileName, std::vector<GameObject*>& outobjects)
{
	std::ifstream file(fileName);
	std::stringstream buffer;
	buffer << file.rdbuf();

	rapidjson::Document document;
	document.Parse(buffer.str().c_str());

	if (document.HasMember("Objects"))
	{
		rapidjson::Value& objects = document["Objects"];
		GameObject* object;
		outobjects.clear();
		for (auto i = 0; i < objects.Size(); i++)
		{			
			NCL::Maths::Vector3 position(objects[i]["position"]["x"].GetFloat(),
										objects[i]["position"]["y"].GetFloat(),
										objects[i]["position"]["z"].GetFloat());

			NCL::Maths::Quaternion orientation(objects[i]["quaternion"]["x"].GetFloat(),
											objects[i]["quaternion"]["y"].GetFloat(),
											objects[i]["quaternion"]["z"].GetFloat(),
											objects[i]["quaternion"]["w"].GetFloat());

			NCL::Maths::Vector3 dim(objects[i]["dimensions"]["x"].GetFloat(),
				objects[i]["dimensions"]["y"].GetFloat(),
				objects[i]["dimensions"]["z"].GetFloat());

			NCL::Maths::Vector3 scale(objects[i]["scale"]["x"].GetFloat(),
				objects[i]["scale"]["y"].GetFloat(),
				objects[i]["scale"]["z"].GetFloat());

			NCL::CollisionVolume* volume = nullptr;
			int objectType = objects[i]["objShape"].GetInt();

			object = new GameObject();
			object->GetTransform().
				SetPosition(position).
				SetScale(scale);

			switch (objectType)
			{
			case 0:
				volume = new NCL::SphereVolume(dim.x);
				object->SetRenderObject(new RenderObject
										(&object->GetTransform(), AssetManager::GetInstance()->GetMesh("sphere"), 
										AssetManager::GetInstance()->GetTexture("checkerboard"), ShaderManager::GetInstance()->GetShader("default")));
				break;
			case 1:
				volume = new NCL::AABBVolume(dim);
				object->SetRenderObject(new RenderObject
				(&object->GetTransform(), AssetManager::GetInstance()->GetMesh("cube"),
					AssetManager::GetInstance()->GetTexture("checkerboard"), ShaderManager::GetInstance()->GetShader("default")));
				break;
			}			
			object->SetBoundingVolume(volume);
			outobjects.push_back(object);
		}
	}
}

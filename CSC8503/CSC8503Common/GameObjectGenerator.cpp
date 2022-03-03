#include "GameObjectGenerator.h"
#include <fstream>
#include <sstream>

#include "../../Common/Quaternion.h"
#include "../CSC8503Common/SphereVolume.h"
#include "../CSC8503Common/AABBVolume.h"
#include "../CSC8503Common/GameObject.h"
#include "AssetManager.h"
#include "..//..//Plugins/OpenGLRendering/OGLMesh.h"
#include "..//..//Plugins/OpenGLRendering/OGLTexture.h"
#include "..//..//Plugins/OpenGLRendering/ShaderManager.h"
#include "../../Common/MeshMaterial.h"
#include "../GameTech/PhysicsXSystem.h"

NCL::CSC8503::GameObjectGenerator::GameObjectGenerator()
{
}

NCL::CSC8503::GameObjectGenerator::~GameObjectGenerator()
{
}

void NCL::CSC8503::GameObjectGenerator::SetTransform(GameObject* object, const rapidjson::Value& value) 
{
	NCL::Maths::Vector3 position, scale;
	NCL::Maths::Vector4 ori;

	GetVector(value, "position", position);
	GetVector(value, "scale", scale);
	GetVector(value, "quaternion", ori);
	
	Quaternion orientation(ori);

	object->GetTransform().
		SetPosition(position).
		SetScale(scale).
		SetOrientation(orientation);
}

void NCL::CSC8503::GameObjectGenerator::SetPhysicsObject(GameObject* object, const rapidjson::Value& value)
{
	NCL::Maths::Vector3 dim;
	//NCL::CollisionVolume* volume = nullptr;
	GeometryData::Data::BoxData* box = nullptr;
	GeometryData::Data::SphereData* sphere = nullptr;

	GetVector(value, "dimensions", dim);
	int objectType = value["objShape"].GetInt();
	
	switch (objectType)
	{
	case 0:
		sphere = new GeometryData::Data::SphereData(dim.x);
		break;
	case 1:
		box = new GeometryData::Data::BoxData(dim);
		break;
	}
	//object->SetBoundingVolume(volume);
}

void NCL::CSC8503::GameObjectGenerator::SetRenderObject(GameObject* object, const rapidjson::Value& value)
{
	int objectType = value["objShape"].GetInt();
	MeshMaterial* material = nullptr;
	if (value.HasMember("materialPath"))
	{
		material = AssetManager::GetInstance()->GetMaterial(value["materialPath"].GetString());
	}

	switch (objectType)
	{
	case 0:
		
		object->SetRenderObject(new RenderObject
		(&object->GetTransform(), AssetManager::GetInstance()->GetMesh(value["meshPath"].GetString()),
			AssetManager::GetInstance()->GetTexture("checkerboard"), ShaderManager::GetInstance()->GetShader("default"), material));
		break;
	case 1:
		
		object->SetRenderObject(new RenderObject
		(&object->GetTransform(), AssetManager::GetInstance()->GetMesh(value["meshPath"].GetString()),
			AssetManager::GetInstance()->GetTexture("checkerboard"), ShaderManager::GetInstance()->GetShader("default"), material));
		break;
	}
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
			object = new GameObject();

			SetTransform(object, objects[i]);
			SetPhysicsObject(object, objects[i]);
			SetRenderObject(object, objects[i]);
			
			outobjects.push_back(object);
		}
	}
}

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
#include "../../Common/ShaderManager.h"
#include "../../Common/MeshMaterial.h"
#include "PhysXConvert.h"
#include "../Paint/PaintableRenderObject.h"


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
	NCL::Maths::Vector3 scale;
	NCL::Maths::Vector3 dim;
	NCL::Maths::Vector3 dimOffset;
	PxGeometry* volume = nullptr;

	GetVector(value, "scale", scale);
	GetVector(value, "dimensions", dim);
	GetVector(value, "dimensionsOrigin", dimOffset);
	int objectType = value["objShape"].GetInt();

	switch (objectType)
	{
	case 0:
		volume = new PxSphereGeometry(dim.x);
		break;
	case 1:
		volume =new PxBoxGeometry(PhysXConvert::Vector3ToPxVec3(dim * scale / 2));
		break;
	case 2:
		volume = new PxCapsuleGeometry(dim.x,dim.y);
		break;
	}
	PxTransform trans = PhysXConvert::TransformToPxTransform(object->GetTransform());
	trans.p = trans.p;
	PhysicsXObject* phyObj = new PhysicsXObject();
	phyObj->properties.type = PhyProperties::Static;
	phyObj->properties.transform = trans;
	phyObj->properties.positionOffset = dimOffset * scale;
	phyObj->properties.volume = volume;
	phyObj->properties.Mass = 10.0f;
	object->SetPhysicsXObject(phyObj);
}

void NCL::CSC8503::GameObjectGenerator::SetRenderObject(GameObject* object, const rapidjson::Value& value)
{
	int objectType = value["objShape"].GetInt();
	MeshMaterial* material = nullptr;
	if (value.HasMember("materialPath"))
	{
		material = AssetManager::GetInstance()->GetMaterial(value["materialPath"].GetString());
	}
	else
	{
		material = AssetManager::GetInstance()->GetMaterial(value["meshPath"].GetString());
	}
	switch (objectType)
	{
	case 0:
		
		object->SetRenderObject(new RenderObject
		(&object->GetTransform(), AssetManager::GetInstance()->GetMesh(value["meshPath"].GetString()),
			AssetManager::GetInstance()->GetTexture("checkerboard"), ShaderManager::GetInstance()->GetShader("default"), material));
		break;
	case 1:
		
		object->SetRenderObject(new PaintableRenderObject
		(&object->GetTransform(), AssetManager::GetInstance()->GetMesh(value["meshPath"].GetString()),
			AssetManager::GetInstance()->GetTexture("checkerboard"), material));
		break;
	case 2:

		object->SetRenderObject(new RenderObject
		(&object->GetTransform(), AssetManager::GetInstance()->GetMesh(value["meshPath"].GetString()),
			AssetManager::GetInstance()->GetTexture("checkerboard"), ShaderManager::GetInstance()->GetShader("default"), material));
		break;
	}
}



void NCL::CSC8503::GameObjectGenerator::Generate(const char* fileName, GameWorld& world)
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
		for (auto i = 0; i < objects.Size(); i++)
		{			
			object = new GameObject();

			SetTransform(object, objects[i]);
			SetPhysicsObject(object, objects[i]);
			SetRenderObject(object, objects[i]);
			
			world.AddGameObject(object);
		}
	}
}

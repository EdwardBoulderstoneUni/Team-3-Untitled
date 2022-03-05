#pragma once
#include<vector>
#include "..//..//include/rapidjson/document.h"
#include "../../Common/Vector3.h"
#include "../../Common/Vector4.h"

namespace NCL {

	namespace CSC8503 {
		class GameObject;

		class GameObjectGenerator
		{

		public:
			GameObjectGenerator() {};
			~GameObjectGenerator();
			void Generate(const char* fileName, std::vector<GameObject*>& objects) ;
			void SetTransform(GameObject*, const rapidjson::Value&);

			void SetPhysicsObject(GameObject*, const rapidjson::Value&);
			void SetRenderObject(GameObject*, const rapidjson::Value&);

			inline void GetVector(const rapidjson::Value& value, const char* attribute, NCL::Maths::Vector3& vector)
			{
				vector.x = value[attribute]["x"].GetFloat();
				vector.y = value[attribute]["y"].GetFloat();
				vector.z = value[attribute]["z"].GetFloat();
			}
			inline void GetVector(const rapidjson::Value& value, const char* attribute, NCL::Maths::Vector4& vector)
			{
				vector.x = value[attribute]["x"].GetFloat();
				vector.y = value[attribute]["y"].GetFloat();
				vector.z = value[attribute]["z"].GetFloat();
				vector.w = value[attribute]["w"].GetFloat();
			}
		private:
		};
	}
}

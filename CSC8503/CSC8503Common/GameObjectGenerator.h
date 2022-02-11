#pragma once
#include<vector>

namespace NCL {
	namespace CSC8503 {
		class GameObject;
		class GameObjectGenerator
		{

		public:
			GameObjectGenerator();
			~GameObjectGenerator();
			void Generate(const char* fileName, std::vector<GameObject*>& objects);
		};
	}
}

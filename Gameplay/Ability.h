#pragma once

#include "ePlayerRole.h"
#include "..//Common/Vector2.h"
#include <fstream>
#include <sstream>

#include "../include/rapidjson/document.h"

namespace NCL {
	namespace CSC8503 {
		class Ability {
		public:
			Ability(const char* colourName, int abilityNumber);
			~Ability();

			inline void GetVector(const rapidjson::Value& value, const char* attribute, NCL::Maths::Vector2& vector)
			{
				vector.x = value[attribute]["x"].GetFloat();
				vector.y = value[attribute]["y"].GetFloat();
			}

		private:
			float cooldown;
			float range;
			NCL::Maths::Vector2 size;
			float startUp;
			float endLag;

			bool isFiring;
		};
	}
}
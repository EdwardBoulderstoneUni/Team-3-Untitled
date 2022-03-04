#include "Ability.h"
#include "..//CSC8503/CSC8503Common/GameObjectGenerator.h"
#include <string>
#include <iostream>

namespace NCL {
	namespace CSC8503 {
		Ability::Ability(const char* colourName, int abilityNumber) {
			std::ifstream file("Ability.json");
			std::stringstream buffer;
			buffer << file.rdbuf();

			rapidjson::Document document;
			document.Parse(buffer.str().c_str());

			if (document.HasMember(colourName))
			{
				rapidjson::Value& objects = document[colourName];

				cooldown = objects[abilityNumber]["cooldown"].GetFloat();
				range = objects[abilityNumber]["range"].GetFloat();
				GetVector(objects[abilityNumber], "size", size);
				startUp = objects[abilityNumber]["cooldown"].GetFloat();
				endLag = objects[abilityNumber]["cooldown"].GetFloat();
			}
		}
	}
}
#include "AbilityContainer.h"

namespace NCL {
	namespace CSC8503 {
		AbilityContainer::AbilityContainer() {
			std::string colour = "Red";
			allAbilities[0] = new Ability(colour.c_str(), 0);
			allAbilities[1] = new Ability(colour.c_str(), 1);
			colour = "Green";
			allAbilities[2] = new Ability(colour.c_str(), 0);
			allAbilities[3] = new Ability(colour.c_str(), 1);
			colour = "Blue";
			allAbilities[4] = new Ability(colour.c_str(), 0);
			allAbilities[5] = new Ability(colour.c_str(), 1);
		}

		AbilityContainer::~AbilityContainer() {
			for (int i = 0; i < 6; ++i)
			{
				if (allAbilities != nullptr)
				{
					delete allAbilities[i];
					allAbilities[i] = nullptr;
				}
			}
		}
	}
}
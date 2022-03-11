#pragma once
#include "Ability.h"

namespace NCL {
	namespace CSC8503 {
		class Ability;
		class AbilityContainer {
		public:
			AbilityContainer();
			~AbilityContainer();

			Ability *allAbilities[6];
		};
	}
}

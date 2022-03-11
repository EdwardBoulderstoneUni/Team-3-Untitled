#include "Player.h"

namespace NCL {
	namespace CSC8503 {
		Player::Player(PlayerRole colour, AbilityContainer aCont) {
			pColour = colour;
			AssignRole(aCont);
		}

		Player::~Player() {
			for (auto i : abilities)
				delete i;
		}

		void Player::Move() {

		}

		void Player::Jump() {

		}

		void Player::Shoot() {

		}

		void Player::AssignRole(AbilityContainer aCont) {
			std::string colour;

			switch (pColour) {
			case Red:
				colour = "Red";
				abilities[0] = aCont.allAbilities[0];
				abilities[1] = aCont.allAbilities[1];
				break;
			case Green:
				colour = "Green";
				abilities[0] = aCont.allAbilities[2];
				abilities[1] = aCont.allAbilities[3];
				break;
			case Blue:
				colour = "Blue";
				abilities[0] = aCont.allAbilities[4];
				abilities[1] = aCont.allAbilities[5];
				break;
			}
		}
	}
}
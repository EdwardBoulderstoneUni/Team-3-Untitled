#include "Player.h"

namespace NCL {
	namespace CSC8503 {
		Player::Player(PlayerRole colour, Vector3 transform) {
			pColour = colour;
			AssignRole();
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

		void Player::AssignRole() {
			std::string colour;

			switch (pColour) {
			case Red:
				colour = "Red";
				abilities[0] = new Ability(colour.c_str(), 0);
				abilities[1] = new Ability(colour.c_str(), 1);
				break;
			case Green:
				colour = "Green";
				abilities[0] = new Ability(colour.c_str(), 0);
				abilities[1] = new Ability(colour.c_str(), 1);
				break;
			case Blue:
				colour = "Blue";
				abilities[0] = new Ability(colour.c_str(), 0);
				abilities[1] = new Ability(colour.c_str(), 1);
				break;
			}
		}
	}
}
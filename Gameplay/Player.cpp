#include "Player.h"
#include "../Common/Keyboard.h"
#include "../Common/Window.h"

namespace NCL {
	namespace CSC8503 {
		Player::Player(PlayerRole colour, AbilityContainer* aCont) {
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
			int jumpcount = 0;
			if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE) && isGrounded) {
				GetPhysicsXObject()->AddForce(Vector3(0.0f, 15.0f, 0.0f));
				jumpcount++;
			}
			//Press SPACE to double jump if player is jumping and not on the ground yet
			if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SPACE) && !isGrounded && jumpcount==1) {
				GetPhysicsXObject()->AddForce(Vector3(0.0f, 15.0f, 0.0f));
				jumpcount = 0;
			}
			
		}

		void Player::Dash() {
			forward = Quaternion(transform.GetOrientation())* Vector3(0, 0, 1);
			if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SHIFT)) {
				GetPhysicsXObject()->AddForce(forward * 5.0f);
			}
		}

		void Player::Shoot() {

		}


		void Player::AssignRole(AbilityContainer* aCont) {
			std::string colour;

			switch (pColour) {
			case Red:
				colour = "Red";
				abilities[0] = aCont->allAbilities[0];
				abilities[1] = aCont->allAbilities[1];
				break;			
			case Green:			
				colour = "Green";
				abilities[0] = aCont->allAbilities[2];
				abilities[1] = aCont->allAbilities[3];
				break;				
			case Blue:				
				colour = "Blue";	
				abilities[0] = aCont->allAbilities[4];
				abilities[1] = aCont->allAbilities[5];
				break;
			}
		}
	}
}
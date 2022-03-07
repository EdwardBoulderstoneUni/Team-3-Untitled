#include "Player.h"
#include "../Common/Keyboard.h"
#include "../Common/Window.h"

namespace NCL {
	namespace CSC8503 {
		Player::Player(PlayerRole colour, AbilityContainer* aCont)
		{
			forward = Quaternion(transform.GetOrientation()) * Vector3(0, 0, 1);
			right = Vector3::Cross(Vector3(0, 1, 0), -forward);
			pColour = colour;
			AssignRole(aCont);
			InitComponents();
		}

		Player::~Player() {
			for (auto i : abilities)
				delete i;

			for (auto i : components)
				delete i;
		}

		void Player::InitComponents() {
			for (auto i : components)
				i->Init();
		}

		void Player::Update() {
			UpdateComponents();
		}

		void Player::UpdateComponents() {
			for (auto i : components)
				i->Update();
		}

		void Player::Move() {
			// Move forward
			if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::W)) {
				GetPhysicsXObject()->AddForce(forward * 5.0f);
			}
			// Move backward
			if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::S)) {
				GetPhysicsXObject()->AddForce(-forward * 5.0f);
			}
			// Move left
			if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::A)) {
				GetPhysicsXObject()->AddForce(-right * 5.0f);
			}
			// Move right
			if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::D)) {
				GetPhysicsXObject()->AddForce(right * 5.0f);
			}
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
			if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::SHIFT)) {
				GetPhysicsXObject()->AddForce(forward * 15.0f);
			}
		}

		ComponentCamera* Player::GetComponentCamera()
		{
			return nullptr;
		}

		ComponentInput* Player::GetComponentInput()
		{
			return nullptr;
		}

		ComponentPhysics* Player::GetComponentPhysics()
		{
			return nullptr;
		}

		void Player::Shoot() {

		}

		void Player::AssignRole(AbilityContainer* aCont)
		{
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
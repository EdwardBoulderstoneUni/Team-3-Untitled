#include "Player.h"
#include "../Common/Keyboard.h"
#include "../Common/Window.h"
#include "PlayerController.h"
namespace NCL {
	namespace CSC8503 {
		Player::Player(PlayerRole colour, AbilityContainer* aCont)
		{
			forward = Quaternion(transform.GetOrientation()) * Vector3(0, 0, 1);
			right = Vector3::Cross(Vector3(0, 1, 0), -forward);
			pColour = colour;
			AssignRole(aCont);
		}

		Player::~Player() {
			for (auto i : abilities)
				delete i;
		}
		void Player::SetUp()
		{
			auto physics = new ComponentPhysics();
			physics->motionType = ComponentPhysics::Dynamic;
			physics->center = this->GetTransform().GetPosition();
			physics->orientation = this->GetTransform().GetOrientation();
			physics->size = this->GetTransform().GetScale();
			physics->shapeType = ComponentPhysics::Box;
			physics->mass = 20.0f;
			physics->phyObj = GetPhysicsXObject();

			PushComponet(physics);

			auto input = new ComponentInput();
			input->Callback[jump] = [this]() {
				this->Jump();
			};
			input->Callback[dash] = [this]() {
				this->Dash();
			};
			auto* controller = new PlayerController();
			input->userInterface = new UserInterface(controller);
			PushComponet(input);
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
			GetPhysicsXObject()->SetLinearVelocity(Vector3(0.0f,10.0f, 0.0f));	
		}

		void Player::Dash() {
			GetPhysicsXObject()->SetLinearVelocity(forward * 50.0f);
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
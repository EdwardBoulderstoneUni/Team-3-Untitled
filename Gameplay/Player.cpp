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
			input->Move = [this](Vector2 movement) {
				this->Move(movement);
			};
			auto* controller = new PlayerController();
			input->userInterface = new UserInterface(controller);
			PushComponet(input);

			auto camera = new ComponentCamera();
			camera->gO = this;

			camera->camera = new Camera();
			camera->camera->SetNearPlane(0.1f);
			camera->camera->SetFarPlane(500.0f);
			camera->camera->SetPitch(-15.0f);
			camera->camera->SetYaw(180);

			PushComponet(camera);
		}
		void Player::Move() {
			// Move forward
			if (GetComponentInput()->userInterface->get_movement().y > 0) {
				GetPhysicsXObject()->AddForce(forward * 5.0f);
			}
			// Move backward
			if (GetComponentInput()->userInterface->get_movement().y < 0) {
				GetPhysicsXObject()->AddForce(-forward * 5.0f);
			}
			// Move left
			if (GetComponentInput()->userInterface->get_movement().x < 0) {
				GetPhysicsXObject()->AddForce(-right * 5.0f);
			}
			// Move right
			if (GetComponentInput()->userInterface->get_movement().x > 0) {
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
#include "Player.h"
#include "../Common/Keyboard.h"
#include "../Common/Window.h"
#include "PlayerController.h"
#include "../CSC8503/CSC8503Common/PhysicsXSystem.h"
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
			physics->phyObj = GetPhysicsXObject();
			PhyProperties properties = PhyProperties();
			properties.type = PhyProperties::Character;
			properties.transform = PhysXConvert::TransformToPxTransform(GetTransform());
			properties.Mass = 10.0f;

			Vector3 scale = GetTransform().GetScale();
			properties.volume = new PxBoxGeometry(PhysXConvert::Vector3ToPxVec3(scale));

			physics->phyObj->properties = properties;
			PushComponet(physics);

			auto input = new ComponentInput();
			input->Callback[jump] = [this]() {
				this->Jump();
			};
			input->Callback[dash] = [this]() {
				this->Dash();
			};
			input->Callback[move] = [this]() {
				this->Move();
			};
			input->Callback[attack] = [this]() {
				this->Shoot();
			};
			input->Callback[idle] = [this]() {
				if (!physicsXObject->controller)return;
				physicsXObject->controller->move(PxVec3(0.0f, -9.81f, 0.0f) * 0.05f, 0.0001f, 0.2,
					PxControllerFilters(), NULL);
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
			forward = Quaternion(transform.GetOrientation()) * Vector3(0, 0, 1);
			right = Vector3::Cross(Vector3(0, 1, 0), -forward);

			// Move forward
			if (GetComponentInput()->userInterface->get_movement().y > 0) {
				physicsXObject->controller->move(PhysXConvert::Vector3ToPxVec3(forward), 0.0001f, 0.2,
					PxControllerFilters(), NULL);
			}
			// Move backward
			if (GetComponentInput()->userInterface->get_movement().y < 0) {
				physicsXObject->controller->move(PhysXConvert::Vector3ToPxVec3(forward)*(-1), 0.0001f, 0.2,
					PxControllerFilters(), NULL);
			}
			// Move left
			if (GetComponentInput()->userInterface->get_movement().x < 0) {
				physicsXObject->controller->move(PhysXConvert::Vector3ToPxVec3(-right) * (-1), 0.0001f, 0.2,
					PxControllerFilters(), NULL);
			};
			// Move right
			if (GetComponentInput()->userInterface->get_movement().x > 0) {
				physicsXObject->controller->move(PhysXConvert::Vector3ToPxVec3(right) * (-1), 0.0001f, 0.2,
					PxControllerFilters(), NULL);
			}
		}//TODO just need dir     rewrite later!!

		void Player::Jump() {
			physicsXObject->controller->move(PxVec3(0.0f, 1.0f, 0.0f), 0.0001f, 0.2,
				PxControllerFilters(), NULL);
		}

		void Player::Dash() {
		
		}

		void Player::Shoot() {
			auto bullet = new ComponentPhysics();
			bullet->phyObj = GetPhysicsXObject();
			PxVec3 vel = physicsXObject->properties.transform.rotate(PhysXConvert::Vector3ToPxVec3(Vector3(0, 0, -1) * 200));
			PxTransform t = physicsXObject->properties.transform;
			PhyProperties properties = PhyProperties();
			properties.type = PhyProperties::Dynamic;
			properties.transform = t;
			properties.Mass = 3.0f;
			Vector3 scale = GetTransform().GetScale();
			properties.volume = new PxSphereGeometry(scale.x);
			bullet->phyObj->SetLinearVelocity(PhysXConvert::PxVec3ToVector3(vel));

			bullet->phyObj->properties = properties;
			PushComponet(bullet);
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
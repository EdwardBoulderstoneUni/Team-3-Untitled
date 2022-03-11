#include "Player.h"
#include "../Common/Keyboard.h"
#include "../Common/Window.h"
#include "PlayerController.h"
#include "../CSC8503/CSC8503Common/PhysicsXSystem.h"
void _OnShootEvent(const EVENT* pEvent, UINT dwOwnerData)
{

}
namespace NCL {
	namespace CSC8503 {
		Player::Player(PlayerRole colour, AbilityContainer* aCont, GameObjectType type)
		{
			forward = Quaternion(transform.GetOrientation()) * Vector3(0, 0, 1);
			right = Vector3::Cross(Vector3(0, 1, 0), -forward);
			pColour = colour;
			AssignRole(aCont);
			this->type = type;
		}

		Player::~Player() {
			for (auto i : abilities)
				delete i;

			delete bullet;
		}

		void Player::SetUp()
		{
			auto physics = new ComponentPhysics();
			physics->phyObj = GetPhysicsXObject();
			PhyProperties properties = PhyProperties();
			properties.type = PhyProperties::Character;
			properties.transform = PhysXConvert::TransformToPxTransform(GetTransform());
			properties.Mass = 10.0f;

			Vector3 scale = GetTransform().GetScale()/2.0f;
			properties.volume = new PxBoxGeometry(PhysXConvert::Vector3ToPxVec3(scale));

			physics->phyObj->properties = properties;
			PushComponet(physics);

			auto input = new ComponentInput();
			input->ButtonCallback[jump] = [this]() {
				this->Jump();
			};
			input->ButtonCallback[dash] = [this]() {
				this->Dash();
			};
			input->ButtonCallback[attack] = [this]() {
				this->Openfire();
			};
			input->MovCallback = [this](Vector2 dir) {
				this->Move(dir);
			};
			input->ButtonCallback[reload] = [this]() {
				this->Reload();
			};
			
			input->UpdateCallback = [this](float dt) {
				if (!physicsXObject->controller)return;
				physicsXObject->controller->move(PxVec3(0.0f, -9.81f, 0.0f) * 0.05f, 0.0001f, 0.2,
					PxControllerFilters(), NULL);

				forward = Quaternion(transform.GetOrientation()) * Vector3(0, 0, 1);
				right = Vector3::Cross(Vector3(0, 1, 0), -forward);

				dashCooldown -= dt;
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
		void Player::Move(Vector2 dir) {
			
			// Move forward
			if (dir.y>0) {
				physicsXObject->controller->move(PhysXConvert::Vector3ToPxVec3(forward), 0.0001f, 0.2,
					PxControllerFilters(), NULL);
			}
			// Move backward
			if (dir.y<0) {
				physicsXObject->controller->move(PhysXConvert::Vector3ToPxVec3(forward)*(-1), 0.0001f, 0.2,
					PxControllerFilters(), NULL);
			}
			// Move left
			if (dir.x<0) {
				physicsXObject->controller->move(PhysXConvert::Vector3ToPxVec3(right) * (-1), 0.0001f, 0.2,
					PxControllerFilters(), NULL);
			};
			// Move right
			if (dir.x>0) {
				physicsXObject->controller->move(PhysXConvert::Vector3ToPxVec3(right), 0.0001f, 0.2,
					PxControllerFilters(), NULL);
			}
		}

		void Player::Jump() {
			if (not isGrounded)return;
			physicsXObject->controller->move(PxVec3(0.0f, 50.0f, 0.0f), 0.0001f, 0.2, PxControllerFilters(), NULL);
			isGrounded = false;
		}
		void Player::Dash() {
				if (dashCooldown <= 0.0f) {
					physicsXObject->controller->move(PhysXConvert::Vector3ToPxVec3(forward) * 10.0f, 0.0001f, 0.2,
						PxControllerFilters(), NULL);
					dashCooldown = 2.0f;
				}
		}
		void Player::Openfire() {
			if (ammo > 0) {
				YiEventSystem::GetMe()->PushEvent(PLAYER_OPEN_FIRE);
				ammo--;
			}
		}
		float Player::TakeDamage(float dmg) {
			health = health - dmg < 0 ? 0 : health - dmg;
			return health;
		}

		bool Player::IsDead() {
			return health == 0 ? true : false;
		}

		// Give damage to palyer a
		void Player::GiveDamage(float dmg, Player* a) {
			a->TakeDamage(dmg);
			if (a->IsDead() == true) {
				teamKill++;
			}
		}



		void Player::Reload() {
			isReloading = false;
			if (ammo >= 0 && ammo < maxAmmo) {				
				isReloading = true;
				ammo = maxAmmo;
				// Finish reload
				isReloading = false;
			}
		}

		void Player::AssignRole(AbilityContainer* aCont)
		{
			std::string colour;

			switch (pColour) {
			case PlayerRole_red:
				colour = "Red";
				abilities[0] = aCont->allAbilities[0];
				abilities[1] = aCont->allAbilities[1];
				bullet = new Bullet(static_cast<GameObjectType>(this->type + 1), PlayerRole_red);
				break;
			case PlayerRole_green:
				colour = "Green";
				abilities[0] = aCont->allAbilities[2];
				abilities[1] = aCont->allAbilities[3];
				bullet = new Bullet(static_cast<GameObjectType>(this->type + 1), PlayerRole_green);
				break;
			case PlayerRole_blue:
				colour = "Blue";
				abilities[0] = aCont->allAbilities[4];
				abilities[1] = aCont->allAbilities[5];
				bullet = new Bullet(static_cast<GameObjectType>(this->type + 1), PlayerRole_blue);
				break;
			}
		}
	
	}
}


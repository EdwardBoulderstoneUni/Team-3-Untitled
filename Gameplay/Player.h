#pragma once
#include "../CSC8503/CSC8503Common/GameObject.h"
#include "Ability.h"
#include "AbilityContainer.h"
#include "ePlayerRole.h"

namespace NCL {
	namespace CSC8503 {		
		class Player : public GameObject {
		public:
			Player(PlayerRole colour, AbilityContainer aCont);
			~Player();

			void Move();
			void Jump();
			void Dash();
			void Shoot();
			void AssignRole(AbilityContainer aCont);

			void OnCollisionBegin(GameObject* otherObject) override{
				if (otherObject->GetName() == "floor") {
					isGrounded = true;
				}
			}

			void OnCollisionEnd(GameObject* otherObject) override{
				if (otherObject->GetName() == "floor") {
					isGrounded = false;
				}
			}
		private:

			float health = 100.0f;
			float speed = 5.0f;
			
			// t is short for timer (cooldowns)
			float tDash;
			float dashCooldown;
			float tAbility1;
			float tAbility2;
			float tDeath;

			int ammo = 20;

			bool isGrounded = true;
			bool isDashing = false;
			bool isDead = false;
			bool hasAmmo = true;

			PlayerRole pColour;
			NCL::CSC8503::Ability *abilities[2];

			Vector3 forward;
		};
	}
}
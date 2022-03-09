#pragma once
#include "ComponetGameObject.h"
#include "Ability.h"
#include "AbilityContainer.h"

#include "ePlayerRole.h"

namespace NCL {
	namespace CSC8503 {		
		class Player : public ComponetGameObject {
		public:
			Player(PlayerRole colour, AbilityContainer* aCont);
			~Player();

			void SetUp() override;

			void Move();
			void Jump();
			void Shoot();
			void AssignRole(AbilityContainer* aCont);
			void Dash();

			bool isGrounded = false;

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

			bool isDashing = false;
			bool isDead = false;
			bool hasAmmo = true;

			PlayerRole pColour;
			NCL::CSC8503::Ability *abilities[2];

			std::vector<Component> components;

			Vector3 forward;
			Vector3 right;
		};
	}
}
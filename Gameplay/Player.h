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
			void GiveDamage(float dmg, Player* a);
			bool IsDead();
			float TakeDamage(float dmg);
			void Reload();
			void AssignRole(AbilityContainer* aCont);
			void Dash();

		private:
			float health = 100.0f;
			float speed = 5.0f;
			
			// t is short for timer (cooldowns)
			float tDash;
			float dashCooldown;
			float tAbility1;
			float tAbility2;
			float tDeath;
			float damage;
		
			int maxAmmo = 20;
			int ammo = 20;
			int teamKill = 0;

			bool isGrounded = true;
			bool isDashing = false;
			bool hasAmmo = true;
			bool isReloading = false;

			PlayerRole pColour;
			NCL::CSC8503::Ability *abilities[2];

			std::vector<Component> components;

			Vector3 forward;
			Vector3 right;
			PxTransform camOri;
		};
	}
}
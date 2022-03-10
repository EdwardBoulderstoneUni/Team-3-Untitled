#pragma once
#include "ComponetGameObject.h"
#include "Ability.h"
#include "AbilityContainer.h"
#include "../CSC8503/GameTech/YiEventSystem.h"
#include "ePlayerRole.h"
#include "Bullet.h"
namespace NCL {
	namespace CSC8503 {		
		class Player : public ComponetGameObject {
		public:
			Player(PlayerRole colour, AbilityContainer* aCont, GameObjectType type);
			~Player();

			void SetUp() override;

			void Move(Vector2 dir);
			void Jump();
			void GiveDamage(float dmg, Player* a);
			bool IsDead();
			bool CanShoot();
			float TakeDamage(float dmg);
			void Reload();
			void AssignRole(AbilityContainer* aCont);
			void Dash();

			Bullet* GetBullet() {
				return bullet;
			}

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
			float damage;
		
			int maxAmmo = 20;
			int ammo = 20;
			int teamKill = 0;

			bool isDashing = false;
			bool hasAmmo = true;
			bool isReloading = false;
			
			PlayerRole pColour;
			NCL::CSC8503::Ability *abilities[2];

			std::vector<Component> components;

			Vector3 forward;
			Vector3 right;
			PxTransform camOri;

			Bullet* bullet;
		};
	}
}
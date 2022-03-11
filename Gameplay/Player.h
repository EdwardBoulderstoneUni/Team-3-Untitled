#pragma once
#include "ComponentGameObject.h"
#include "Ability.h"
#include "AbilityContainer.h"
#include "../CSC8503/GameTech/YiEventSystem.h"
#include "ePlayerRole.h"
#include "Bullet.h"
namespace NCL {
	namespace CSC8503 {		
		class Player : public ComponentGameObject {
		public:
			Player(PlayerRole colour, AbilityContainer* aCont, GameObjectType type);
			~Player();

			void SetUp() override;

			void Move(Vector2 dir);
			void Jump();
			void GiveDamage(float dmg, Player* a);
			bool IsDead();
			float TakeDamage(float dmg);
			void Reload();
			void AssignRole(AbilityContainer* aCont);
			void Dash();
			int GetAmmo() {
				return ammo;
			}
			int GetTime() {
				return time;
			}

			Bullet* GetBullet() {
				return bullet;
			}

			void Openfire();
			Vector3 GetForward() { return forward; }
			void SetForward(Vector3 val) { forward = val; }
			Vector3 GetShootDiretion() { return shootDir; }
			void SetShootDirection(Vector3 val) { shootDir = val; }

			bool isGrounded = false;

		private:
			float health = 100.0f;
			float speed = 5.0f;
			
			// t is short for timer (cooldowns)
			float tDash;
			float dashCooldown=0.0f;
			float tAbility1;
			float tAbility2;
			float tDeath;
			float damage;
		
			int maxAmmo = 20;
			int ammo = 20;
			int teamKill = 0;
			int jumpNo = 0;
			int time = 0;

			bool isJumping = false;
			bool hasAmmo = true;
			bool isReloading = false;
			
			PlayerRole pColour;
			NCL::CSC8503::Ability *abilities[2];

			std::vector<Component> components;

			Vector3 forward;
			Vector3 right;
			Vector3 shootDir;
			PxTransform camOri;

			Bullet* bullet;

		};
	}
}
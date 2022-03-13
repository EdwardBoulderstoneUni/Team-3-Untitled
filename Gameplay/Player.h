#pragma once
#include "ComponentGameObject.h"
#include "Ability.h"
#include "AbilityContainer.h"
#include "../CSC8503/CSC8503Common/StateMachine.h"
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

			bool isGrounded=true;

		private:
			float health = 100.0f;
			float CurrentJumpspeed = 2.0f;
			float JumpHeight = 5.0f;
			float CurrentHeight = 0.0f;
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

		
			bool isDashing;
			bool isWalking;

			bool hasAmmo = true;
			bool isReloading = false;
			
			StateMachine* playerState;
			StateMachine* weaponState;

			PlayerRole pColour;
			Ability *abilities[2];

			vector<Component> components;

			Vector3 forward;
			Vector3 right;
			Vector3 shootDir;
			PxTransform camOri;

			Bullet* bullet;

			void SetupStateMachine();
			Input lastInput;
		};
	}
}
#pragma once
#include "ComponentGameObject.h"
#include "Ability.h"
#include "AbilityContainer.h"
#include "../CSC8503/CSC8503Common/PushdownMachine.h"
#include "../CSC8503/GameTech/YiEventSystem.h"
#include "ePlayerRole.h"
#include "Bullet.h"
namespace NCL {
	namespace CSC8503 {
		class Player : public ComponentGameObject {
		public:
			Player(PlayerRole colour, AbilityContainer* aCont, GameObjectType type);
			~Player();
			virtual void Update(float dt)override;
			void SetUp() override;

			void Move();
			void Jump(float dt);
	
			void GiveDamage(float dmg, Player* a);
			bool IsDead();
			void Respawn();
			float TakeDamage(float dmg);
			void Reload();
			void AssignRole(AbilityContainer* aCont);
			void Dash(float dt);
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

			bool GetGrounded(){ return isGrounded; }
			void SetGrounded(bool s) { isGrounded = s; }
			Input GetLastInput() { return lastInput; }
			float GetJumpingTimeStack() { return JumpingTimeStack; }
			void SetJumpingTimeStack(float t) { JumpingTimeStack = t; }
			float GetDashingTimeStack() { return DashingTimeStack; }
			void  SetDashingTimeStack(float t) { DashingTimeStack = t; }
			bool DashAlready() { return dashCooldown <= 0 ? true : false; }
			void DashCooldown() { dashCooldown = 1.0f; }
			void RespawnCooldown() { respawnCooldown = 1.0f; }
		private:
		private:
			float health = 100.0f;

			//this is for jumping
			float JumpingTimeStack;
			bool isGrounded = true;

			//this is for dashing
			float DashingTimeStack;
			float dashCooldown= 1.0f;
			float respawnCooldown = 1.0f;

			// t is short for timer (cooldowns)
			float tAbility1;
			float tAbility2;
			float tDeath;
			float damage;
		
			int maxAmmo = 20;
			int ammo = 20;
			int teamKill = 0;
			int jumpNo = 0;
			int time = 0;

			bool hasAmmo = true;
			bool isReloading = false;
			
			PushdownMachine* playerState;
			PushdownMachine* weaponState;
		

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
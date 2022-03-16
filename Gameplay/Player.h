#pragma once
#include "ComponentGameObject.h"
#include "Ability.h"
#include "AbilityContainer.h"
#include "../CSC8503/CSC8503Common/PushdownMachine.h"
#include "../CSC8503/GameTech/YiEventSystem.h"
#include "ePlayerRole.h"

#define DASH_CD 5.0f
#define DASH_DURA 0.2f
#define RESPAWN_CD 5.0f
#define RESPAWN_DURA 5.0f
#define JUMP_DURA 3.2f
#define DOUBLE_DURA 1.5f*3.15f

namespace NCL {
	namespace CSC8503 {
		struct TimeStack{
			float jumpingTimeStack=0.0f;
			float dashingTimeStack=0.0f;
			float dashCooldown=-1.0f;
			float respawnCooldown=-1.0f;
			float deathTimeStack=0.0f;
		};
		class Player : public ComponentGameObject {
		public:
			Player(PlayerRole colour, AbilityContainer* aCont, GameObjectType type,bool localplayer=false);
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
			
			void Dash(float dt);
			int GetAmmo() {
				return ammo;
			}
			int GetHealth() {
				return health;
			}
			void SetHealth(int h) {
				health = h;
			}
			int GetTime() {
				return time;
			}

			void Openfire();
			Vector3 GetForward() { return forward; }
			void SetForward(Vector3 val) { forward = val; }
			Vector3 GetShootDiretion() { return shootDir; }
			void SetShootDirection(Vector3 val) { shootDir = val; }

			bool GetGrounded(){ return isGrounded; }
			void SetGrounded(bool s) { isGrounded = s; }
			Input GetLastInput() { return lastInput; }
			PlayerRole GetRole() { return pColour; }
			int GetScore() { return score; }
			int GetTeamKill() { return teamKill; }
			void AddScore(int s) { score += s; }
			void AddTeamKill(int k) { teamKill += k; }
			TimeStack* GetTimeStack() { return &timeStack; }

			int GetDashCD() { return timeStack.dashCooldown; }
			bool DashCD() {return timeStack.dashCooldown >= 1 ? true : false;}
		private:
			TimeStack timeStack;
			float health = 100.0f;

			bool isGrounded = true;

			// t is short for timer (cooldowns)
			float tAbility1;
			float tAbility2;
			
			int maxAmmo = 20;
			int ammo = 20;
			int teamKill = 0;
			int jumpNo = 0;
			int time = 0;
			int score = 0;

			bool hasAmmo = true;
			bool isReloading = false;
			bool isLocalPlayer;

			PushdownMachine* playerState;
			PushdownMachine* weaponState;
		
			PlayerRole pColour;
			Ability *abilities[2];

			Vector3 forward;
			Vector3 right;
			Vector3 shootDir;
			PxTransform camOri;

			void SetupStateMachine();
			void AssignRole(AbilityContainer* aCont);
			Input lastInput;
		};
	}
}
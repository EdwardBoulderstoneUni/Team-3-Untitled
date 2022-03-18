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
		struct DirectionVec {
		
			void CaculateRight() {
				right=Vector3::Cross(Vector3(0, 1, 0), -forward);
			}
			Vector3 forward;
			Vector3 right;
			Vector3 shootDir;
		};
		struct PlayerPro {
			float health = 100.0f;
			bool isGrounded = true;
			float tAbility1;
			float tAbility2;
			float speed = 1.0f;
			int maxAmmo = 20;
			int ammo = 20;
			int teamKill = 0;
			int score = 0;
			int damage;
		};
		class Player : public ComponentGameObject {
		public:
			Player(PlayerRole colour, AbilityContainer* aCont, GameObjectType type,bool localplayer=false);
			~Player();
			virtual void Update(float dt)override;
			void SetUp() override;

			void Respawn();

			DirectionVec GetDirectionVec() { return dirVec;}

			Input GetLastInput() { return lastInput; }
			PlayerRole GetRole() { return pColour; }
			TimeStack* GetTimeStack() { return timeStack; }
			PlayerPro* GetPlayerPro() { return playerPro; }
		private:
			bool isLocalPlayer;
			TimeStack* timeStack;
			PlayerPro* playerPro;
			PushdownMachine* playerState;
			PushdownMachine* weaponState;
		
			PlayerRole pColour;
			Ability *abilities[2];

			DirectionVec dirVec;

			void SetupStateMachine();
			void AssignRole(AbilityContainer* aCont);
			Input lastInput;
		};
	}
}
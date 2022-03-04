#pragma once
#include "../CSC8503/CSC8503Common/GameObject.h"
#include "Ability.h"
#include "ePlayerRole.h"

namespace NCL {
	namespace CSC8503 {
		class Player : GameObject {
		public:
			Player(PlayerRole colour, Vector3 transform);
			~Player();

			void Move();
			void Jump();
			void Shoot();
			void AssignRole();

		private:

			float health = 100.0f;
			float speed = 5.0f;
			
			// t is short for timer (cooldowns)
			float tDash;
			float tAbility1;
			float tAbility2;
			float tDeath;

			int ammo = 20;

			bool isGrounded = true;
			bool isDashing = false;
			bool isDead = false;
			bool hasAmmo = true;

			PlayerRole pColour;
			Ability* abilities[2];
		};
	}
}
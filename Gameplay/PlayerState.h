#pragma once
#include "../CSC8503/CSC8503Common/PushdownState.h"
#include "Player.h"
using namespace NCL;
using namespace CSC8503;

#define M_INIT										\
Player* player = static_cast<Player*>(userdata);	\
Input lastInput = player->GetLastInput();           \
TimeStack* timeStack = player->GetTimeStack();		\
PhysicsXObject* phyobj =player->GetPhysicsXObject(); \
DirectionVec dir=player->GetDirectionVec();         \
PlayerPro* playerPro=player->GetPlayerPro();         \

#define M_DIE                        \
if (playerPro->health==0){           \
*newState = new Die();               \
(*newState)->userdata = player;      \
return PushdownResult::Push;         \
}                                    \


#define M_STANDINGJUMP                                       \
if (lastInput.buttons[jump]){								 \
	*newState = new StandingJump();							 \
	(*newState)->userdata = player;							 \
	playerPro->isGrounded=false;							 \
	timeStack->jumpingTimeStack = 0.0f;						 \
	return PushdownResult::Push;							 \
}															 \

#define M_DOUBLEJUMP                                         \
if (lastInput.buttons[jump]){								 \
	*newState = new DoubleJump();							 \
	(*newState)->userdata = player;							 \
	timeStack->jumpingTimeStack = 0.0f;						 \
	return PushdownResult::Push;							 \
}															 \

#define M_DASH                                               \
if (lastInput.buttons[dash] and timeStack->dashCooldown < 0){\
*newState = new Dash();										 \
(*newState)->userdata = player;								 \
timeStack->dashingTimeStack = 0.0f;							 \
return PushdownResult::Push;								 \
}															 \

#define M_WALK                                               \
if (lastInput.movement_direction != Vector2()){				 \
	*newState = new Walk();									 \
	(*newState)->userdata = player;							 \
	return PushdownResult::Push;							 \
}															 \


#define M_PLAYER_MOVE                                                                \
if (lastInput.movement_direction == Vector2(0, 1)) {								 \
	phyobj->CMove(PhysXConvert::Vector3ToPxVec3(dir.forward) * playerPro->speed);	 \
}																					 \
if (lastInput.movement_direction == Vector2(0, -1)) {								 \
	phyobj->CMove(PhysXConvert::Vector3ToPxVec3(-dir.forward) * playerPro->speed);	 \
}																					 \
if (lastInput.movement_direction == Vector2(1, 0)) {								 \
	phyobj->CMove(PhysXConvert::Vector3ToPxVec3(dir.right) * playerPro->speed);		 \
}																					 \
if (lastInput.movement_direction == Vector2(-1, 0)) {								 \
	phyobj->CMove(PhysXConvert::Vector3ToPxVec3(-dir.right) * playerPro->speed);	 \
}																					 \

#define M_GRAVITY player->GetPhysicsXObject()->CMove(PxVec3(0, -9.8f, 0) * 0.1f);

#define DASH_OFF 5.0f
class Die :public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {
		M_INIT
			if (timeStack->deathTimeStack > RESPAWN_DURA)
			{
				timeStack->respawnCooldown = RESPAWN_CD;
				timeStack->deathTimeStack = 0.0f;
				playerPro->health = 100.0;
				YiEventSystem::GetMe()->PushEvent(PLAYER_RESPWAN, player->GetWorldID());
				return PushdownResult::Pop;
			}
		timeStack->deathTimeStack += dt;
		M_GRAVITY
			return PushdownResult::NoChange;
	}
};
class Dash :public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {
		M_INIT
			if (timeStack->dashingTimeStack > DASH_DURA)
			{
				timeStack->dashCooldown = DASH_CD;
				return PushdownResult::Pop;
			}
		M_DIE
			if (lastInput.movement_direction == Vector2(0, 1)) {
				phyobj->CMove(PhysXConvert::Vector3ToPxVec3(dir.forward) * DASH_OFF);
			}
		if (lastInput.movement_direction == Vector2(0, -1)) {
			phyobj->CMove(PhysXConvert::Vector3ToPxVec3(-dir.forward) * DASH_OFF);
		}
		if (lastInput.movement_direction == Vector2(1, 0)) {
			phyobj->CMove(PhysXConvert::Vector3ToPxVec3(dir.right) * DASH_OFF);
		}
		if (lastInput.movement_direction == Vector2(-1, 0)) {
			phyobj->CMove(PhysXConvert::Vector3ToPxVec3(-dir.forward) * DASH_OFF);
		}
		if (lastInput.movement_direction == Vector2(0, 0)) {
			phyobj->CMove(PhysXConvert::Vector3ToPxVec3(dir.forward) * DASH_OFF);
		}
		timeStack->dashingTimeStack += dt;
		return PushdownResult::NoChange;
	}
};
class DoubleJump :public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {
		M_INIT
			if (timeStack->jumpingTimeStack > DOUBLE_DURA)
			{
				return PushdownResult::Pop;
			}
		M_DASH
			M_DIE
			M_PLAYER_MOVE
			player->GetPhysicsXObject()->CMove(PxVec3(0, 1, 0) * cos(timeStack->jumpingTimeStack));
		timeStack->jumpingTimeStack += dt * 3;
		return PushdownResult::NoChange;
	}

};
class StandingJump :public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {
		M_INIT
			if (timeStack->jumpingTimeStack > JUMP_DURA)
			{
				return PushdownResult::Pop;
			}
		M_DOUBLEJUMP
			M_DASH
			M_DIE
			M_PLAYER_MOVE
			player->GetPhysicsXObject()->CMove(PxVec3(0, 1, 0) * cos(timeStack->jumpingTimeStack));
		timeStack->jumpingTimeStack += dt * 3;
		return PushdownResult::NoChange;
	}
};
class Walk :public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {
		M_INIT
			if (player->GetLastInput().movement_direction == Vector2()) {
				return PushdownResult::Pop;
			}
		M_STANDINGJUMP
			M_DASH
			M_DIE
			M_PLAYER_MOVE
			M_GRAVITY
			return PushdownResult::NoChange;
	}
};
class Idle :public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {
		M_INIT
			M_STANDINGJUMP
			M_DASH
			M_WALK
			M_DIE
			M_GRAVITY
			return PushdownResult::NoChange;
	}
};




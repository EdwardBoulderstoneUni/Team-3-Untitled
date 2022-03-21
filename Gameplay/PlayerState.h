#pragma once
#include "../CSC8503/CSC8503Common/PushdownState.h"
#include "Player.h"
using namespace NCL;
using namespace CSC8503;

#define M_INIT										\
Player* player = static_cast<Player*>(userdata);	\
Input lastInput = player->GetLastInput();           \
TimeStack* stack = player->GetTimeStack();			\

#define M_DIE                        \
if (player->IsDead())                \
{                                    \
*newState = new Die();               \
(*newState)->userdata = player;      \
return PushdownResult::Push;         \
}                                    \

						
#define M_STANDINGJUMP                                       \
if (lastInput.buttons[jump])								 \
{															 \
	*newState = new StandingJump();							 \
	(*newState)->userdata = player;							 \
	player->SetGrounded(false);								 \
	stack->jumpingTimeStack = 0.0f;							 \
	return PushdownResult::Push;							 \
}															 \

#define M_DOUBLEJUMP                                         \
if (lastInput.buttons[jump])								 \
{															 \
	*newState = new DoubleJump();							 \
	(*newState)->userdata = player;							 \
	stack->jumpingTimeStack = 0.0f;							 \
	return PushdownResult::Push;							 \
}															 \

#define M_DASH                                               \
if (lastInput.buttons[dash] and stack->dashCooldown < 0)	 \
{                                                            \
*newState = new Dash();										 \
(*newState)->userdata = player;								 \
stack->dashingTimeStack = 0.0f;								 \
return PushdownResult::Push;								 \
}															 \

#define M_WALK                                               \
if (lastInput.movement_direction != Vector2())               \
{															 \
	*newState = new Walk();									 \
	(*newState)->userdata = player;							 \
	return PushdownResult::Push;							 \
}															 \

class Die :public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {
		M_INIT
		if(stack->deathTimeStack>RESPAWN_DURA)
		{
			stack->respawnCooldown = RESPAWN_CD;
			return PushdownResult::Pop;
		}
		player->GetPhysicsXObject()->CMove(PxVec3(0, -9.8f, 0) * 0.1f);
		return PushdownResult::NoChange;
	}
};
class Dash :public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {
		M_INIT
		if (stack->dashingTimeStack > DASH_DURA)
		{
			stack->dashCooldown = DASH_CD;
			return PushdownResult::Pop;
		}
		M_DIE
		player->Dash(dt);
		return PushdownResult::NoChange;
	}
};
class DoubleJump :public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {
		M_INIT
		if (stack->jumpingTimeStack> DOUBLE_DURA)
		{
			return PushdownResult::Pop;
		}
 		M_DASH
		M_DIE
		player->Jump(dt);
		player->Move();
		return PushdownResult::NoChange;
	}

};
class StandingJump :public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {
		M_INIT
		if (stack->jumpingTimeStack > JUMP_DURA)
		{
			return PushdownResult::Pop;
		}
		M_DOUBLEJUMP
		M_DASH
		M_DIE
		player->Jump(dt);
		player->Move();
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
		player->Move();
		player->GetPhysicsXObject()->CMove(PxVec3(0,-9.8f,0)*0.1f);
		return PushdownResult::NoChange;
	}
};
class Idle :public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState **newState) override {
		M_INIT
		M_STANDINGJUMP
		M_DASH
		M_WALK
		M_DIE
		player->GetPhysicsXObject()->CMove(PxVec3(0,-9.8f,0)*0.1f);
		return PushdownResult::NoChange;
	}
};

//class Respawn : public PushdownState {
//	PushdownResult OnUpdate(float dt,
//		PushdownState** newState) override {
//		Player* player = static_cast<Player*>(userdata);
//		if (player->IsDead() == true) {
//			player->RespawnCooldown();
//			return PushdownResult::Pop;
//		}
//		player->Respawn();
//		return PushdownResult::NoChange;
//	}
//};




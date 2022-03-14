#pragma once
#include "../CSC8503/CSC8503Common/PushdownState.h"
#include "Player.h"
using namespace NCL;
using namespace CSC8503;
class Dash :public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {
		Player* player = static_cast<Player*>(userdata);
		if (player->GetDashingTimeStack()>0.2f)
		{
			player->DashCooldown();
			return PushdownResult::Pop;
		}
		player->Dash(dt);
		return PushdownResult::NoChange;
	}
};
class DoubleJump :public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {
		Player* player = static_cast<Player*>(userdata);
		Input lastInput = player->GetLastInput();
		if (player->GetJumpingTimeStack()>1.5f*3.15f)
		{
			return PushdownResult::Pop;
		}
		if (lastInput.buttons[dash] and player->DashAlready()) {
			*newState = new Dash();
			(*newState)->userdata = player;
			player->SetDashingTimeStack(0.0f);
			return PushdownResult::Push;
		}
		player->Jump(dt);
		player->Move();
		return PushdownResult::NoChange;
	}

};
class StandingJump :public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {
		Player* player = static_cast<Player*>(userdata);
		Input lastInput = player->GetLastInput();
		if (player->GetJumpingTimeStack()>3.2f)
		{
			return PushdownResult::Pop;
		}
		if (lastInput.buttons[jump]) {
			*newState = new DoubleJump();
			(*newState)->userdata = player;
			player->SetJumpingTimeStack(0.0f);
			return PushdownResult::Push;
		}
		if (lastInput.buttons[dash]and player->DashAlready()) {
			*newState = new Dash();
			(*newState)->userdata = player;
			player->SetDashingTimeStack(0.0f);
			return PushdownResult::Push;
		}
		player->Jump(dt);
		player->Move();
		return PushdownResult::NoChange;
	}
};
class Walk :public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {
		Player* player = static_cast<Player*>(userdata);
		Input lastInput = player->GetLastInput();
		if (player->GetLastInput().movement_direction == Vector2())
			return PushdownResult::Pop;
		if (lastInput.buttons[jump])
		{
			*newState = new StandingJump();
			(*newState)->userdata = player;
			player->SetGrounded(false);
			player->SetJumpingTimeStack(0.0f);
			return PushdownResult::Push;
		}
		if (lastInput.buttons[dash] and player->DashAlready()) {
			*newState = new Dash();
			(*newState)->userdata = player;
			player->SetDashingTimeStack(0.0f);
			return PushdownResult::Push;
		}
		player->Move();
		player->GetPhysicsXObject()->CMove(PxVec3(0,-9.8f,0)*0.1f);
		return PushdownResult::NoChange;
	}
};
class Idle :public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState **newState) override {
		Player* player = static_cast<Player*>(userdata);
		Input lastInput = player->GetLastInput();
		if (lastInput.buttons[jump])
		{
			*newState = new StandingJump();
			(*newState)->userdata = player;
			player->SetGrounded(false);
			player->SetJumpingTimeStack(0.0f);
			return PushdownResult::Push;
		}
		if (lastInput.buttons[dash] and player->DashAlready()) {
			*newState = new Dash();
			(*newState)->userdata = player;
			player->SetDashingTimeStack(0.0f);
			return PushdownResult::Push;
		}
		if (lastInput.movement_direction not_eq Vector2()) {
			*newState = new Walk();
			(*newState)->userdata = player;
			return PushdownResult::Push;
		}
		player->GetPhysicsXObject()->CMove(PxVec3(0,-9.8f,0)*0.1f);
		return PushdownResult::NoChange;
	}
};






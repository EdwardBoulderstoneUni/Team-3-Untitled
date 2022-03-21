#pragma once
#include "../CSC8503/CSC8503Common/PushdownState.h"
#include "Player.h"
using namespace NCL;
using namespace CSC8503;
class Shooting :public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {
		Player* player = static_cast<Player*>(userdata);
		Input lastInput = player->GetLastInput();
		if (lastInput.buttons[attack])
		{
			player->Openfire();
		}
		if (lastInput.buttons[reload])
		{
			player->Reload();
		}
		return PushdownResult::NoChange;
	}
};
//class Roloading :public PushdownState {
//	PushdownResult OnUpdate(float dt,
//		PushdownState** newState) override {
//		Player* player = static_cast<Player*>(userdata);
//		return PushdownResult::NoChange;
//	}
//};
class Hold :public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {
		Player* player = static_cast<Player*>(userdata);
		Input lastInput = player->GetLastInput();
		if (lastInput.buttons[attack])
		{
			*newState = new Shooting();
			(*newState)->userdata = player;
			return PushdownResult::Push;
		}
		return PushdownResult::NoChange;
	}
};
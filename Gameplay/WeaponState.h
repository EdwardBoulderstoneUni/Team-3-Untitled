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


class Shooting :public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {
		M_INIT
		if (lastInput.buttons[attack])
		{
			if (playerPro->ammo > 0) {
				YiEventSystem::GetMe()->PushEvent(PLAYER_OPEN_FIRE, player->GetWorldID());
				playerPro->ammo--;
			}
		}
		if (lastInput.buttons[reload])
		{
			if (playerPro->ammo >= 0 && playerPro->ammo < playerPro->maxAmmo) {
				playerPro->ammo = playerPro->maxAmmo;
			}
		}
		return PushdownResult::NoChange;
	}
};

class Hold :public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {
		M_INIT
		if (lastInput.buttons[attack])
		{
			*newState = new Shooting();
			(*newState)->userdata = player;
			return PushdownResult::Push;
		}
		return PushdownResult::NoChange;
	}
};
#pragma once
#include "Player.h"
#include "ePlayerRole.h"

class Ability {
public:
	Ability();
	~Ability();
private:
	float cooldown;
	float range;
	float startUp;
	float endLag;

	bool isFiring;
	PlayerRole colour;
};
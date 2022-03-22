#pragma once
#include "Cube.h"
#include "ePlayerRole.h"
#include "Player.h"
#define GRENADE_DURA 5.0f
class Grenade : public Cube {
public:
	Grenade(Player& player);
	virtual ~Grenade();
	void SetUp() override;
	virtual void Update(float dt)override;
	float GetDamage() {
		return damage;
	}
	int GetPlayerID() { return playerID; }
private:
	float timeStack = 0;
	float damage = 0;
	int playerID;
};


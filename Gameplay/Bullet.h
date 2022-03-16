#pragma once
#include "Sphere.h"
#include "ePlayerRole.h"
#include "Player.h"
#define BULLET_DURA 5.0f
class Bullet : public Sphere {
public:
	Bullet(Player& player);
	void SetUp() override;
	virtual void Update(float dt)override;
	float GetDamage(){
		return damage;
	}
	int GetShooterID() { return shooterID; }
private:
	float timeStack = 0;
	float damage = 0;
	int shooterID;
};

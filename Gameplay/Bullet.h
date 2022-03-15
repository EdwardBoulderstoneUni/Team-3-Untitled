#pragma once
#include "Sphere.h"
#include "ePlayerRole.h"

class Bullet : public Sphere {
public:
	Bullet(GameObjectType type, PlayerRole colour);
	void SetUp() override;
	virtual void Update(float dt)override;
	float GetDamage(){
		damage;
	}
private:
	float timeStack = 0;
	float damage = 0;
};

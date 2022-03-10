#pragma once
#include "Sphere.h"
#include "ePlayerRole.h"

class Bullet : public Sphere {
public:
	Bullet(GameObjectType type, PlayerRole colour);

	float GetDamage(){
		damage;
	}

private:
	float damage = 0;
};

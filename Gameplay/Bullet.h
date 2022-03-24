#pragma once
#include "Sphere.h"
#include "ePlayerRole.h"
#include "Player.h"
class Bullet : public Sphere {
public:
	Bullet(Player& player);
	void SetUp() override;
	virtual void Update(float dt)override;
	float GetDamage(){
		return damage;
	}
	int GetShooterID() { return shooterID; }
	void OnCollisionBegin(GameObject* otherObject, Vector3 point) override;
private:
	float timeStack = 0;
	float damage = 0;
	int shooterID;
};

#include "Bullet.h"
#include "../CSC8503/GameTech/YiEventSystem.h"
Bullet::Bullet(Player& player) {
	switch (player.type)
	{
	case GameObjectType_team1:
		type = GameObjectType_team1Bullet;
		break;
	case GameObjectType_team2:
		type = GameObjectType_team2Bullet;
		break;
	}
	damage = player.GetPlayerPro()->damage;
	shooterID = player.GetWorldID();
}
Bullet::~Bullet()
{
}
void Bullet::SetUp() {
	Sphere::SetUp();
}

void Bullet::Update(float dt)
{
	ComponentGameObject::Update(dt);
	timeStack += dt;
	if(timeStack> BULLET_DURA)
		YiEventSystem::GetMe()->PushEvent(OBJECT_DELETE,GetWorldID());
}

void Bullet::OnRed() {
	if (isOnRed == true) {
		damage = damage * 2;
	}
}
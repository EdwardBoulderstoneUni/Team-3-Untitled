#include "Bullet.h"
#include "../CSC8503/GameTech/YiEventSystem.h"
Bullet::Bullet(Player& player) {
	if(player.type==GameObjectType::GameObjectType_team1)
		this->type = GameObjectType::GameObjectType_team1Bullet;
	if (player.type == GameObjectType::GameObjectType_team2)
		this->type = GameObjectType::GameObjectType_team2Bullet;

	switch (player.GetRole()) {
	case PlayerRole_red:
		damage = 5;
		break;
	case PlayerRole_green:
		damage = 3;
		break;
	case PlayerRole_blue:
		damage = 4;
		break;
	default:
		damage = 0;
		break;
	}
	shooterID = player.GetWorldID();
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
#include "Bullet.h"
#include "../CSC8503/GameTech/YiEventSystem.h"
Bullet::Bullet(GameObjectType type, PlayerRole colour) {
	this->type = type;

	switch (colour) {
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
}
void Bullet::SetUp() {
	Sphere::SetUp();

}

void Bullet::Update(float dt)
{
	ComponentGameObject::Update(dt);
	timeStack += dt;
	if(timeStack>1.0f)
		YiEventSystem::GetMe()->PushEvent(OBJECT_DELETE,GetWorldID());
}

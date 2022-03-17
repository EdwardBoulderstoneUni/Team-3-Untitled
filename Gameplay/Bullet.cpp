#include "Bullet.h"
#include "../CSC8503/GameTech/YiEventSystem.h"
void _Handle(const EVENT* pEvent, DWORD64 dwOwnerData)
{
	Bullet* bullet = (Bullet*)dwOwnerData;
	bullet->GetPhysicsXObject()->SetLinearVelocity(Vector3(0,1,0));
}
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
	YiEventSystem::GetMe()->RegisterEventHandle("OPEN_FIRE", _Handle,(DWORD64)this);
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
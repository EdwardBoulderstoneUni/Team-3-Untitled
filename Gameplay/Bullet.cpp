#include "Bullet.h"
#include "../CSC8503/GameTech/YiEventSystem.h"
#include "../Paint/PaintableSurface.h"
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
	if(timeStack>1.0f)
		YiEventSystem::GetMe()->PushEvent(OBJECT_DELETE,GetWorldID());
}

void Bullet::OnCollisionBegin(GameObject* otherObject, Vector3 point)
{
	if (otherObject->type == GameObjectType_floor || otherObject->type == GameObjectType_wall)
	{
		const auto local_collision_point = point - otherObject->GetTransform().GetPosition();
		const auto floor_or_wall = dynamic_cast<ComponentGameObject*>(otherObject);
		dynamic_cast<PaintableSurface*>(floor_or_wall->GetComponent(Component::Paint))->Paint(local_collision_point);
		YiEventSystem::GetMe()->PushEvent(OBJECT_DELETE, worldID);
	}
}

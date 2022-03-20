#include "Grenade.h"
#include "../CSC8503/GameTech/YiEventSystem.h"

Grenade::Grenade(Player& player) {
	switch (player.type)
	{
	case GameObjectType_team1:
		type = GameObjectType_team1Grenade;
		break;
	case GameObjectType_team2:
		type = GameObjectType_team2Grenade;
		break;
	}
	damage = player.GetPlayerPro()->grenadeDmg;
	playerID = player.GetWorldID();
}
Grenade::~Grenade() {

}
void Grenade::SetUp() {
	Cube::SetUp();
}
void Grenade::Update(float dt)
{
	ComponentGameObject::Update(dt);
	timeStack += dt;
	if (timeStack > GRENADE_DURA)
		YiEventSystem::GetMe()->PushEvent(OBJECT_DELETE, GetWorldID());
}
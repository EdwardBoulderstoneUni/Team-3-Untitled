#include "Bullet.h"

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
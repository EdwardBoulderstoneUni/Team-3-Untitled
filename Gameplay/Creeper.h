#pragma once
#include "ComponentGameObject.h"
#include "../CSC8503/CSC8503Common/PushdownMachine.h"
#include "../CSC8503/CSC8503Common/GameWorld.h"
#include "Player.h"
class Creeper :public ComponentGameObject {
public:
	Creeper(GameWorld& gameWorld);
	virtual ~Creeper();
	virtual void SetUp();
	virtual void Update(float dt);
	Player* target;
	Vector3 forward;
	float speed = 0.2f;
private:
	float visualField = 50.0f;
	GameWorld& world;
	void SetupStateMachine();
	PushdownMachine* stateMachine;
};
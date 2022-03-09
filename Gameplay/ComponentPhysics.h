#pragma once
#include "Component.h"
#include "../CSC8503/CSC8503Common/Transform.h"
#include "../CSC8503/CSC8503Common/PhysicsXObject.h"
class ComponentPhysics : public Component {
public:
	void Init() override;
	void Update(float dt) override;
	PhysicsXObject* phyObj;
};
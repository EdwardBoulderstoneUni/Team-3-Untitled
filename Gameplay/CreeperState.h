#pragma once
#pragma once
#include "../CSC8503/CSC8503Common/PushdownState.h"
#include "Creeper.h"

using namespace NCL;
using namespace CSC8503;
#define M_INIT										\
Creeper* creeper = static_cast<Creeper*>(userdata);	\
PhysicsXObject* phyobj =creeper->GetPhysicsXObject(); \
Transform trans=creeper->GetTransform();            \

#define M_GRAVITY if (phyobj)creeper->GetPhysicsXObject()->CMove(PxVec3(0, -9.8f, 0) * 0.1f);\
else trans.SetPosition(Vector3(0,-9.8f,0)*0.1f);\

class Creep:public PushdownState{
	PushdownResult OnUpdate(float dt,
	PushdownState** newState) override {
		M_INIT
			if (not creeper->target) {
				return PushdownResult::Pop;
			}
		if (phyobj) phyobj->CMove(PhysXConvert::Vector3ToPxVec3(creeper->forward) * creeper->speed);
		else trans.SetPosition((creeper->forward) * creeper->speed);
		M_GRAVITY
			return PushdownResult::NoChange;
	}
};
class _Idle :public PushdownState {
	PushdownResult OnUpdate(float dt,
		PushdownState** newState) override {
		M_INIT
			if (creeper->target) {
				*newState = new Creep();
				(*newState)->userdata = creeper;
				return PushdownResult::Push;
			}
		M_GRAVITY
			return PushdownResult::NoChange;
	}
};




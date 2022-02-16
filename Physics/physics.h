#pragma once
#include "PxPhysicsAPI.h"
#include "../CSC8503/GameTech/TutorialGame.h"

namespace physx{
	class physics
	{
	public:
		physics();
		~physics();
	private:

		
		void AddFloorToWorld(const PxTransform& t, PxVec3 fullExtents);
		void initPhysics();
		void stepPhysics(bool /*interactive*/);
		void cleanupPhysics(bool /*interactive*/);
		int snippetMain(int flag, const char* const*, TutorialGame* t, float dt);
	};
}

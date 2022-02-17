#pragma once
#include "PxPhysicsAPI.h"
#include "../CSC8503/GameTech/TutorialGame.h"

namespace physx {
	class Physics
	{
	public:
		Physics();
		~Physics();

		void Update(float dt);
	private:

		PxRigidDynamic* createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity = PxVec3(0));

		void createStack(const PxTransform& t, PxU32 size, PxReal halfExtent);

		void initPhysics(bool interactive);

		void stepPhysics(bool /*interactive*/);

		void cleanupPhysics(bool /*interactive*/);


	};
}

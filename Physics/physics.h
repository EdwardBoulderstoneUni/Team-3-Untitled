#pragma once
#include "PxPhysicsAPI.h"
#include "../CSC8503/GameTech/TutorialGame.h"

namespace NCL {
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

		PxDefaultAllocator			gAllocator;
		PxDefaultErrorCallback		gErrorCallback;
		PxFoundation* gFoundation = NULL;
		PxPhysics* gPhysics = NULL;
		PxDefaultCpuDispatcher* gDispatcher = NULL;
		PxScene* gScene = NULL;
		/* TODO
			Make able to faciliate more materials in the future
		*/
		PxMaterial* gMaterial = NULL;
		PxPvd* gPvd = NULL;



	};
}

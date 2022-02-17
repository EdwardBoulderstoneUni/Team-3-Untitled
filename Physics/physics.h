#pragma once
#include "PxPhysicsAPI.h"
#include "../CSC8503/GameTech/TutorialGame.h"

namespace NCL {
	class Physics
	{
	public:
		Physics();
		~Physics();

		void update(float dt);
	private:
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

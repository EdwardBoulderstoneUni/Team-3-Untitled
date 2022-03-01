#pragma once
#include "../CSC8503Common/GameWorld.h"
#include "../../include/PhysX/PxPhysicsAPI.h"
#include "../CSC8503Common/CollisionVolume.h"
using namespace physx;
using namespace NCL;
class PhysicsXSystem {
		public:
			PhysicsXSystem(GameWorld& g);
			~PhysicsXSystem();
			void initPhysics();
			void Update(float dt);
			void cleanupPhysics();
			void addActor(GameObject& actor);

			PxTransform& parseTransform(Transform transform);
			PxShape& parseVolume(Vector3 volume);
		protected:
			void getActorsPose(PxRigidActor** actors, const PxU32 numActors);
			void updateObjects(PxTransform pose, PxU32 count);
			PxDefaultAllocator		gAllocator; 
			PxDefaultErrorCallback	gErrorCallback;

			PxFoundation* gFoundation = NULL;
			PxPhysics* gPhysics = NULL;
			PxDefaultCpuDispatcher* gDispatcher = NULL;
			PxScene* gScene = NULL;
			PxMaterial* gMaterial = NULL;
			PxPvd* gPvd = NULL;

			GameWorld& gameWorld;
			float dTOffset;	
		};
	
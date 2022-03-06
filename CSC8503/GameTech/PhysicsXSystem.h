#pragma once
#include "../CSC8503Common/GameWorld.h"
#include "PhysXAPI.h"
using namespace physx;
using namespace NCL;

class PhysicsXSystem {
		public:
			PhysicsXSystem(GameWorld& g);
			~PhysicsXSystem();
			void initPhysics();
			void Update(float dt);
			void addDynamicActor(GameObject& actor);
			void addStaticActor(GameObject& actor);

			bool raycast(Vector3 origin,Vector3 dir,float maxdis,PxRaycastBuffer& hit);
			bool raycastCam(Camera& camera, float maxdis,PxRaycastBuffer& hit);
			void addActors(std::vector<GameObject*>& actors);
		protected:
		
			void getActorsPose(PxRigidActor** actors, const PxU32 numActors);

			Vector3 Unproject(const Vector3& screenPos, const Camera& cam);
			Matrix4 GenerateInverseView(const Camera& c);
			Matrix4 GenerateInverseProjection(float aspect, float fov, float nearPlane, float farPlane);

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
	
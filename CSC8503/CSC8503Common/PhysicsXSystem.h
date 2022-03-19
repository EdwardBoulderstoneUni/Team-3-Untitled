#pragma once
#include "../CSC8503Common/GameWorld.h"
#include "../../include/PhysX/PxRigidActor.h"
#include "../../include/PhysX/PxQueryReport.h"
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
			void SyncGameObjs();

			bool raycast(NCL::Maths::Vector3 origin, NCL::Maths::Vector3 dir,float maxdis,PxRaycastBuffer& hit);
			bool raycastCam(Camera& camera, float maxdis,PxRaycastBuffer& hit);
			
		protected:
		
			void getActorsPose(PxRigidActor** actors, const PxU32 numActors);

			NCL::Maths::Vector3 Unproject(const NCL::Maths::Vector3& screenPos, const Camera& cam);
			NCL::Maths::Matrix4 GenerateInverseView(const Camera& c);
			NCL::Maths::Matrix4 GenerateInverseProjection(float aspect, float fov, float nearPlane, float farPlane);
			GameWorld& gameWorld;
			float dTOffset;	
		};
	
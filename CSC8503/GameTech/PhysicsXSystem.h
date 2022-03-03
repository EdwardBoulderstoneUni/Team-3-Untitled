#pragma once
#include "../CSC8503Common/GameWorld.h"
#include "../../include/PhysX/PxPhysicsAPI.h"
//#include "../CSC8503Common/CollisionVolume.h"

using namespace physx;
using namespace NCL;
struct GeometryData
{
	GeometryData() :type(), data(){}
	enum Type
	{
		Sphere,
		Capsule,
		Box,
		ConvexMesh,
		TriangleMesh,
		HeightField,
	};
	Type type;
	union Data
	{
		Data(){}
		struct SphereData
		{
			SphereData(float newRadius) {
				radius = newRadius;
			}
			float radius;
		}sphereData;

		struct CapsuleData
		{
			CapsuleData(float newRadius,float newhalfHeight) {
				radius = newRadius;
				halfHeight=newhalfHeight;
			}
			float radius;
			float halfHeight;
		}capsuleData;

		struct BoxData
		{
			BoxData(Vector3 vector) {
				halfx = vector.x;
				halfy = vector.y;
				halfz = vector.z;
			}
			float halfx;
			float halfy;
			float halfz;
		}boxData;

	}data;

};
class PhysicsXSystem {
		public:
			PhysicsXSystem(GameWorld& g);
			~PhysicsXSystem();
			void initPhysics();
			void Update(float dt);
			void cleanupPhysics();
			void addDynamicActor(GameObject& actor);
			void addStaticActor(GameObject& actor);
			PxTransform& parseTransform(Transform transform);

			GeometryData::Data::BoxData AABBToBoxData(const Vector3& halfDims);

			PhysicsXObject* createPhysicsXObject(Transform transform,GeometryData geoData);
			
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
	
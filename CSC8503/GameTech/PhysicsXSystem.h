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
			void cleanupPhysics(PxRigidActor** actors, const PxU32 numActors);
			void addDynamicActor(GameObject& actor, GeometryData geoData);
			void addStaticActor(GameObject& actor, GeometryData geoData);
			GeometryData createBoxGeo(const Vector3 hfExtents);
			GeometryData createBoxGeo(float x, float y, float z);
			GeometryData createSphereGeo(float radius);
			GeometryData createCapsuleGeo(float radius, float hfHeight);
			bool raycast(Vector3 origin,Vector3 dir,float maxdis,PxRaycastBuffer& hit);
			bool raycastCam(Camera& camera, float maxdis,PxRaycastBuffer& hit);
		protected:
			PxTransform& parseTransform(Transform transform);

			GeometryData::Data::BoxData AABBToBoxData(const Vector3& halfDims);

			PhysicsXObject* createPhysicsXObject(Transform transform, GeometryData geoData);
			
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
	
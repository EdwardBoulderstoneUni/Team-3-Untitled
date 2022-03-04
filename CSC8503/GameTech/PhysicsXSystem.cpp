#include "PhysicsXSystem.h"
#include <vector>
#define PVD_HOST "127.0.0.1"
#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}
#define MAX_NUM_ACTOR_SHAPES 128

using namespace physx;
PhysicsXSystem::PhysicsXSystem(GameWorld & g):gameWorld(g)
{
	dTOffset = 0.0f;
	initPhysics();
}

PhysicsXSystem::~PhysicsXSystem()
{
	PX_RELEASE(gScene);
	PX_RELEASE(gDispatcher);
	PX_RELEASE(gPhysics);
	if (gPvd)
	{
		PxPvdTransport* transport = gPvd->getTransport();
		gPvd->release();	gPvd = NULL;
		PX_RELEASE(transport);
	}
	PX_RELEASE(gFoundation);
}

void PhysicsXSystem::initPhysics()
{
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
}

void PhysicsXSystem::Update(float dt)
{
	float mStepSize = 1.0f / 60.0f;
	dTOffset += dt;
	if (dTOffset < mStepSize)return;
	dTOffset -= mStepSize;
	gScene->simulate(mStepSize);
	gScene->fetchResults(true);
	PxScene* scene;
	PxGetPhysics().getScenes(&scene, 1);
	PxU32 nbActors = scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
	if (nbActors)
	{
		std::vector<PxRigidActor*> actors(nbActors);
		scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
		getActorsPose(&actors[0], static_cast<PxU32>(actors.size()));
		cleanupPhysics(&actors[0], static_cast<PxU32>(actors.size()));
	}
	
}

void PhysicsXSystem::cleanupPhysics(PxRigidActor** actors, const PxU32 numActors)
{
	/*for (PxU32 i = 0; i < numActors; i++)
	{
		const bool sleeping = actors[i]->is<PxRigidDynamic>() ? actors[i]->is<PxRigidDynamic>()->isSleeping() : false;
		GameObject* obj = (GameObject*)actors[i]->userData;
		obj->GetPhysicsXObject()->ClearForces();

	}*/
}

void PhysicsXSystem::addDynamicActor(GameObject& actor, GeometryData geoData)
{
	PhysicsXObject* phyObj = createPhysicsXObject(actor.GetTransform(), geoData);
	actor.SetPhysicsXObject(phyObj);
	if (phyObj == nullptr)return;

	PxRigidDynamic* body = gPhysics->createRigidDynamic(phyObj->GetTransform());
	body->attachShape(phyObj->GetVolume());
	PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);

	body->userData = &actor;
	phyObj->SetRigidBody(body);
	gScene->addActor(*body);
}

void PhysicsXSystem::addStaticActor(GameObject& actor, GeometryData geoData)
{
	PhysicsXObject* phyObj = createPhysicsXObject(actor.GetTransform(), geoData);
	actor.SetPhysicsXObject(phyObj);
	if (phyObj == nullptr)return;
	PxRigidStatic *body=gPhysics->createRigidStatic(phyObj->GetTransform());
	body->attachShape(phyObj->GetVolume());

	body->userData = &actor;
	gScene->addActor(*body);
}

void PhysicsXSystem::getActorsPose(PxRigidActor** actors, const PxU32 numActors)
{
	PxShape* shapes[MAX_NUM_ACTOR_SHAPES];
	for (PxU32 i = 0; i < numActors; i++)
	{
		const PxU32 nbShapes = actors[i]->getNbShapes();
		PX_ASSERT(nbShapes <= MAX_NUM_ACTOR_SHAPES);
		actors[i]->getShapes(shapes, nbShapes);
		const bool sleeping = actors[i]->is<PxRigidDynamic>() ? actors[i]->is<PxRigidDynamic>()->isSleeping() : false;
		for (PxU32 j = 0; j < nbShapes; j++)
		{
			const PxTransform shapePose(PxShapeExt::getGlobalPose(*shapes[j], *actors[i]));
			const PxGeometryHolder h = shapes[j]->getGeometry();
			GameObject* obj=(GameObject*)actors[i]->userData;
			obj->GetTransform().SetPosition(Vector3(shapePose.p.x, shapePose.p.y, shapePose.p.z));
			obj->GetTransform().SetOrientation(Quaternion(shapePose.q.x, shapePose.q.y, shapePose.q.z,
				shapePose.q.w));
		}
	}
}

PxTransform& PhysicsXSystem::parseTransform(Transform transform)
{
	Vector3 positon = transform.GetPosition();
	Quaternion quat = transform.GetOrientation();
	return PxTransform(PxVec3(positon.x, positon.y, positon.z), PxQuat(quat.x, quat.y, quat.z, quat.w));
}


PhysicsXObject* PhysicsXSystem::createPhysicsXObject(Transform transform,GeometryData geoData)
{
	PxShape* shape = nullptr;
	
	switch (geoData.type)
	{
	case GeometryData::Type::Sphere:
	{
		shape = gPhysics->createShape(PxSphereGeometry(geoData.data.sphereData.radius), *gMaterial);
		break;
	}

	case GeometryData::Type::Capsule:
	{
		shape = gPhysics->createShape(PxCapsuleGeometry(geoData.data.sphereData.radius,
			geoData.data.capsuleData.halfHeight), *gMaterial);
		break;
	}

	case GeometryData::Type::Box:
	{
		shape = gPhysics->createShape(PxBoxGeometry(geoData.data.boxData.halfx,
			geoData.data.boxData.halfy,
			geoData.data.boxData.halfz), *gMaterial);
		break;
	}

	default:
		break;
	}
	if (shape == nullptr)return nullptr;
	else return new PhysicsXObject(parseTransform(transform), *shape);
}

GeometryData PhysicsXSystem::createBoxGeo(const Vector3 hfExtents)
{
	GeometryData geo = GeometryData();
	geo.type = GeometryData::Box;
	geo.data.boxData.halfx = hfExtents.x;
	geo.data.boxData.halfy = hfExtents.y;
	geo.data.boxData.halfz = hfExtents.z;
	return geo;
}

GeometryData PhysicsXSystem::createBoxGeo(float x, float y, float z)
{
	GeometryData geo = GeometryData();
	geo.type = GeometryData::Box;
	geo.data.boxData.halfx = x;
	geo.data.boxData.halfy = y;
	geo.data.boxData.halfz = z;
	return geo;
}

GeometryData PhysicsXSystem::createSphereGeo(float radius)
{
	GeometryData geo = GeometryData();
	geo.type = GeometryData::Sphere;
	geo.data.sphereData.radius = radius;
	return geo;
}

GeometryData PhysicsXSystem::createCapsuleGeo(float radius, float hfHeight)
{
	GeometryData geo = GeometryData();
	geo.type = GeometryData::Capsule;
	geo.data.capsuleData.radius = radius;
	geo.data.capsuleData.halfHeight = hfHeight;
	return geo;
}

GeometryData::Data::BoxData AABBToBoxData(const Vector3& halfDims)
{
	return GeometryData::Data::BoxData(halfDims);
}


#include "PhysicsXSystem.h"
#include <vector>
#define PVD_HOST "127.0.0.1"
#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}
#define MAX_NUM_ACTOR_SHAPES 128
#define PhysX_DEBUG
using namespace physx;
PhysicsXSystem::PhysicsXSystem(GameWorld & g):gameWorld(g)
{
	dTOffset = 0.0f;
	initPhysics();
}

PhysicsXSystem::~PhysicsXSystem()
{
	gPhysics->release();
	gPvd->release();
}

void PhysicsXSystem::initPhysics()
{
#ifdef PhysX_DEBUG
	bool recordMemoryAllocations = true;
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	if (!gFoundation)
		std::cout << "PxCreateFoundation failed!" << std::endl;

	gPvd = PxCreatePvd(*gFoundation);
	if (!gPvd)
		std::cout << "gPvd failed!" << std::endl;
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), 
		recordMemoryAllocations, gPvd);
	if (!gPhysics)
		std::cout << "gPhysics failed!" << std::endl;

	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	gScene = gPhysics->createScene(sceneDesc);
	if (!gScene)
		std::cout << "gScene failed!" << std::endl;
	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
#else
	gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	gScene = gPhysics->createScene(sceneDesc);

	PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
#endif // PhysX_DEBUG

	
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
	}
}

void PhysicsXSystem::cleanupPhysics()
{
	PX_RELEASE(gScene);
	PX_RELEASE(gDispatcher);
	PX_RELEASE(gPhysics);
	PX_RELEASE(gFoundation);
}

void PhysicsXSystem::addDynamicActor(GameObject& actor)
{
	PhysicsXObject* phyObj = actor.GetPhysicsXObject();
	if (phyObj == nullptr)return;

	PxRigidDynamic* body = gPhysics->createRigidDynamic(phyObj->GetTransform());
	body->attachShape(phyObj->GetVolume());
	PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);

	body->userData = &actor;
	actor.GetPhysicsXObject()->SetRigidDynamic(body);
	gScene->addActor(*body);
}

void PhysicsXSystem::addStaticActor(GameObject& actor)
{
	PhysicsXObject* phyObj = actor.GetPhysicsXObject();
	if (phyObj == nullptr)return;
	PxRigidStatic *body=gPhysics->createRigidStatic(phyObj->GetTransform());
	body->attachShape(phyObj->GetVolume());

	body->userData = &actor;
	//actor.GetPhysicsXObject()->SetRigidDynamic(body);
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
			//updateObjects(shapePose,i);
		}
	}
}

void PhysicsXSystem::updateObjects(PxTransform pose, PxU32 count)
{
	
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

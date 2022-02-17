#include "Physics.h"
#include "PxPhysicsAPI.h"

//#include "SnippetUtils.h"
#define PX_RELEASE(x)      if(x)  { x->release(); x = NULL;  }
#define PVD_HOST "127.0.0.1"	//Set this to the IP address of the system running the PhysX Visual Debugger that you want to connect to.

using namespace physx;

PxReal stackZ = 10.0f;

TutorialGame* game;

Physics::Physics() {

}
Physics::~Physics()
{

}

void Physics::Update(float dt) {

}

PxRigidDynamic* createDynamic(const PxTransform& t, const PxGeometry& geometry, const PxVec3& velocity = PxVec3(0))
{
	PxRigidDynamic* dynamic = PxCreateDynamic(*gPhysics, t, geometry, *gMaterial, 10.0f);
	dynamic->setAngularDamping(0.5f);
	dynamic->setLinearVelocity(velocity);
	gScene->addActor(*dynamic);
	return dynamic;
}

void Physics::createStack(const PxTransform& t, PxU32 size, PxReal halfExtent)
{
	
}

void Physics::initPhysics(bool interactive)
{
		
}

void Physics::stepPhysics(bool /*interactive*/)
{
		
}

void Physics::cleanupPhysics(bool /*interactive*/)
{
		
}



//	void AddFloorToWorld(const PxTransform& t, PxVec3 fullExtents) {
//		GameObject* floor;
//		PxRigidStatic* body;
//		PxShape* shape = gPhysics->createShape(PxBoxGeometry(fullExtents.x / 2, fullExtents.y / 2, fullExtents.z / 2), *gMaterial);
//		PxTransform localTm(t.p);
//		body = gPhysics->createRigidStatic(t.transform(localTm));
//		body->attachShape(*shape);
//		gScene->addActor(*body);
//		floor = game->AddPxFloorToWorld(new GameObject(), body, Vector3(t.p.x, t.p.y, t.p.z), Vector3(fullExtents.x / 2, fullExtents.y / 2, fullExtents.z / 2));
//	}
//
//	void initPhysics() {
//		gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
//
//		gPvd = PxCreatePvd(*gFoundation);
//		PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
//		gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);
//
//		gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);
//
//		PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
//		sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
//		gDispatcher = PxDefaultCpuDispatcherCreate(2);
//		sceneDesc.cpuDispatcher = gDispatcher;
//		sceneDesc.filterShader = PxDefaultSimulationFilterShader;
//		gScene = gPhysics->createScene(sceneDesc);
//
//		PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
//		if (pvdClient) {
//			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
//			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
//			pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
//		}
//
//		gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
//	}
//
//	void stepPhysics(bool /*interactive*/)
//	{
//		gScene->simulate(1.0f / 60.0f);
//		gScene->fetchResults(true);
//	}
//
//	void cleanupPhysics(bool /*interactive*/)
//	{
//		PX_RELEASE(gScene);
//		PX_RELEASE(gDispatcher);
//		PX_RELEASE(gPhysics);
//		if (gPvd)
//		{
//			PxPvdTransport* transport = gPvd->getTransport();
//			gPvd->release();     gPvd = NULL;
//			PX_RELEASE(transport);
//		}
//		PX_RELEASE(gFoundation);
//
//		std::cout << "World done.\n" << std::endl;
//	}
//
//	int snippetMain(int flag, const char* const*, TutorialGame* t, float dt)
//	{
//#ifdef RENDER_SNIPPET
//		extern void renderLoop();
//		renderLoop();
//#else
//		static const PxU32 frameCount = 100;
//		if (flag) {
//			game = t;
//			initPhysics();
//			AddFloorToWorld(PxTransform(PxVec3(0, -20, 0)), PxVec3(1000.0f, 1, 1000));
//		}
//		stepPhysics(true);
//		cleanupPhysics(false);
//#endif
//
//		return 0;
//	}
//}
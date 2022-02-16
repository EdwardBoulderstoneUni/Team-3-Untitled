#include"physics.h"
#include <ctype.h>
#include "PxPhysicsAPI.h"
#include "SnippetPrint.h"
#include "SnippetPVD.h"
#include "..//CSC8503/GameTech/TutorialGame.h"
//#include "SnippetUtils.h"
#define PX_RELEASE(x)      if(x)  { x->release(); x = NULL;  }

using namespace physx;

PxDefaultAllocator         gAllocator;
PxDefaultErrorCallback     gErrorCallback;

PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;

PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;

PxMaterial* gMaterial = NULL;

PxPvd* gPvd = NULL;

PxReal stackZ = 10.0f;

TutorialGame* game;

physics::physics() {

}
physics::~physics()
{

}


void AddFloorToWorld(const PxTransform& t, PxVec3 fullExtents) {
    GameObject* floor;
    PxRigidStatic* body;
    PxShape* shape = gPhysics->createShape(PxBoxGeometry(fullExtents.x / 2, fullExtents.y / 2, fullExtents.z / 2), *gMaterial);
    PxTransform localTm(t.p);
    body = gPhysics->createRigidStatic(t.transform(localTm));
    body->attachShape(*shape);
    gScene->addActor(*body);
    floor = game-> AddPxFloorToWorld(new GameObject(), body, Vector3(t.p.x, t.p.y, t.p.z), Vector3(fullExtents.x / 2, fullExtents.y / 2, fullExtents.z / 2));
}

void initPhysics() {
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
    if (pvdClient) {
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }

    gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
}

void stepPhysics(bool /*interactive*/)
{
    gScene->simulate(1.0f / 60.0f);
    gScene->fetchResults(true);
}

void cleanupPhysics(bool /*interactive*/)
{
    PX_RELEASE(gScene);
    PX_RELEASE(gDispatcher);
    PX_RELEASE(gPhysics);
    if (gPvd)
    {
        PxPvdTransport* transport = gPvd->getTransport();
        gPvd->release();     gPvd = NULL;
        PX_RELEASE(transport);
    }
    PX_RELEASE(gFoundation);

    printf("World done.\n");
}

int snippetMain(int flag, const char* const*, TutorialGame* t, float dt)
{
#ifdef RENDER_SNIPPET
    extern void renderLoop();
    renderLoop();
#else
    static const PxU32 frameCount = 100;
    if (flag) {
        game = t;
        initPhysics();
        AddFloorToWorld(PxTransform(PxVec3(0, -20, 0)), PxVec3(1000.0f, 1, 1000));
    }
    stepPhysics(true);
    cleanupPhysics(false);
#endif

    return 0;
}

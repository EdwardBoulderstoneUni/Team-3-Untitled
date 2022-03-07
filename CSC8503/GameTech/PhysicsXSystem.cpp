#include "PhysicsXSystem.h"
#include "PhysXConvert.h"
#include "../../Common/Maths.h"
#include <vector>
#define PVD_HOST "127.0.0.1"
#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}
#define MAX_NUM_ACTOR_SHAPES 128

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;
PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
PxMaterial* gMaterial = NULL;
PxPvd* gPvd = NULL;


class ContackCallback :public PxSimulationEventCallback {
	void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) { PX_UNUSED(constraints); PX_UNUSED(count); }
	void onWake(PxActor** actors, PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
	void onSleep(PxActor** actors, PxU32 count) { PX_UNUSED(actors); PX_UNUSED(count); }
	void onTrigger(PxTriggerPair* pairs, PxU32 count) { PX_UNUSED(pairs); PX_UNUSED(count); }
	void onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32) {}
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
	{
		PX_UNUSED((pairHeader));
		GameObject* a = (GameObject*)pairHeader.actors[0]->userData;
		GameObject* b = (GameObject*)pairHeader.actors[1]->userData;
		a->OnCollisionBegin(b);
	}
};
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
PxFilterFlags contactReportFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	PX_UNUSED(attributes0);
	PX_UNUSED(attributes1);
	PX_UNUSED(filterData0);
	PX_UNUSED(filterData1);
	PX_UNUSED(constantBlockSize);
	PX_UNUSED(constantBlock);

	// all initial and persisting reports for everything, with per-point data
	pairFlags = PxPairFlag::eSOLVE_CONTACT | PxPairFlag::eDETECT_DISCRETE_CONTACT
		| PxPairFlag::eNOTIFY_TOUCH_FOUND
		| PxPairFlag::eNOTIFY_TOUCH_PERSISTS
		| PxPairFlag::eNOTIFY_CONTACT_POINTS;
	return PxFilterFlag::eDEFAULT;
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
	//sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	sceneDesc.filterShader = contactReportFilterShader;
	ContackCallback* callback = new ContackCallback;
	sceneDesc.simulationEventCallback = callback;
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
	SyncGameObjs();
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

void PhysicsXSystem::addDynamicActor(GameObject& actor)
{
	PxShape* shape = gPhysics->createShape(*actor.GetPhysicsXObject()->GetVolume(), *gMaterial);
	PxTransform localTm(actor.GetTransform().GetPosition().x,
		actor.GetTransform().GetPosition().y, 
		actor.GetTransform().GetPosition().z);
	PxRigidDynamic* body = gPhysics->createRigidDynamic(localTm);
	PxRigidBodyExt::updateMassAndInertia(*body,10.0f);
	body->attachShape(*shape);
	gScene->addActor(*body);
	body->userData = &actor;
	actor.GetPhysicsXObject()->SetRigActor(body);
	shape->release();
}

void PhysicsXSystem::addStaticActor(GameObject& actor)
{
	PxShape* shape = gPhysics->createShape(*actor.GetPhysicsXObject()->GetVolume(), *gMaterial);
	PxTransform localTm(actor.GetTransform().GetPosition().x,
		actor.GetTransform().GetPosition().y,
		actor.GetTransform().GetPosition().z);
	PxRigidStatic* body = gPhysics->createRigidStatic(localTm);

	body->attachShape(*shape);
	gScene->addActor(*body);
	body->userData = &actor;
	actor.GetPhysicsXObject()->SetRigActor(body);
	shape->release();
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

Vector3 PhysicsXSystem::Unproject(const Vector3& screenPos, const Camera& cam)
{
	Vector2 screenSize = Window::GetWindow()->GetScreenSize();

	float aspect = screenSize.x / screenSize.y;
	float fov = cam.GetFieldOfVision();
	float nearPlane = cam.GetNearPlane();
	float farPlane = cam.GetFarPlane();

	//Create our inverted matrix! Note how that to get a correct inverse matrix,
	//the order of matrices used to form it are inverted, too.
	Matrix4 invVP = GenerateInverseView(cam) * GenerateInverseProjection(aspect, fov, nearPlane, farPlane);

	//Our mouse position x and y values are in 0 to screen dimensions range,
	//so we need to turn them into the -1 to 1 axis range of clip space.
	//We can do that by dividing the mouse values by the width and height of the
	//screen (giving us a range of 0.0 to 1.0), multiplying by 2 (0.0 to 2.0)
	//and then subtracting 1 (-1.0 to 1.0).
	auto clipSpace = Vector4(
		(screenPos.x / screenSize.x) * 2.0f - 1.0f,
		(screenPos.y / screenSize.y) * 2.0f - 1.0f,
		(screenPos.z),
		1.0f
	);

	//Then, we multiply our clipspace coordinate by our inverted matrix
	Vector4 transformed = invVP * clipSpace;

	//our transformed w coordinate is now the 'inverse' perspective divide, so
	//we can reconstruct the final world space by dividing x,y,and z by w.
	return Vector3(transformed.x / transformed.w, transformed.y / transformed.w, transformed.z / transformed.w);
}

Matrix4 PhysicsXSystem::GenerateInverseView(const Camera& c)
{
	float pitch = c.GetPitch();
	float yaw = c.GetYaw();
	Vector3 position = c.GetPosition();

	Matrix4 iview =
		Matrix4::Translation(position) *
		Matrix4::Rotation(yaw, Vector3(0, 1, 0)) *
		Matrix4::Rotation(pitch, Vector3(1, 0, 0));

	return iview;
}

Matrix4 PhysicsXSystem::GenerateInverseProjection(float aspect, float fov, float nearPlane, float farPlane)
{
	Matrix4 m;

	float t = tan(fov * PI_OVER_360);

	float neg_depth = nearPlane - farPlane;

	const float h = 1.0f / t;

	float c = (farPlane + nearPlane) / neg_depth;
	float e = -1.0f;
	float d = 2.0f * (nearPlane * farPlane) / neg_depth;

	m.array[0] = aspect / h;
	m.array[5] = tan(fov * PI_OVER_360);

	m.array[10] = 0.0f;
	m.array[11] = 1.0f / d;

	m.array[14] = 1.0f / e;

	m.array[15] = -c / (d * e);

	return m;
}

bool PhysicsXSystem::raycast(Vector3 origin, Vector3 dir, float maxdis, PxRaycastBuffer& hit)
{
	PxVec3 pxori = PhysXConvert::Vector3ToPxVec3(origin);
	PxVec3 pxdir = PhysXConvert::Vector3ToPxVec3(dir);
	return gScene->raycast(pxori, pxdir, maxdis, hit);
}

bool PhysicsXSystem::raycastCam(Camera& camera, float maxdis,PxRaycastBuffer& hit)
{
	Vector3 camPos = camera.GetPosition();
	PxVec3 pxCamPos = PhysXConvert::Vector3ToPxVec3(camPos);
	Vector2 screenMouse = Window::GetMouse()->GetAbsolutePosition();
	Vector2 screenSize = Window::GetWindow()->GetScreenSize();
	auto nearPos = Vector3(screenMouse.x,
		screenSize.y - screenMouse.y,
		-0.99999f
	);
	auto farPos = Vector3(screenMouse.x,
		screenSize.y - screenMouse.y,
		0.99999f
	);
	Vector3 a = Unproject(nearPos, camera);
	Vector3 b = Unproject(farPos, camera);
	Vector3 c = b - a;
	c.Normalise();
	return raycast(camera.GetPosition(), c, maxdis, hit);
}

void PhysicsXSystem::SyncGameObjs()
{
	std::vector<GameObject*>& actors=gameWorld.GetGameObjects();
	for (int i = 0; i < actors.size();i++) {
		PhysicsXObject* obj = actors[i]->GetPhysicsXObject();
		if (obj == nullptr)continue;
		if (obj->isInScene())continue;
		if (obj->isDynamic())addDynamicActor(*actors[i]);
		else addStaticActor(*actors[i]);
	}
}



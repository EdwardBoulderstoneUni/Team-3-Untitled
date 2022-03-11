#include "PhysicsXSystem.h"
#include "PhysXConvert.h"
#include "../../Common/Maths.h"
#include <vector>
#include "../../include/PhysX/PxPhysicsAPI.h"
#include "../../Gameplay/Player.h"


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
PxControllerManager* gManager = NULL;

PxFilterFlags contactReportFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags & pairFlags, const void* constantBlock, PxU32 constantBlockSize)
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

PxFilterFlags customizeFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	
	PX_UNUSED(attributes0);
	PX_UNUSED(attributes1);
	PX_UNUSED(filterData0);
	PX_UNUSED(filterData1);
	PX_UNUSED(constantBlockSize);
	PX_UNUSED(constantBlock);


	//// 默认对所有为过滤的碰撞产生默认回调
	//pairFlags = PxPairFlag::eCONTACT_DEFAULT;

	//// 如果两个物体的filterData均包含对方的ID，则需要触发eNOTIFY_TOUCH_FOUND回调
	//if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
	//{
	//	pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
	//}

	//// 如果某个物体可以吸收(Drain)粒子，那么只允许它跟粒子有碰撞，忽略它与其他物体的碰撞
	//if (isDrainGroup(filterData0) || isDrainGroup(filterData1))
	//{
	//	PxFilterData filterDataOther = isDrainGroup(filterData0) ? filterData1 : filterData0;
	//	if (!isParticleGroup(filterDataOther))
	//		return PxFilterFlag::eKILL;
	//}

	//// 属于force0组的物体，只与smoke碰撞
	//if (isForce0Group(filterData0) || isForce0Group(filterData1))
	//{
	//	PxFilterData filterDataOther = isForce0Group(filterData0) ? filterData1 : filterData0;
	//	if (filterDataOther != collisionGroupSmoke)
	//		return PxFilterFlag::eKILL;
	//}

	//// 属于force1组的物体，只与waterfall碰撞
	//if (isForce1Group(filterData0) || isForce1Group(filterData1))
	//{
	//	PxFilterData filterDataOther = isForce1Group(filterData0) ? filterData1 : filterData0;
	//	if (filterDataOther != collisionGroupWaterfall)
	//		return PxFilterFlag::eKILL;

	return PxFilterFlag::eDEFAULT;
}

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
		PhysicsXSystem::FlagCheck(a, b);
	}
};
class CharacterCallback :public PxUserControllerHitReport,public PxControllerBehaviorCallback {
	void onShapeHit(const PxControllerShapeHit& hit) {
		PX_UNUSED((hit));
		GameObject* a = (GameObject*)hit.actor->userData;
		GameObject* b = (GameObject*)hit.controller->getActor()->userData;
		PhysicsXSystem::FlagCheck(a, b);
	}
	void onControllerHit(const PxControllersHit& hit) {}
	void onObstacleHit(const PxControllerObstacleHit& hit){}

	PxControllerBehaviorFlags		getBehaviorFlags(const PxShape& shape, const PxActor& actor) { 
		//GameObject* a = (GameObject*)actor.userData;
		return PxControllerBehaviorFlags(0); 
	}
	PxControllerBehaviorFlags		getBehaviorFlags(const PxController& controller) { return PxControllerBehaviorFlags(0); }
	PxControllerBehaviorFlags		getBehaviorFlags(const PxObstacle& obstacle) { return PxControllerBehaviorFlags(0); }
};

ContackCallback* callback = new ContackCallback;
CharacterCallback* characterCallback = new CharacterCallback;


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
	PX_RELEASE(gFoundation);

	if (gPvd)
	{
		PxPvdTransport* transport = gPvd->getTransport();
		gPvd->release();	gPvd = NULL;
		PX_RELEASE(transport);
	}
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
	sceneDesc.filterShader = contactReportFilterShader;
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
	gManager = PxCreateControllerManager(*gScene);
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
		SynActorsPose(&actors[0], static_cast<PxU32>(actors.size()));
	}
}

void PhysicsXSystem::addActor(GameObject& actor)
{
	PhysicsXObject* phyObj = actor.GetPhysicsXObject();
	PhyProperties properties = phyObj->properties;
	PxTransform trans = PxTransform();
	PxRigidDynamic* dynaBody = nullptr;
	PxRigidStatic* statBody = nullptr;
	PxShape* shape = nullptr;

	PxBoxControllerDesc* desc=nullptr;
	PxBoxGeometry* geo = nullptr;
	switch (properties.type)
	{
	case PhyProperties::Dynamic:
		dynaBody = gPhysics->createRigidDynamic(properties.transform);

		shape = gPhysics->createShape(*properties.volume, *gMaterial);
		dynaBody->attachShape(*shape);
		dynaBody->setMass(properties.Mass);

		gScene->addActor(*dynaBody);

		dynaBody->userData = &actor;
		phyObj->rb = dynaBody;
		shape->release();
		break;
	case PhyProperties::Static:
		statBody = gPhysics->createRigidStatic(properties.transform);

		shape = gPhysics->createShape(*properties.volume, *gMaterial);
		statBody->attachShape(*shape);

		gScene->addActor(*statBody);

		statBody->userData = &actor;
		phyObj->rb = statBody;

		shape->release();
		break;
	case PhyProperties::Character:
		
		geo = (PxBoxGeometry*)properties.volume;
		trans = properties.transform;
		desc = new PxBoxControllerDesc();
		desc->halfSideExtent = geo->halfExtents.x;
		desc->halfForwardExtent = geo->halfExtents.z;
		desc->halfHeight = geo->halfExtents.y;
		desc->position.set(trans.p.x, trans.p.y, trans.p.z);
		desc->material = gMaterial;
		desc->density = 10;
		desc->reportCallback = characterCallback;
		desc->behaviorCallback = characterCallback;

		phyObj->controller = gManager->createController(*desc);
		phyObj->controller->getActor()->userData = &actor;
		phyObj->rb = phyObj->controller->getActor();
		break;
	default:
		break;
	}
}

void PhysicsXSystem::SynActorsPose(PxRigidActor** actors, const PxU32 numActors)
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
			if (obj->GetPhysicsXObject()->controller)continue;
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


	Matrix4 invVP = GenerateInverseView(cam) * GenerateInverseProjection(aspect, fov, nearPlane, farPlane);

	auto clipSpace = Vector4(
		(screenPos.x / screenSize.x) * 2.0f - 1.0f,
		(screenPos.y / screenSize.y) * 2.0f - 1.0f,
		(screenPos.z),
		1.0f
	);

	Vector4 transformed = invVP * clipSpace;

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
	for (auto actor:actors)
	{
		PhysicsXObject* obj = actor->GetPhysicsXObject();
		if (obj->rb)continue;
		addActor(*actor);
	}
}

void PhysicsXSystem::FlagCheck(GameObject* a, GameObject* b) {

	//here bullet和player之间的关系需要重新调整
	if (a->type == GameObjectType_floor and
		b->type == GameObjectType_team1) {
		Player* player = (Player*)b;
		player->isGrounded = true;
	}
		
}



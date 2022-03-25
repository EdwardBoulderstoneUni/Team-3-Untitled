#include "Creeper.h"
#include "CreeperState.h"
#define PI 3.1415926
Creeper::Creeper(GameWorld& gameWorld):world(gameWorld)
{
	type = GameObjectType_AI;
	target = nullptr;
	SetupStateMachine();
}

Creeper::~Creeper()
{

}

void Creeper::SetUp()
{
	auto physics = new ComponentPhysics();
	physics->phyObj = GetPhysicsXObject();
	PhyProperties properties = PhyProperties();
	properties.type = PhyProperties::Character;
	properties.transform = PhysXConvert::TransformToPxTransform(GetTransform());
	properties.Mass = 10.0f;
	properties.positionOffset = Vector3(0, 4.1, 0);

	Vector3 scale = GetTransform().GetScale();
	properties.volume = new PxCapsuleGeometry(PhysXConvert::Vector3ToPxVec3(scale).x,
		PhysXConvert::Vector3ToPxVec3(scale).y);

	physics->phyObj->properties = properties;
	PushComponent(physics);

}

void Creeper::Update(float dt)
{
	std::vector<GameObject*> objs=world.GetGameObjects();
	if (target) {
		forward = (target->GetTransform().GetPosition() - GetTransform().GetPosition()).Normalised();
		GetTransform().SetOrientation(target->GetTransform().GetOrientation()*Quaternion::EulerAnglesToQuaternion(0,200,0));
		if ((target->GetTransform().GetPosition() - GetTransform().GetPosition()).Length() > visualField) {
			target = nullptr;
		}
	}
	else {
		for (auto i : objs) {
			if (i->type == GameObjectType_team1 or i->type == GameObjectType_team2) {
				Vector3 pos = i->GetTransform().GetPosition();
				if ((pos - GetTransform().GetPosition()).Length() <= this->visualField) {
						target = (Player*)i;
						forward = (target->GetTransform().GetPosition() - GetTransform().GetPosition()).Normalised();
				}
			}
		}
	}
	stateMachine->Update(dt);
}

void Creeper::SetupStateMachine()
{
	_Idle* idle = new _Idle();
	idle->userdata = this;
	stateMachine = new PushdownMachine(idle);

}

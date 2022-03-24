#include "Player.h"
#include "PlayerController.h"
#include "../CSC8503/CSC8503Common/PhysicsXSystem.h"
#include "PlayerState.h"
#include "WeaponState.h"
Player::Player(PlayerRole colour, AbilityContainer* aCont, GameObjectType type)
{
	dirVec.forward = Quaternion(transform.GetOrientation()) * Vector3(0, 0, 1);
	dirVec.CaculateRight();
	pColour = colour;
	playerPro = new PlayerPro();
	timeStack = new TimeStack();
	networkInput = false;
	AssignRole(aCont);
	this->type = type;
	SetupStateMachine();
}

Player::~Player() {


}

void Player::SetUp()
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

	
	const auto input = new ComponentInput();
	input->user_interface = new PlayerController();

	PushComponent(input);

	auto camera = new ComponentCamera();
	camera->gO = this;

	camera->camera = new Camera();
	camera->camera->SetNearPlane(0.1f);
	camera->camera->SetFarPlane(500.0f);
	camera->camera->SetPitch(-15.0f);

	PushComponent(camera);	
}




void Player::Respawn() {
	YiEventSystem::GetMe()->PushEvent(PLAYER_RESPWAN, GetWorldID());
}


void Player::AssignRole(AbilityContainer* aCont)
{
	std::string colour;

	switch (pColour) {
	case PlayerRole_red:
		colour = "Red";
		abilities[0] = aCont->allAbilities[0];
		abilities[1] = aCont->allAbilities[1];
		playerPro->damage = 5;
		break;
	case PlayerRole_green:
		colour = "Green";
		abilities[0] = aCont->allAbilities[2];
		abilities[1] = aCont->allAbilities[3];
		playerPro->damage = 4;
		break;
	case PlayerRole_blue:
		colour = "Blue";
		abilities[0] = aCont->allAbilities[4];
		abilities[1] = aCont->allAbilities[5];
		playerPro->damage = 3;
		break;
	}
}

void Player::SetupStateMachine()
{
	Idle* idle = new Idle();
	idle->userdata = this;
	playerState = new PushdownMachine(idle);

	Hold* hold = new Hold();
	hold->userdata = this;
	weaponState = new PushdownMachine(hold);
}
void Player::Update(float dt) {
	ComponentGameObject::Update(dt);

	if (GetComponentCamera()) {
		Vector2 screenSize = Window::GetWindow()->GetScreenSize();
		Vector3 target = PhysicsXSystem::getMe()->ScreenToWorld(*GetComponentCamera()->camera, screenSize / 2.0f, false);
		dirVec.shootDir = (target - transform.GetPosition()).Normalised();
		dirVec.forward = dirVec.shootDir;
	}
	
	dirVec.CaculateRight();

	if (GetComponentInput()) {
		lastInput = GetComponentInput()->user_interface->get_inputs();
		transform.SetOrientation(Quaternion::EulerAnglesToQuaternion(lastInput.look_direction.x,
			lastInput.look_direction.y, 0));
	}
	if (networkInput) {
		transform.SetOrientation(Quaternion::EulerAnglesToQuaternion(lastInput.look_direction.x,
			lastInput.look_direction.y, 0));
	}
	playerState->Update(dt);
	weaponState->Update(dt);
	timeStack->dashCooldown -= dt;
	timeStack->respawnCooldown -= dt;
	timeStack->grenadeCD -= dt;
}
#include "Player.h"
#include "PlayerController.h"
#include "../CSC8503/CSC8503Common/PhysicsXSystem.h"
#include "PlayerState.h"
#include "WeaponState.h"
Player::Player(PlayerRole colour, AbilityContainer* aCont, GameObjectType type,bool localplayer)
{
	forward = Quaternion(transform.GetOrientation()) * Vector3(0, 0, 1);
	right = Vector3::Cross(Vector3(0, 1, 0), forward);
	shootDir = forward;
	pColour = colour;
	isLocalPlayer = localplayer;
	AssignRole(aCont);
	this->type = type;
	SetupStateMachine();
}

Player::~Player() {
	for (auto i : abilities)
		delete i;

}

void Player::SetUp()
{
	auto physics = new ComponentPhysics();
	physics->phyObj = GetPhysicsXObject();
	PhyProperties properties = PhyProperties();
	properties.type = PhyProperties::Character;
	properties.transform = PhysXConvert::TransformToPxTransform(GetTransform());
	properties.Mass = 10.0f;
	properties.positionOffset = Vector3(0,4.1,0);

	Vector3 scale = GetTransform().GetScale();
	properties.volume = new PxCapsuleGeometry(PhysXConvert::Vector3ToPxVec3(scale).x,
		PhysXConvert::Vector3ToPxVec3(scale).y);

	physics->phyObj->properties = properties;
	PushComponent(physics);

	if (isLocalPlayer) {
		const auto input = new ComponentInput();
		input->user_interface = new PlayerController();

		PushComponent(input);

		auto camera = new ComponentCamera();
		camera->gO = this;

		camera->camera = new Camera();
		camera->camera->SetNearPlane(0.1f);
		camera->camera->SetFarPlane(500.0f);
		camera->camera->SetPitch(-15.0f);
		camera->camera->SetYaw(180);

		PushComponent(camera);
	}
}
void Player::Move() {
	if (lastInput.movement_direction == Vector2(0, 1)) {
		physicsXObject->CMove(PhysXConvert::Vector3ToPxVec3(forward));
	}
	if (lastInput.movement_direction == Vector2(0, -1)) {
		physicsXObject->CMove(PhysXConvert::Vector3ToPxVec3(-forward));
	}
	if (lastInput.movement_direction == Vector2(1, 0)) {
		physicsXObject->CMove(PhysXConvert::Vector3ToPxVec3(right));
	}
	if (lastInput.movement_direction == Vector2(-1, 0)) {
		physicsXObject->CMove(PhysXConvert::Vector3ToPxVec3(-right));
	}
}
void Player::Jump(float dt) {
	physicsXObject->CMove(PxVec3(0, 1, 0) * cos(JumpingTimeStack));
	JumpingTimeStack += dt * 3;
}
void Player::Dash(float dt) {
	physicsXObject->controller->move(PhysXConvert::Vector3ToPxVec3(forward)*3.0f, 0.0001f, 0.2,
			PxControllerFilters(), NULL);
	DashingTimeStack += dt;
}
void Player::Openfire() {
	if (ammo > 0) {
		YiEventSystem::GetMe()->PushEvent(PLAYER_OPEN_FIRE, GetWorldID());
		ammo--;
	}
}
float Player::TakeDamage(float dmg) {
	health = health - dmg < 0 ? 0 : health - dmg;
	return health;
}
bool Player::IsDead() {
	return health == 0 ? true : false;
}
void Player::Respawn() {
	auto physics = new ComponentPhysics();
	physics->phyObj = GetPhysicsXObject();
	PhyProperties properties = PhyProperties();
	properties.type = PhyProperties::Character;
	properties.transform = PhysXConvert::TransformToPxTransform(GetTransform());
	properties.Mass = 10.0f;

	Vector3 scale = GetTransform().GetScale() / 2.0f;
	properties.volume = new PxBoxGeometry(PhysXConvert::Vector3ToPxVec3(scale));

	physics->phyObj->properties = properties;
	PushComponent(physics);
}

// Give damage to palyer a
void Player::GiveDamage(float dmg, Player* a) {
	a->TakeDamage(dmg);
	if (a->IsDead() == true) {
		teamKill++;
	}
}

void Player::Reload() {
	isReloading = false;
	if (ammo >= 0 && ammo < maxAmmo) {
		isReloading = true;
		ammo = maxAmmo;
		// Finish reload
		isReloading = false;
	}
}

void Player::AssignRole(AbilityContainer* aCont)
{
	std::string colour;

	switch (pColour) {
	case PlayerRole_red:
		colour = "Red";
		abilities[0] = aCont->allAbilities[0];
		abilities[1] = aCont->allAbilities[1];
		break;
	case PlayerRole_green:
		colour = "Green";
		abilities[0] = aCont->allAbilities[2];
		abilities[1] = aCont->allAbilities[3];
		break;
	case PlayerRole_blue:
		colour = "Blue";
		abilities[0] = aCont->allAbilities[4];
		abilities[1] = aCont->allAbilities[5];
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
	forward = transform.GetOrientation() * Vector3(0, 0, 1);
	right = Vector3::Cross(forward, Vector3(0, 1, 0));
	if(GetComponentInput())
		lastInput = GetComponentInput()->user_interface->get_inputs();
	playerState->Update(dt);
	weaponState->Update(dt);
	dashCooldown -= dt;
	respawnCooldown -= dt;
}
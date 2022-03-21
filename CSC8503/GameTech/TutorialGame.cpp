#include "TutorialGame.h"
#include "../CSC8503Common/GameWorld.h"
#include "../CSC8503Common/GameObjectGenerator.h"
#include "../CSC8503Common/AssetManager.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Plugins/OpenGLRendering/ShaderManager.h"
#include "../../Common/TextureLoader.h"
#include "../../Common/Assets.h"
#include "../GameTech/TutorialMenu.h"
#include "..//..//Gameplay/ePlayerRole.h"
#include "../../Gameplay/GameObjects.h"
#include "../../Gameplay/Bullet.h"
#include "../../Gameplay/Grenade.h"

using namespace NCL;
using namespace CSC8503;

TutorialGame::TutorialGame()
{ 
	eventSystem = new YiEventSystem();
	world = new GameWorld();
	renderer = new GameTechRenderer(*world);
	physicsX = new PhysicsXSystem(*world);
	DebugMode = false;

	Debug::SetRenderer(renderer);
	InitialiseUI();
	InitialiseAssets();
}

/*

Each of the little demo scenarios used in the game uses the same 2 meshes, 
and the same texture and shader. There's no need to ever load in anything else
for this module, even in the coursework, but you can add it if you like!

*/
void TutorialGame::UpdateRender(float dt)
{
	Debug::FlushRenderables(dt);
	renderer->Update(dt);
	renderer->Render();
}
void TutorialGame::SetSingleMode()
{

	InitialiseAssets();
}

void TutorialGame::SetMultiMode()
{

	InitWorld();
}
void TutorialGame::InitialiseAssets() {
	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("cube.msh", &cubeMesh);
	loadFunc("sphere.msh", &sphereMesh);
	loadFunc("Female_Guard.msh", &charMeshA);
	loadFunc("courier.msh", &charMeshB);
	loadFunc("security.msh", &enemyMesh);
	loadFunc("coin.msh", &bonusMesh);
	loadFunc("capsule.msh", &capsuleMesh);

	basicTex = (OGLTexture*)TextureLoader::LoadAPITexture("corridor_wall_c.tga");
	basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");

	ShaderManager::GetInstance()->Init();
	AssetManager::GetInstance()->Init();
	InitAbilityContainer();
	GameObjectGenerator g;
	std::string worldFilePath = Assets::DATADIR;
	worldFilePath.append("world.json");
	g.Generate(worldFilePath.c_str(), *world);

	InitWorld();
	InitPlayer(Vector3(20, 3, 0), GameObjectType_team2);
	InitPlayer(Vector3(20, 3, -20), GameObjectType_team1,true);
	RegisterEventHandles();
}

void TutorialGame::InitialiseUI()
{
	gameUI = new GameUI();
	renderer->SetUI(gameUI);
	//gameMenu.reset(new TutorialMenu(this));
	//gameUI->PushMenu(gameMenu);
	//InGameState* t = new InGameState(this);
	//pauseMachine = new PushdownMachine(t);
	//pauseMachine = new PushdownMachine(new InGameState(this));
}
TutorialGame::~TutorialGame()	{
	AudioManager::Cleanup();
	delete physicsX;
	delete renderer;
	delete world;
	delete gameUI;
	delete player;
	delete abilityContainer;
}

void TutorialGame::UpdateGame(float dt)
{
	eventSystem->ProcessAllEvent();
	AudioManager::GetInstance().Play_Sound();
	AudioManager::GetInstance().Update(dt);

	if (DebugMode) {
		CalculateFrameRate(dt);
	}
	UpdateGameObjects(dt);
	physicsX->Update(dt);
	
	HUDUpdate(dt);
	world->UpdateWorld(dt);
	renderer->Update(dt);
	renderer->Render();

	Debug::FlushRenderables(dt);

}

void TutorialGame::InitAbilityContainer() {
	abilityContainer = new AbilityContainer();
}

void TutorialGame::InitPlayer(Vector3 pos, GameObjectType team,bool islocal)
{
	player = new Player(PlayerRole_blue, abilityContainer, team,islocal);


	player->GetTransform()
		.SetScale(Vector3(4, 4, 4))
		.SetPosition(pos);
	

	player->InitAllComponent();

	player->SetRenderObject(new RenderObject(&player->GetTransform(), charMeshA, basicTex, basicShader));

	world->SetMainCamera(player->GetComponentCamera()->camera);
	
	world->AddGameObject(player);
}

void TutorialGame::InitWorld()
{
	InitDefaultFloor(Vector3(0,0,0),Vector4(1,1,1,1));
	InitDefaultFloor(Vector3(150, 0, 0), Vector4(0, 1, 0, 1));
	InitDefaultFloor(Vector3(-150, 0, 0), Vector4(1, 0, 0, 1));
	InitDefaultFloor(Vector3(0,0,150), Vector4(0, 0, 1, 1));
	AudioManager::Startup();
	//AudioManager::GetInstance().Play_Sound();
}

/*

A single function to add a large immoveable cube to the bottom of our world

*/
GameObject* TutorialGame::AddFloorToWorld(const Vector3& position)
{
	auto floor = new Floor();

	auto floorSize = Vector3(100, 2, 100);
	
	floor->GetTransform()
	     .SetScale(floorSize * 2)
	     .SetPosition(position);

	floor->InitAllComponent();

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader));
	
	world->AddGameObject(floor);

	return floor;
}

/*

Builds a game object that uses a sphere mesh for its graphics, and a bounding sphere for its
rigid body representation. This and the cube function will let you build a lot of 'simple' 
physics worlds. You'll probably need another function for the creation of OBB cubes too.

*/
GameObject* TutorialGame::AddSphereToWorld(const Vector3& position, float radius, float inverseMass)
{
	auto sphere = new Sphere();

	auto sphereSize = Vector3(radius, radius, radius);


	sphere->GetTransform()
	      .SetScale(sphereSize)
	      .SetPosition(position);
	sphere->InitAllComponent();
	sphere->SetRenderObject(new RenderObject(&sphere->GetTransform(), sphereMesh, basicTex, basicShader));

	world->AddGameObject(sphere);

	return sphere;
}

GameObject* TutorialGame::AddCapsuleToWorld(const Vector3& position, float halfHeight, float radius, float inverseMass)
{
	auto capsule = new GameObject();

	//auto volume = new CapsuleVolume(halfHeight, radius);
	//capsule->SetBoundingVolume(volume);

	capsule->GetTransform()
	       .SetScale(Vector3(radius * 2, halfHeight, radius * 2))
	       .SetPosition(position);

	capsule->SetRenderObject(new RenderObject(&capsule->GetTransform(), capsuleMesh, basicTex, basicShader));
	//capsule->SetPhysicsObject(new PhysicsObject(&capsule->GetTransform(), capsule->GetBoundingVolume()));

	//capsule->GetPhysicsObject()->SetInverseMass(inverseMass);
	//capsule->GetPhysicsObject()->InitCubeInertia();

	world->AddGameObject(capsule);

	return capsule;
}

GameObject* TutorialGame::AddCubeToWorld(const Vector3& position, Vector3 dimensions, float inverseMass)
{
	auto cube = new Cube();

	cube->GetTransform()
	    .SetPosition(position)
	    .SetScale(dimensions * 2);
	cube->InitAllComponent();
	cube->SetRenderObject(new RenderObject(&cube->GetTransform(), cubeMesh, basicTex, basicShader));
	
	world->AddGameObject(cube);

	return cube;
}

void TutorialGame::InitDefaultFloor(Vector3 position, Vector4 color)
{
	Floor* floor = new Floor();

	floor->GetTransform()
		.SetScale(Vector3(150, 1, 150))
		.SetPosition(position);

	floor->InitAllComponent();

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader));
	floor->GetRenderObject()->SetColour(color);

	world->AddGameObject(floor);
}

void TutorialGame::RegisterEventHandles()
{
	eventSystem->RegisterEventHandle("OPEN_FIRE", _openFirHandle,(DWORD64)this);
	eventSystem->RegisterEventHandle("THROW_GRENADE", _GrenadeHandle, (DWORD64)this);
	eventSystem->RegisterEventHandle("OBJECT_DELETE", _deleteHandle,(DWORD64)this);
	eventSystem->RegisterEventHandle("HIT", _HitHandle, (DWORD64)world);
	eventSystem->RegisterEventHandle("RESPWAN", _respawnHandle, (DWORD64)world);
	eventSystem->RegisterEventHandle("COLOR_ZONE", _colorzoneHandle, (DWORD64)world);
	eventSystem->RegisterEventHandle("DAMAGE_RANGE", _damageRangeHandle, (DWORD64)world);
}

void TutorialGame::HUDUpdate(float dt)
{
	PlayerPro* playerPro = player->GetPlayerPro();
	TimeStack* timeStack = player->GetTimeStack();
	renderer->DrawString("Damage :" + std::to_string(playerPro->damage), Vector2(5, 95));
	renderer->DrawString("Ammo Left: " + std::to_string(playerPro->ammo), Vector2(5, 90));
	if (playerPro->ammo == 0) {
		renderer->DrawString("Press R to reload. ", Vector2(30, 40));
	}

	int health = playerPro->health;
	renderer->DrawString("Health: " + std::to_string(health), Vector2(5, 85));
	renderer->DrawString("Speed : " + std::to_string(playerPro->speed), Vector2(5, 100));

	//World timer
	if (tLeft >= 0) {
		tLeft -= dt;
		int m = tLeft / 60;
		int s = int(tLeft) % 60;
		renderer->DrawString("Time Remaining: " + std::to_string(m) + "m" + std::to_string(s) + "s", Vector2(30, 10));
	}
	else {
		YiEventSystem::GetMe()->PushEvent(GAME_OVER);
	}

	renderer->DrawString("Score: " + std::to_string(playerPro->score), Vector2(70, 85));
	renderer->DrawString("TeamKill: " + std::to_string(playerPro->teamKill), Vector2(70, 20));

	if(timeStack->dashCooldown>0)
		renderer->DrawString("Dash CD: " + std::to_string(timeStack->dashCooldown), Vector2(5, 80));
	else
		renderer->DrawString("Dash ready!", Vector2(5, 80));

	if (timeStack->grenadeCD > 0)
		renderer->DrawString("Grenade CD: " + std::to_string(timeStack->grenadeCD), Vector2(5, 60));
	else
		renderer->DrawString("Grenade ready!", Vector2(5, 60));

	Vector3 position = player->GetTransform().GetPosition()+Vector3(0,5,0);
	Vector2 screenSize = Window::GetWindow()->GetScreenSize();

#define TARGET_OFF 20.0f
	Vector3 target = PhysicsXSystem::getMe()->ScreenToWorld(*player->GetComponentCamera()->camera, screenSize / 2.0f);
	Vector3 targetleft= PhysicsXSystem::getMe()->ScreenToWorld(*player->GetComponentCamera()->camera, 
		screenSize / 2.0f+Vector2(-TARGET_OFF,0));
	Vector3 targetright = PhysicsXSystem::getMe()->ScreenToWorld(*player->GetComponentCamera()->camera,
		screenSize / 2.0f + Vector2(TARGET_OFF, 0));
	Vector3 targettop = PhysicsXSystem::getMe()->ScreenToWorld(*player->GetComponentCamera()->camera,
		screenSize / 2.0f + Vector2(0, TARGET_OFF));
	Vector3 targetbot = PhysicsXSystem::getMe()->ScreenToWorld(*player->GetComponentCamera()->camera,
		screenSize / 2.0f + Vector2(0, -TARGET_OFF));
	renderer->DrawLine(position, target, Vector4(0,1,0,1));
	renderer->DrawLine(targetleft, targetright, Vector4(0, 1, 0, 1));
	renderer->DrawLine(targettop, targetbot, Vector4(0, 1, 0, 1));
}

GameObject* TutorialGame::AddPlayerToWorld(const Vector3& position)
{
	float meshSize = 3.0f;
	float inverseMass = 0.5f;

	auto character = new Player(PlayerRole::PlayerRole_blue, abilityContainer, GameObjectType_team1);

	//auto volume = new AABBVolume(Vector3(0.3f, 0.85f, 0.3f) * meshSize);

	//character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
	         .SetScale(Vector3(meshSize, meshSize, meshSize))
	         .SetPosition(position);
	character->InitAllComponent();

	if (rand() % 2)
	{
		character->SetRenderObject(new RenderObject(&character->GetTransform(), charMeshA, nullptr, basicShader));
	}
	else
	{
		character->SetRenderObject(new RenderObject(&character->GetTransform(), charMeshB, nullptr, basicShader));
	}
	
	
	world->AddGameObject(character);

	return character;
}

GameObject* TutorialGame::AddEnemyToWorld(const Vector3& position)
{
	float meshSize = 3.0f;
	float inverseMass = 0.5f;

	auto character = new GameObject();

	//auto volume = new AABBVolume(Vector3(0.3f, 0.9f, 0.3f) * meshSize);
	//character->SetBoundingVolume((CollisionVolume*)volume);

	character->GetTransform()
	         .SetScale(Vector3(meshSize, meshSize, meshSize))
	         .SetPosition(position);

	character->SetRenderObject(new RenderObject(&character->GetTransform(), enemyMesh, nullptr, basicShader));
	//character->SetPhysicsObject(new PhysicsObject(&character->GetTransform(), character->GetBoundingVolume()));

	//character->GetPhysicsObject()->SetInverseMass(inverseMass);
	//character->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(character);

	return character;
}

GameObject* TutorialGame::AddPaint(const Vector3& position)
{
	GameObject* disc = new GameObject();

	disc->GetTransform()
		.SetScale(Vector3(4, 0.01f, 4))
		.SetPosition(position);

	disc->SetRenderObject(new RenderObject(&disc->GetTransform(), AssetManager::GetInstance()->GetMesh("Cylinder.msh"), nullptr, basicShader));
	disc->GetRenderObject()->SetColour(Vector4(1, 0, 0, 1));

	world->AddGameObject(disc);
	return disc;
}

GameObject* TutorialGame::AddBonusToWorld(const Vector3& position)
{
	auto apple = new GameObject();

	//auto volume = new SphereVolume(0.25f);
	//apple->SetBoundingVolume((CollisionVolume*)volume);
	apple->GetTransform()
	     .SetScale(Vector3(0.25, 0.25, 0.25))
	     .SetPosition(position);

	apple->SetRenderObject(new RenderObject(&apple->GetTransform(), bonusMesh, nullptr, basicShader));
	//apple->SetPhysicsObject(new PhysicsObject(&apple->GetTransform(), apple->GetBoundingVolume()));

	//apple->GetPhysicsObject()->SetInverseMass(1.0f);
	//apple->GetPhysicsObject()->InitSphereInertia();

	world->AddGameObject(apple);

	return apple;
}


void TutorialGame::CalculateFrameRate(float dt) {
	float currentTime = GetTickCount64() * 0.001f;
	++framesPerSecond;
	if (currentTime - lastTime > 1.0f)
	{
		lastTime = currentTime;
		FPS = framesPerSecond;
		framesPerSecond = 0;
	}
	renderer->DrawString(std::to_string(FPS), Vector2(20, 80));
}


void TutorialGame::_openFirHandle(const EVENT* pEvent, DWORD64 dwOwnerData)
{
	TutorialGame* game = (TutorialGame*)dwOwnerData;
	string worldID = pEvent->vArg[0];

	Player* player = static_cast<Player*>(game->world->FindObjectbyID(stoi(worldID)));
	Vector3 position = player->GetTransform().GetPosition() + Vector3(0,5,0);

	auto bullet = new Bullet(*player);

	auto sphereSize = Vector3(1.0f, 1.0f, 1.0f);
	DirectionVec dir = player->GetDirectionVec();
	bullet->GetTransform()
		.SetScale(sphereSize)
		.SetPosition(position + dir.shootDir * 15);
	bullet->InitAllComponent();
	bullet->SetRenderObject(new RenderObject(&bullet->GetTransform(), game->sphereMesh,
		game->basicTex, game->basicShader));

	game->world->AddGameObject(bullet);

	//auto func = [](GameObject* object, Vector3 position) {TutorialGame::getMe()->AddPaint(position); };
	//bullet->SetCollisionFunction(func);
	game->physicsX->addActor(*bullet);
	bullet->GetPhysicsXObject()->SetLinearVelocity(dir.shootDir * 250.0f);
}

void TutorialGame::_GrenadeHandle(const EVENT* pEvent, DWORD64 dwOwnerData) {
	TutorialGame* game = (TutorialGame*)dwOwnerData;
	string worldID = pEvent->vArg[0];

	Player* player = static_cast<Player*>(game->world->FindObjectbyID(stoi(worldID)));
	Vector3 position = player->GetTransform().GetPosition() + Vector3(0, 5, 0);

	auto grenade = new Grenade(*player);

	auto cubeSize = Vector3(1.0f, 1.0f, 1.0f);
	DirectionVec dir = player->GetDirectionVec();
	grenade->GetTransform()
		.SetScale(cubeSize)
		.SetPosition(position + dir.shootDir * 15);
	grenade->InitAllComponent();
	grenade->SetRenderObject(new RenderObject(&grenade->GetTransform(), game->cubeMesh,
		game->basicTex, game->basicShader));

	game->world->AddGameObject(grenade);

	//auto func = [](GameObject* object, Vector3 position) {TutorialGame::getMe()->AddPaint(position); };
	//bullet->SetCollisionFunction(func);
	game->physicsX->addActor(*grenade);
	grenade->GetPhysicsXObject()->SetLinearVelocity(dir.shootDir * 60.0f);
}

void TutorialGame::_deleteHandle(const EVENT* pEvent, DWORD64 dwOwnerData)
{
	TutorialGame* game = (TutorialGame*)dwOwnerData;
	string worldID = pEvent->vArg[0];
	GameObject* temp= game->world->FindObjectbyID(stoi(worldID));
	game->physicsX->deleteActor(*temp);
	game->world->RemoveGameObject(temp);
}
void TutorialGame::_HitHandle(const EVENT* pEvent, DWORD64 dwOwnerData)
{
	string bulletID = pEvent->vArg[0];
	string hitID = pEvent->vArg[1];
	GameWorld* world = (GameWorld*)dwOwnerData;

	Bullet* bullet = static_cast<Bullet*>(world->FindObjectbyID(stoi(bulletID)));
	if (not bullet)return;
	int shooterID = bullet->GetShooterID();

	Player* shooter = static_cast<Player*>(world->FindObjectbyID(shooterID));
	Player* hitobj = static_cast<Player*>(world->FindObjectbyID(stoi(hitID)));
	PlayerPro* playerPro = hitobj->GetPlayerPro();
	int health=hitobj->GetPlayerPro()->health;
	if (health > 0 and playerPro->health - bullet->GetDamage() <= 0) {
		std::cout << (std::to_string(shooter->GetWorldID()) + " --->" +
			std::to_string(hitobj->GetWorldID())) << std::endl;
		shooter->GetPlayerPro()->teamKill++;
	}
	if (playerPro->health not_eq 0)
		shooter->GetPlayerPro()->score++;
	playerPro->health = playerPro->health - bullet->GetDamage() < 0 ? 0 : playerPro->health - bullet->GetDamage();
	YiEventSystem::GetMe()->PushEvent(OBJECT_DELETE, stoi(bulletID));
}
void TutorialGame::_respawnHandle(const EVENT* pEvent, DWORD64 dwOwnerData)
{
	GameWorld* world = (GameWorld*)dwOwnerData;
	string worldID = pEvent->vArg[0];
 	Player* player = static_cast<Player*>(world->FindObjectbyID(stoi(worldID)));
	player->GetPhysicsXObject()->CTrans(PxExtendedVec3(20,5,10));
}
void TutorialGame::_colorzoneHandle(const EVENT* pEvent, DWORD64 dwOwnerData)
{
	GameWorld* world = (GameWorld*)dwOwnerData;
	string worldID = pEvent->vArg[0];
	string color = pEvent->vArg[1];
	Player* player = static_cast<Player*>(world->FindObjectbyID(stoi(worldID)));
	float currenthealth = player->GetPlayerPro()->health;

	switch (stoi(color))
	{case 0:
		player->GetPlayerPro()->health = currenthealth + 0.1 > 100 ? 100 : currenthealth + 0.1;
		break;
	case 1:
		player->GetPlayerPro()->damage = 100.0f;
		break;
	case 2:
		player->GetPlayerPro()->speed = 1.5f;
		break;
	case 3:
		player->GetPlayerPro()->Reset(player->GetRole());
	}
	
}
void TutorialGame::_damageRangeHandle(const EVENT* pEvent, DWORD64 dwOwnerData) {
	string grenadeID = pEvent->vArg[0];
	GameWorld* world = (GameWorld*)dwOwnerData;
	Grenade* grenade = static_cast<Grenade*>(world->FindObjectbyID(stoi(grenadeID)));
	int playerID = grenade->GetPlayerID();
	Player* player = static_cast<Player*>(world->FindObjectbyID(playerID));
}

void TutorialGame::UpdateGameObjects(float dt)
{
	world->OperateOnContents(
		[&](GameObject* o)
		{
			if (o->IsActive())
			{
				o->Update(dt);
			}
		}
	);
}


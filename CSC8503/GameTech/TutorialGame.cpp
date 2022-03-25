#include "TutorialGame.h"
#include "../CSC8503Common/GameWorld.h"
#include "../CSC8503Common/GameObjectGenerator.h"
#include "../CSC8503Common/AssetManager.h"
#include "../../Plugins/OpenGLRendering/OGLMesh.h"
#include "../../Plugins/OpenGLRendering/OGLShader.h"
#include "../../Plugins/OpenGLRendering/OGLTexture.h"
#include "../../Common/ShaderManager.h"
#include "../../Common/TextureLoader.h"
#include "../../Common/Assets.h"
#include "../GameTech/TutorialMenu.h"
#include "../../Gameplay/ePlayerRole.h"
#include "../../Gameplay/GameObjects.h"
#include "../../Gameplay/Bullet.h"
#include "../../Gameplay/Grenade.h"
#include "../GameTech/DebugMode.h"

using namespace NCL;
using namespace CSC8503;

TutorialGame::TutorialGame()
{ 
	eventSystem = new YiEventSystem();
	world = new GameWorld();
	Window::GetWindow()->GetRenderer()->SetWorld(world);
	renderer = dynamic_cast<GameTechRenderer*>(Window::GetWindow()->GetRenderer());
	physicsX = new PhysicsXSystem(*world);

	Debug::SetRenderer(renderer);
	InitialiseUI();
	InitialiseAssets();
	AudioManager::Startup();
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
	AudioManager::GetInstance().Play_Sound(AudioManager::SoundPreset::SoundPreset_InGame);
	InitWorld();
}
void TutorialGame::InitialiseAssets() {
	InitAbilityContainer();
	GameObjectGenerator g;
	std::string worldFilePath = Assets::DATADIR;
	worldFilePath.append("world.json");
	g.Generate(worldFilePath.c_str(), *world);
	InitWorld();
	//InitPlayer(Vector3(20, 3, 0), GameObjectType_team2);
	//InitPlayer(Vector3(20, 3, -20), GameObjectType_team1,true);
	RegisterEventHandles();
}

void TutorialGame::InitialiseUI()
{
	gameUI = new GameUI();
	renderer->SetUI(gameUI);
}
TutorialGame::~TutorialGame()	{
	AudioManager::Cleanup();
	delete physicsX;
	delete renderer;
	delete world;
	delete gameUI;
	delete localPlayer;
	delete abilityContainer;
}

void TutorialGame::UpdateGame(float dt)
{
#ifndef DEBUG
	TIMER_START(x);
    eventSystem->ProcessAllEvent();
    TIMER_STOP(x);
    Debug::Print("EventSystem:" + std::to_string(TIMER_MSEC(x)) + "ms", Vector2(55, 95));
	
	TIMER_RESET(x);
	AudioManager::GetInstance().Update(dt);
	TIMER_STOP(x);
	Debug::Print("AudioManager:" + std::to_string(TIMER_MSEC(x)) + "ms", Vector2(55, 100));

	TIMER_RESET(x);
	UpdateGameObjects(dt);
	TIMER_STOP(x);
	Debug::Print("GameObject:" + std::to_string(TIMER_MSEC(x)) + "ms", Vector2(55, 90));

	TIMER_RESET(x);
	physicsX->Update(dt);
	TIMER_STOP(x);
	Debug::Print("PhysicsX:" + std::to_string(TIMER_MSEC(x)) + "ms", Vector2(55, 85));
	#else
	eventSystem->ProcessAllEvent();
	AudioManager::GetInstance().Update(dt);

	UpdateGameObjects(dt);
	physicsX->Update(dt);
	#endif

	HUDUpdate(dt);

	world->UpdateWorld(dt);
	renderer->Update(dt);
	renderer->Render();

	Debug::FlushRenderables(dt);

#ifdef DEBUG_MODE
	physicsX->DrawCollisionLine();
	CalculateFrameRate(dt);
	Memoryfootprint();
#endif // DEBUG
}

void TutorialGame::InitAbilityContainer() {
	abilityContainer = new AbilityContainer();
}

Player* TutorialGame::InitPlayer(Vector3 pos, GameObjectType team)
{
	auto player = new Player(abilityContainer, team);
	player->GetTransform()
		.SetScale(Vector3(4, 4, 4))
		.SetPosition(pos);
	player->InitAllComponent();
	player->SetRenderObject(new RenderObject(&player->GetTransform(), AssetManager::GetInstance()->GetMesh("Male_Guard.msh"), AssetManager::GetInstance()->GetTexture("checkerboard"), ShaderManager::GetInstance()->GetShader("default")));
	if (team == GameObjectType_team1)
		player->GetRenderObject()->SetColour(Vector4(0,1,0,1));
	else
		player->GetRenderObject()->SetColour(Vector4(1,0,0,1));
	world->AddGameObject(player);
	return player;
}

Creeper* NCL::CSC8503::TutorialGame::InitCreeper(Vector3 pos)
{
	auto creeper = new Creeper(*world);
	creeper->GetTransform()
		.SetScale(Vector3(4, 4, 4))
		.SetPosition(pos);
	creeper->InitAllComponent();
	creeper->SetRenderObject(new RenderObject(&creeper->GetTransform(), 
		AssetManager::GetInstance()->GetMesh("Male_Guard.msh"), AssetManager::GetInstance()->GetTexture("checkerboard"), ShaderManager::GetInstance()->GetShader("default")));
	creeper->GetRenderObject()->SetColour(Vector4(1, 0.4, 0.7, 1));
	world->AddGameObject(creeper);
	return creeper;
}

void NCL::CSC8503::TutorialGame::AddPaint(GameObject* object, GameObject *collisionSurface, Vector4 color)
{
	GameObject* disc = new GameObject();
	disc->GetTransform()
		.SetScale(Vector3(15, 0.01f, 15))
		.SetPosition(PhysXConvert::PxVec3ToVector3(object->GetPhysicsXObject()->collisionPoint))
		.SetOrientation(collisionSurface->GetTransform().GetOrientation());

	disc->SetRenderObject(new RenderObject(&disc->GetTransform(), 
		AssetManager::GetInstance()->GetMesh("Cylinder.msh"), 
		AssetManager::GetInstance()->GetTexture("paint"), 
		ShaderManager::GetInstance()->GetShader("default")));
	disc->GetRenderObject()->SetColour(color);

	world->AddGameObject(disc);
}

void TutorialGame::InitWorld()
{
	InitDefaultFloor();
}

void TutorialGame::InitDefaultFloor()
{
	Floor* floor = new Floor();

	floor->GetTransform()
		.SetScale(Vector3(500, 1, 500))
		.SetPosition(Vector3(-250, 10, 0));

	floor->InitAllComponent();
	world->AddGameObject(floor);

}

void TutorialGame::RegisterEventHandles()
{
	eventSystem->RegisterEventHandle("OPEN_FIRE", _openFirHandle,(DWORD64)this);
	eventSystem->RegisterEventHandle("BHF", _paint, (DWORD64)this);


	eventSystem->RegisterEventHandle("THROW_GRENADE", _GrenadeHandle, (DWORD64)this);
	eventSystem->RegisterEventHandle("OBJECT_DELETE", _deleteHandle,(DWORD64)this);
	eventSystem->RegisterEventHandle("HIT", _HitHandle, (DWORD64)this);
	eventSystem->RegisterEventHandle("RESPWAN", _respawnHandle, (DWORD64)world);
	eventSystem->RegisterEventHandle("COLOR_ZONE", _colorzoneHandle, (DWORD64)world);
	eventSystem->RegisterEventHandle("DAMAGE_RANGE", _damageRangeHandle, (DWORD64)this);
}

#ifndef ORBIS
void TutorialGame::HUDUpdate(float dt)
{
	if (not localPlayer)return;
	PlayerPro* playerPro = localPlayer->GetPlayerPro();
	TimeStack* timeStack = localPlayer->GetTimeStack();
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
	//	YiEventSystem::GetMe()->PushEvent(GAME_OVER);
	}
	renderer->DrawString("PlayerKill: " + std::to_string(localPlayer->GetPlayerPro()->playerKill), Vector2(70, 85));
	

	if(timeStack->dashCooldown>0)
		renderer->DrawString("Dash CD: " + std::to_string(timeStack->dashCooldown), Vector2(5, 80));
	else
		renderer->DrawString("Dash ready!", Vector2(5, 80));

	if (timeStack->grenadeCD > 0)
		renderer->DrawString("Grenade CD: " + std::to_string(timeStack->grenadeCD), Vector2(5, 60));
	else
		renderer->DrawString("Grenade ready!", Vector2(5, 60));


	Vector3 position = localPlayer->GetTransform().GetPosition()+Vector3(0,5,0);
	Vector2 screenSize = Window::GetWindow()->GetScreenSize();

#define TARGET_OFF 20.0f
	Vector3 target = PhysicsXSystem::getMe()->ScreenToWorld(*localPlayer->GetComponentCamera()->camera, screenSize / 2.0f);
	Vector3 targetleft= PhysicsXSystem::getMe()->ScreenToWorld(*localPlayer->GetComponentCamera()->camera, 
		screenSize / 2.0f+Vector2(-TARGET_OFF,0));
	Vector3 targetright = PhysicsXSystem::getMe()->ScreenToWorld(*localPlayer->GetComponentCamera()->camera,
		screenSize / 2.0f + Vector2(TARGET_OFF, 0));
	Vector3 targettop = PhysicsXSystem::getMe()->ScreenToWorld(*localPlayer->GetComponentCamera()->camera,
		screenSize / 2.0f + Vector2(0, TARGET_OFF));
	Vector3 targetbot = PhysicsXSystem::getMe()->ScreenToWorld(*localPlayer->GetComponentCamera()->camera,
		screenSize / 2.0f + Vector2(0, -TARGET_OFF));
	renderer->DrawLine(position, target, Vector4(0,1,0,1));
	renderer->DrawLine(targetleft, targetright, Vector4(0, 1, 0, 1));
	renderer->DrawLine(targettop, targetbot, Vector4(0, 1, 0, 1));
}
#endif

void TutorialGame::CalculateFrameRate(float dt) {
	float currentTime = GetTickCount64() * 0.001f;
	++framesPerSecond;
	if (currentTime - lastTime > 1.0f)
	{
		lastTime = currentTime;
		FPS = framesPerSecond;
		framesPerSecond = 0;
	}
	renderer->DrawString("FPS: "+std::to_string(int(FPS)), Vector2(55, 65));
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

	bullet->SetRenderObject(new RenderObject(&bullet->GetTransform(), AssetManager::GetInstance()->GetMesh("Sphere.msh"),
		AssetManager::GetInstance()->GetTexture("checkerboard"), ShaderManager::GetInstance()->GetShader("default")));

	game->world->AddGameObject(bullet);

	game->physicsX->addActor(*bullet);
	bullet->GetPhysicsXObject()->SetLinearVelocity(dir.shootDir * 250.0f);
	AudioManager::GetInstance().Play_Sound(AudioManager::SoundPreset::SoundPreset_Fire, false);
}
void TutorialGame::_paint(const EVENT* pEvent, DWORD64 dwOwnerData) {
	TutorialGame* game = (TutorialGame*)dwOwnerData;
	string bulletID = pEvent->vArg[0];
	string wallID = pEvent->vArg[1];
	Bullet* bullet =(Bullet*)game->world->FindObjectbyID(stoi(bulletID));
	GameObject* wall = game->world->FindObjectbyID(stoi(wallID));
	if (not bullet)return;
	int shootID = bullet->GetShooterID();
	PlayerRole pColor = ((Player*)game->world->FindObjectbyID(shootID))->GetRole();

	Vector4 color;
	switch (pColor)
	{
	case PlayerRole::PlayerRole_blue:
		color = Vector4(0, 0, 1, 1);
		break;
	case PlayerRole::PlayerRole_green:
		color = Vector4(0, 1, 0, 1);
		break;
	case PlayerRole::PlayerRole_red:
		color = Vector4(1, 0, 0, 1);
		break;
	}
	
	if (not bullet)
		return;
	AudioManager::GetInstance().Play_Sound(AudioManager::SoundPreset::SoundPreset_Collision, false);
	game->AddPaint(bullet, wall, color);
	YiEventSystem::GetMe()->PushEvent(OBJECT_DELETE, bullet->GetWorldID());
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
	grenade->SetRenderObject(new RenderObject(&grenade->GetTransform(), AssetManager::GetInstance()->GetMesh("Cube.msh"),
		AssetManager::GetInstance()->GetTexture("checkerboard"), ShaderManager::GetInstance()->GetShader("default")));

	game->world->AddGameObject(grenade);

	auto func = [](GameObject* object, Vector3 position) {AudioManager::GetInstance().Play_Sound(AudioManager::SoundPreset::SoundPreset_Collision, false); };
	grenade->SetCollisionFunction(func);

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
	TutorialGame* game = (TutorialGame*)dwOwnerData;

	Bullet* bullet = static_cast<Bullet*>(game->world->FindObjectbyID(stoi(bulletID)));
	if (not bullet)return;
	int shooterID = bullet->GetShooterID();

	Player* shooter = static_cast<Player*>(game->world->FindObjectbyID(shooterID));
	Player* hitobj = static_cast<Player*>(game->world->FindObjectbyID(stoi(hitID)));
	PlayerPro* playerPro = hitobj->GetPlayerPro();
	int health=hitobj->GetPlayerPro()->health;
	if (health > 0 and playerPro->health - bullet->GetDamage() <= 0) {
		std::cout << (std::to_string(shooter->GetWorldID()) + " --->" +
			std::to_string(hitobj->GetWorldID())) << std::endl;
		shooter->GetPlayerPro()->playerKill++;
		if (shooter->type = GameObjectType_team1)
		{
			game->team1Kill++;
		}
		else if (shooter->type == GameObjectType_team2)
		{
			game->team2Kill++;
		}
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
	if(player->GetPhysicsXObject())
		player->GetPhysicsXObject()->CTrans(PxExtendedVec3(-200, 50, 0));
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

	TutorialGame* game = (TutorialGame*)dwOwnerData;
	Player* enemy = nullptr;
	for (auto i : game->world->GetGameObjects()) {
		if (i->type == GameObjectType_team2) {
			enemy = static_cast<Player*>(i);

			Grenade* grenade = static_cast<Grenade*>(game->world->FindObjectbyID(stoi(grenadeID)));
			if (not grenade)return;
			int playerID = grenade->GetPlayerID();
			Player* player = static_cast<Player*>(game->world->FindObjectbyID(playerID));


			PlayerPro* playerPro = enemy->GetPlayerPro();
		

			Vector3 hitobjPos = enemy->GetTransform().GetPosition();
			Vector3 grenadePos = grenade->GetTransform().GetPosition();
			Vector3 dis = hitobjPos - grenadePos;
			float d = dis.Length();//distance between grenade and object
			if (d < 100.0) // damage range = 5 
			{
				PxRaycastBuffer hit;
				// move the 1.0 to the distance vector.
				bool status=game->physicsX->raycast(grenadePos + Vector3(0,1,0),dis.Normalised(),d, hit);
				if (status) {
					GameObject* obj=(GameObject*)hit.block.actor->userData;
					if (obj->type == GameObjectType_team2) {
						playerPro->health -= 25;
						std::cout << playerPro->health << std::endl;
						if (playerPro->health <= 0) {
							playerPro->health = 0;
							std::cout << (std::to_string(player->GetWorldID()) + " --->" +
								std::to_string(enemy->GetWorldID())) << std::endl;
							player->GetPlayerPro()->playerKill++;
							//game->team1Kill++;
		
						}
						YiEventSystem::GetMe()->PushEvent(OBJECT_DELETE, stoi(grenadeID));
					}
				}
				
			}
			else
			{
				YiEventSystem::GetMe()->PushEvent(OBJECT_DELETE, stoi(grenadeID));
			}
		}
	}	
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

int TutorialGame::GetTeamKill()
{
	for (auto i : world->GetGameObjects())
	{
		if (localPlayer->type == GameObjectType_team1)
		{
			team1Kill += localPlayer->GetPlayerPro()->playerKill;
			return team1Kill;
		}
		if (localPlayer->type == GameObjectType_team2)
		{
			team2Kill += localPlayer->GetPlayerPro()->playerKill;
			return team2Kill;
		}
	}
}
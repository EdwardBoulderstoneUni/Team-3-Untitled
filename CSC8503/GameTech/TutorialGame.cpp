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

using namespace NCL;
using namespace CSC8503;

TutorialGame* TutorialGame::p_self = NULL;
TutorialGame::TutorialGame()
{ 
	eventSystem = new YiEventSystem();
	p_self = this;
	world = new GameWorld();
	renderer = new GameTechRenderer(*world);
	physicsX = new PhysicsXSystem(*world);
	forceMagnitude = 10.0f;
	inSelectionMode = false;
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
	//Loading Screen start
	ShaderManager::GetInstance()->Init();
	AssetManager::GetInstance()->Init();
	//Loading End
	InitAbilityContainer();
	GameObjectGenerator g;
	std::string worldFilePath = Assets::DATADIR;
	worldFilePath.append("world.json");
	g.Generate(worldFilePath.c_str(), *world);

	InitWorld();
	InitPlayer(Vector3(-250, 10, 0), GameObjectType_team2);
	InitPlayer(Vector3(-250, 10, -20), GameObjectType_team1,true);
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
	if (lockedObject != nullptr)
	{
		Vector3 objPos = lockedObject->GetTransform().GetPosition();
		Vector3 camPos = objPos + lockedOffset;

		Matrix4 temp = Matrix4::BuildViewMatrix(camPos, objPos, Vector3(0, 1, 0));

		Matrix4 modelMat = temp.Inverse();

		Quaternion q(modelMat);
		Vector3 angles = q.ToEuler(); //nearly there now!

		world->GetMainCamera()->SetPosition(camPos);
		world->GetMainCamera()->SetPitch(angles.x);
		world->GetMainCamera()->SetYaw(angles.y);
	}
	
	HUDUpdate(dt);
	world->UpdateWorld(dt);
	renderer->Update(dt);
	renderer->Render();

	Debug::FlushRenderables(dt);
}

void TutorialGame::InitAbilityContainer() {
	abilityContainer = new AbilityContainer();
}

void TutorialGame::InitPlayer(Vector3 pos, GameObjectType team, bool islocal)
{
	player = new Player(PlayerRole_blue, abilityContainer, team, islocal);
	camFollowPlayer = true;

	player->GetTransform()
		.SetScale(Vector3(4, 4, 4))
		.SetPosition(pos);
	player->InitAllComponent();
	player->SetRenderObject(new RenderObject(&player->GetTransform(), AssetManager::GetInstance()->GetMesh("Female_Guard.msh"), AssetManager::GetInstance()->GetTexture("checkerboard"), ShaderManager::GetInstance()->GetShader("default")));
	world->SetMainCamera(player->GetComponentCamera()->camera);
	world->AddGameObject(player);
}

void TutorialGame::InitWorld()
{
	InitDefaultFloor();
	AudioManager::Startup();
}

void TutorialGame::InitDefaultFloor()
{
	Floor* floor = new Floor();

	floor->GetTransform()
		.SetScale(Vector3(500, 1, 500))
		.SetPosition(Vector3(-250,10,0));

	floor->InitAllComponent();
	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, AssetManager::GetInstance()->GetTexture("checkerboard"), ShaderManager::GetInstance()->GetShader("default")));
	world->AddGameObject(floor);
}

void NCL::CSC8503::TutorialGame::RegisterEventHandles()
{
	eventSystem->RegisterEventHandle("OPEN_FIRE", _openFirHandle);
	eventSystem->RegisterEventHandle("OBJECT_DELETE", _deleteHandle);
	eventSystem->RegisterEventHandle("HIT", _HitHandle);
}

void NCL::CSC8503::TutorialGame::HUDUpdate(float dt)
{
	Player* player = TutorialGame::getMe()->player;
	renderer->DrawString("Ammo Left: " + std::to_string(player->GetAmmo()), Vector2(5, 90));
	if (player->GetAmmo() == 0) {
		renderer->DrawString("Press R to reload. ", Vector2(30, 40));
	}

	renderer->DrawString("Health: " + std::to_string(player->GetHealth()), Vector2(5, 85));

	if (tLeft >= 0) {
		tLeft -= dt;
		int m = tLeft / 60;
		int s = int(tLeft) % 60;
		renderer->DrawString("Time Remaining: " + std::to_string(m) + "m" + std::to_string(s) + "s", Vector2(30, 10));
	}
	else {
		isEnd = true; // Game end.
		renderer->DrawString("Time up!", Vector2(45, 50));
	}

	renderer->DrawString("Score: " + std::to_string(player->GetScore()), Vector2(70, 85));
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

void TutorialGame::_openFirHandle(const EVENT* pEvent, UINT dwOwnerData)
{
	string worldID = pEvent->vArg[0];
	Player* player = static_cast<Player*>(TutorialGame::getMe()->world->FindObjectbyID(stoi(worldID)));
	Vector3 positon = player->GetTransform().GetPosition();
	Vector3 forward = player->GetForward();

	auto bullet = new Bullet(*player);

	auto sphereSize = Vector3(1.0f, 1.0f, 1.0f);

	bullet->GetTransform()
		.SetScale(sphereSize)
		.SetPosition(positon + forward * 15);
	bullet->InitAllComponent();
	bullet->SetRenderObject(new RenderObject(&bullet->GetTransform(), TutorialGame::getMe()->sphereMesh,
		AssetManager::GetInstance()->GetTexture("checkerboard"), ShaderManager::GetInstance()->GetShader("default")));

	TutorialGame::getMe()->world->AddGameObject(bullet);

	//auto func = [](GameObject* object, Vector3 position) {TutorialGame::getMe()->AddPaint(position); };
	//bullet->SetCollisionFunction(func);
	TutorialGame::getMe()->physicsX->addActor(*bullet);
	bullet->GetPhysicsXObject()->SetLinearVelocity(forward * 50.0f);
}

void NCL::CSC8503::TutorialGame::_deleteHandle(const EVENT* pEvent, UINT dwOwnerData)
{
	string worldID = pEvent->vArg[0];
	GameObject* temp= TutorialGame::getMe()->world->FindObjectbyID(stoi(worldID));
	TutorialGame::getMe()->physicsX->deleteActor(*temp);
	TutorialGame::getMe()->world->RemoveGameObject(temp);
}
void NCL::CSC8503::TutorialGame::_HitHandle(const EVENT* pEvent, UINT dwOwnerData)
{
	string bulletID = pEvent->vArg[0];
	string hitID = pEvent->vArg[1];

	Bullet* bullet = static_cast<Bullet*>(TutorialGame::getMe()->world->FindObjectbyID(stoi(bulletID)));
	if (not bullet)return;
	int shooterID = bullet->GetShooterID();

	Player* shooter = static_cast<Player*>(TutorialGame::getMe()->world->FindObjectbyID(shooterID));
	Player* hitobj = static_cast<Player*>(TutorialGame::getMe()->world->FindObjectbyID(stoi(hitID)));

	shooter->AddScore(10);
	hitobj->TakeDamage(bullet->GetDamage());
	if (hitobj->IsDead())
		std::cout<<(std::to_string(shooter->GetWorldID()) + " --->" +
			std::to_string(hitobj->GetWorldID()))<<std::endl;
	YiEventSystem::GetMe()->PushEvent(OBJECT_DELETE, stoi(bulletID));
}
void NCL::CSC8503::TutorialGame::UpdateGameObjects(float dt)
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


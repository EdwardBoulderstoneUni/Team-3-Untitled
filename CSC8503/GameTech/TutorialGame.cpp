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

	basicTex = (OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png");
	basicShader = new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl");

	ShaderManager::GetInstance()->Init();
	AssetManager::GetInstance()->Init();
	InitAbilityContainer();
	GameObjectGenerator g;
	std::string worldFilePath = Assets::DATADIR;
	worldFilePath.append("worldTest.json");
	g.Generate(worldFilePath.c_str(), *world);



	//world->GetMainCamera()->SetNearPlane(0.1f);
	//world->GetMainCamera()->SetFarPlane(500.0f);
	//world->GetMainCamera()->SetPitch(-15.0f);
	//world->GetMainCamera()->SetYaw(315.0f);
	//world->GetMainCamera()->SetPosition(Vector3(-60, 40, 60));


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

void TutorialGame::InitPlayer(Vector3 pos, GameObjectType team,bool islocal)
{
	player = new Player(PlayerRole_blue, abilityContainer, team,islocal);
	camFollowPlayer = true;

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
	InitDefaultFloor();
	
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

void TutorialGame::InitDefaultFloor()
{
	Floor* floor = new Floor();

	floor->GetTransform()
		.SetScale(Vector3(150, 1, 150))
		.SetPosition(Vector3(0,0,0));

	floor->InitAllComponent();

	floor->SetRenderObject(new RenderObject(&floor->GetTransform(), cubeMesh, basicTex, basicShader));

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

GameObject* NCL::CSC8503::TutorialGame::AddPaint(const Vector3& position)
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
		TutorialGame::getMe()->basicTex, TutorialGame::getMe()->basicShader));

	TutorialGame::getMe()->world->AddGameObject(bullet);

	auto func = [](GameObject* object, Vector3 position) {TutorialGame::getMe()->AddPaint(position); };
	bullet->SetCollisionFunction(func);
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


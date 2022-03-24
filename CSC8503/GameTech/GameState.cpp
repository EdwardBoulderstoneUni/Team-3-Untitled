#include "../GameTech/GameState.h"
#include "../CSC8503Common/AssetManager.h"
#include "../../Common/ShaderManager.h"
#include "../../Common/TextureLoader.h"
#include "NetworkedGame.h"

#pragma region Start State
PushdownState::PushdownResult StartState::OnUpdate(float dt, PushdownState** newState)
{
	if (start_menu->EnterGame) {
		game->SetMultiMode();

		*newState = new GamingState(game);
		return PushdownResult::Push;
	}
	else if (start_menu->QuitGame) {
		exit(0);
	}
	AudioManager::GetInstance().Update(dt);
	game->StartRender();
	return PushdownResult::NoChange;
}
void StartState::OnAwake() {
	game->GetUI()->PushMenu(start_menu);
	Window::GetWindow()->ShowOSPointer(true);
	Window::GetWindow()->LockMouseToWindow(true);
}

void StartState::OnSleep() {
	game->GetUI()->RemoveMenu(start_menu);
}

#pragma endregion

#pragma region Gaming State
void GamingState::OnAwake() {
	game->GetUI()->PushMenu(debug_menu);
	Window::GetWindow()->ShowOSPointer(false);
	Window::GetWindow()->LockMouseToWindow(true);
}

void GamingState::OnSleep() {
	game->GetUI()->RemoveMenu(debug_menu);
}

PushdownState::PushdownResult GamingState::OnUpdate(float dt, PushdownState** newState) {
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE))
	{
		*newState = new PauseState(game);
		return PushdownResult::Push;
	}
	
	if (game->tLeft <= 0)
	{
		*newState = new EndState(game);
		return PushdownResult::Push;
	}
	
	game->UpdateGame(dt);
	return PushdownResult::NoChange;
}
#pragma endregion

#pragma region Pause State
void PauseState::OnAwake() {
	game->GetUI()->PushMenu(pause_menu);
	Window::GetWindow()->ShowOSPointer(true);
	Window::GetWindow()->LockMouseToWindow(true);
}

void PauseState::OnSleep() {
	game->GetUI()->RemoveMenu(pause_menu);
}

PauseState::PauseState(TutorialGame* tg) : game(tg)
{
	pause_menu.reset(new PauseMenu(game));
 
}

PushdownState::PushdownResult PauseState::OnUpdate(float dt, PushdownState** newState) {

	AudioManager::GetInstance().Update(dt);

	if (pause_menu->QuitGame) {
		exit(0);
	}
	if (pause_menu->Cancel) {
		AudioManager::GetInstance().Play_Sound(AudioManager::SoundPreset_InGame);
		return PushdownResult::Pop;
	}

	game->StartRender();
	return PushdownResult::NoChange;
}

#pragma endregion

#pragma region End State
void EndState::OnAwake() {
	game->GetUI()->PushMenu(end_menu);
	Window::GetWindow()->ShowOSPointer(true);
	Window::GetWindow()->LockMouseToWindow(true);
}

void EndState::OnSleep() {
	game->GetUI()->RemoveMenu(end_menu);
}

PushdownState::PushdownResult EndState::OnUpdate(float dt, PushdownState** newState)
{

	if (end_menu->QuitGame) {
		exit(0);
	}

	game->StartRender();
	return PushdownResult::NoChange;
}

#pragma region Load State
LoadState::LoadState() {
	loadingGame = true;

	ShaderManager::GetInstance()->Init();
	world = new GameWorld();
	world->SetMainCamera(new Camera(0, 270, Vector3(-15, 0, 0)));
	Window::GetWindow()->GetRenderer()->SetWorld(world);
	renderer = dynamic_cast<GameTechRenderer*>(Window::GetWindow()->GetRenderer());

	auto loadFunc = [](const string& name, OGLMesh** into) {
		*into = new OGLMesh(name);
		(*into)->SetPrimitiveType(GeometryPrimitive::Triangles);
		(*into)->UploadToGPU();
	};

	loadFunc("cube.msh", &cubeMesh);

	scale = Vector3(15, 15, 15);
	object = new GameObject();
	object->GetTransform().SetScale(scale);
	object->SetRenderObject(new RenderObject(&object->GetTransform(),
		cubeMesh,
		(OGLTexture*)TextureLoader::LoadAPITexture("Logo.png"),
		ShaderManager::GetInstance()->GetShader("default")));

	world->AddGameObject(object);
	
	loadingThread = std::thread([this]() {
	AssetManager::GetInstance()->Init(); loadingGame = false;  });	
}

LoadState::~LoadState() {
	delete mesh;
	delete world;
	delete renderer;
	delete object;
}

void LoadState::LoadGame(float dt) {
	while (loadingGame) {
		dt += Window::GetWindow()->GetTimer()->GetTimeDeltaSeconds();
		scale += Vector3(0.1, 0.1, 0.1);
		Update(dt);
	}
	loadingThread.join();
	AssetManager::GetInstance()->UploadToGPU();
}

void LoadState::Update(float dt) {
	world->GetGameObjects()[0]->GetTransform().SetScale(scale);
	world->UpdateWorld(dt);
	renderer->Update(dt);
	renderer->Render();
	std::cout << dt << std::endl;
}
#pragma endregion

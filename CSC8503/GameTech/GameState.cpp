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
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE) or game->tLeft<0)
	{
		*newState = new PauseState(game);
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

PushdownState::PushdownResult PauseState::OnUpdate(float dt, PushdownState** newState) {
	if (pause_menu->EnterGame) {
		game->SetMultiMode();
		game->tLeft = 900.0f;
		*newState = new GamingState(game);
		return PushdownResult::Pop;
	}
	else if (pause_menu->QuitGame) {
		exit(0);
	}
	if (pause_menu->Cancel) {
		return PushdownResult::Pop;
	}

	game->StartRender();
	return PushdownResult::NoChange;
}

#pragma endregion

#pragma region Load State
LoadState::LoadState() {
	loadingGame = true;

	world = new GameWorld();
	world->SetMainCamera(new Camera(0, 0, Vector3(-50, 0, -50)));
	Window::GetWindow()->GetRenderer()->SetWorld(world);
	renderer = dynamic_cast<GameTechRenderer*>(Window::GetWindow()->GetRenderer());
	renderer->DrawString("Loading: 0%" , Vector2(5, 95));
	Update(0.01f);

	mesh = new OGLMesh();
	mesh->GenerateSquare(mesh);
	mesh->SetPrimitiveType(GeometryPrimitive::Triangles);
	mesh->UploadToGPU(renderer);

	object = new GameObject();
	Update(0.01f);
	object->SetRenderObject(new RenderObject(&object->GetTransform(),
		mesh,
		(OGLTexture*)TextureLoader::LoadAPITexture("checkerboard.png"),
		new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl")));

	world->AddGameObject(object);
	renderer->DrawString("Loading: 5%" , Vector2(5, 95));
	Update(0.01f);

	
	ShaderManager::GetInstance()->Init();
	renderer->DrawString("Loading: 35%" , Vector2(5, 95));
	Update(0.01f);
	AssetManager::GetInstance()->Init();
	renderer->DrawString("Loading: 100%" , Vector2(5, 95));
	Update(0.01f);
	loadingGame = false; 
}

LoadState::~LoadState() {
	delete mesh;
	delete world;
	delete renderer;
	delete object;
}

void LoadState::LoadGame() {
	while (loadingGame) {
		Update(0.01f);
	}
}

void LoadState::Update(float dt) {
	world->UpdateWorld(dt);
	renderer->Update(dt);
	renderer->Render();
}
#pragma endregion

#include "../GameTech/GameState.h"
#include "../CSC8503Common/AssetManager.h"
#include "../../Common/ShaderManager.h"
#include "../../Common/TextureLoader.cpp"

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
LoadState::LoadState() {}

LoadState::~LoadState() {
	delete mesh;
	delete world;
	delete renderer;
	delete object;
}

void LoadState::OnAwake() {
	mesh = new OGLMesh();
	mesh->GenerateSquare(mesh);

	world = new GameWorld();
	renderer = new GameTechRenderer(*world);
	object = new GameObject();


	object->SetRenderObject(new RenderObject(&object->GetTransform(),
		mesh, 
		(OGLTexture*)TextureLoader::LoadAPITexture("Logo.png"), 
		new OGLShader("GameTechVert.glsl", "GameTechFrag.glsl")));

	ShaderManager::GetInstance()->Init();
	AssetManager::GetInstance()->Init();

	loadingGame = false;
}

void LoadState::OnSleep() {
}

PushdownState::PushdownResult LoadState::OnUpdate(float dt, PushdownState** newState) {
	if (!loadingGame) {
		return PushdownResult::Pop;
	}
	renderer->Update(dt);

	return PushdownResult::NoChange;
}

#pragma endregion

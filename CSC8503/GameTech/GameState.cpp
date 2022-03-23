#include "../GameTech/GameState.h"

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

PushdownState::PushdownResult PauseState::OnUpdate(float dt, PushdownState** newState) {
	if (pause_menu->EnterGame) {
		game->SetMultiMode();
		game->tLeft =30.0f;
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

#pragma endregion

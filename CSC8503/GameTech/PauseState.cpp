#include "../GameTech/PauseState.h"
#include "../GameTech/TutorialGame.h"
#include "../GameTech/GameUI.h"

using namespace NCL;
using namespace CSC8503;


PushdownState::PushdownResult InGameState::OnUpdate(float dt, PushdownState** newState)
{
	if (game->IsMainMenu() || game->IsFreezed())
	{
		*newState = new MainMenuState(this);
		return PushdownResult::Push;
	}
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE) || game->IsFreezed())
	{
		*newState = new PauseState(this);
		return PushdownResult::Push;
	}
	if (quit)
	{
		return PushdownResult::Pop;
	}

	return PushdownResult::NoChange;
}

void InGameState::OnAwake()
{
	//printf("Enter InGameState.\n");
	Window::GetWindow()->ShowOSPointer(false);
	Window::GetWindow()->LockMouseToWindow(true);

}

void InGameState::OnSleep()
{
	game->SetFreeze(true);
}

PushdownState::PushdownResult PauseState::OnUpdate(float dt, PushdownState** newState)
{
	if (pauseMenu->menuClose)
	{
		return PushdownResult::Pop;
	}
	else if (pauseMenu->quitGame)
	{
		gameState->quit = true;
		return PushdownResult::Pop;
	}
	else if (pauseMenu->singleMode)
	{
		gameState->game->SetSingleMode();
		return PushdownResult::Pop;
	}
	else if (pauseMenu->multiMode)
	{
		gameState->game->SetMultiMode();
		return PushdownResult::Pop;
	}
	return PushdownResult::NoChange;
}


//didn't run OnAwake,so no pause menu.
void PauseState::OnAwake()
{
	gameState->game->GetUI()->PushMenu(pauseMenu);
	Window::GetWindow()->ShowOSPointer(true);
	Window::GetWindow()->LockMouseToWindow(false);
}

void PauseState::OnSleep()
{
	gameState->game->GetUI()->RemoveMenu(pauseMenu);
	gameState->game->SetFreeze(false);
}

PushdownState::PushdownResult MainMenuState::OnUpdate(float dt, PushdownState** newState)
{

	if (mainGameMenu->quitGame)
	{
		gameState->quit = true;
		return PushdownResult::Pop;
	}
	else if (mainGameMenu->singleMode)
	{
		gameState->game->SetSingleMode();
		return PushdownResult::Pop;
	}
	else if (mainGameMenu->multiMode)
	{
		gameState->game->SetMultiMode();
		return PushdownResult::Pop;
	}
	return PushdownResult::NoChange;
}


//didn't run OnAwake,so no pause menu.
void MainMenuState::OnAwake()
{
	gameState->game->GetUI()->PushMenu(mainGameMenu);
	Window::GetWindow()->ShowOSPointer(true);
	Window::GetWindow()->LockMouseToWindow(false);
}

void MainMenuState::OnSleep()
{
	gameState->game->GetUI()->RemoveMenu(mainGameMenu);
	gameState->game->SetFreeze(false);
}


#include "../GameTech/PauseState.h"
#include "../GameTech/TutorialGame.h"
#include "../GameTech/GameUI.h"

using namespace NCL;
using namespace CSC8503;


PushdownState::PushdownResult InGameState::OnUpdate(float dt, PushdownState** newState)
{
	if (Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE) || game->IsFreezed())
	{
		*newState = new PauseState(this);
		return PushdownResult::Push;
	}
	if (quit)
	{
		//1111
		return PushdownResult::Pop;
	}
	return PushdownResult::NoChange;
}

void InGameState::OnAwake()
{
	//printf("Enter InGameState.\n");
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
}

void PauseState::OnSleep()
{
	gameState->game->GetUI()->RemoveMenu(pauseMenu);
	gameState->game->SetFreeze(false);
}

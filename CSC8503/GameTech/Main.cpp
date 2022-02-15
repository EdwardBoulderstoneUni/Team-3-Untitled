#include <random>
#include "../../Common/Window.h"
#include "../CSC8503Common/NavigationGrid.h"
#include "TutorialGame.h"

using namespace NCL;
using namespace CSC8503;
int main()
{
	Window* w = Window::CreateGameWindow("CSC8503 Game technology!", 1280, 720);

	if (!w->HasInitialised())
	{
		return -1;
	}
	srand(static_cast<int>(GetTickCount64()));
	w->ShowOSPointer(false);
	w->LockMouseToWindow(true);

	const auto game = new TutorialGame();
	const auto _ = Window::GetTimer()->GetTimeDeltaSeconds();
	while (w->UpdateWindow() && !Window::GetKeyboard()->KeyDown(KeyboardKeys::ESCAPE))
	{
		const float dt = Window::GetTimer()->GetTimeDeltaSeconds();
		if (dt > 0.1f)
		{
			std::cout << "Skipping large time delta" << std::endl;
			continue; //must have hit a breakpoint or something to have a 1 second frame time!
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::PRIOR))
		{
			w->ShowConsole(true);
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NEXT))
		{
			w->ShowConsole(false);
		}

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::T))
		{
			w->SetWindowPosition(0, 0);
		}

		w->SetTitle("Yianser frame time:" + std::to_string(1000.0f * dt));

		game->update_game(dt);
	}
	Window::DestroyGameWindow();
}

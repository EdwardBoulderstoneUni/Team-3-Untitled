#ifndef ORBIS
#include "../../Common/Window.h"
#else
#include "../../GNMRendering/PS4Window.h"
#endif

#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/StateTransition.h"
#include "../CSC8503Common/State.h"


#include "TutorialGame.h"
#include "NetworkedGame.h"
using namespace NCL;
using namespace CSC8503;

/*

The main function should look pretty familar to you!
We make a window, and then go into a while loop that repeatedly
runs our 'game' until we press escape. Instead of making a 'renderer'
and updating it, we instead make a whole game, and repeatedly update that,
instead.

This time, we've added some extra functionality to the window class - we can
hide or show the

*/
int main()
{
#ifndef ORBIS
	Window* w = Window::CreateGameWindow("CSC8503 Game technology!", 1280, 720);

	if (!w->HasInitialised())
	{
		return -1;
	}

#else
	Window* w = NCL::PS4::PS4Window::CreateGameWindow("CSC8503 Game technology!", 1280, 720);
#endif

	srand(time(nullptr));
	w->ShowOSPointer(false);
	w->LockMouseToWindow(true);

	auto g = new TutorialGame();
	//auto g = new NetworkedGame(); //This is for network game  
	w->GetTimer()->GetTimeDeltaSeconds(); //Clear the timer so we don't get a larget first dt!
	while (w->UpdateWindow() /*&& !Window::GetInterface()->button_down(quit)*/)
	{
		float dt = w->GetTimer()->GetTimeDeltaSeconds();
		if (dt > 0.1f)
		{
			std::cout << "Skipping large time delta" << std::endl;
			continue; //must have hit a breakpoint or something to have a 1 second frame time!
		}
	/*	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::PRIOR))
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
		}*/

		w->SetTitle("Gametech frame time:" + std::to_string(1000.0f * dt));

		g->UpdateGame(dt);
	}
	Window::DestroyGameWindow();
}
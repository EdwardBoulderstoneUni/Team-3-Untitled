#include "../../Common/Window.h"

#include "../CSC8503Common/StateMachine.h"
#include "../CSC8503Common/StateTransition.h"
#include "../CSC8503Common/State.h"

#include "../CSC8503Common/NavigationGrid.h"

#include "TutorialGame.h"
#include "PauseMenu.h"
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
int main() {
	srand(time(0));

	Window* w = Window::CreateGameWindow("CSC8503 Game technology!", 1280, 720);

	if (!w->HasInitialised()) {
		return -1;
	}

	w->ShowOSPointer(true);
	w->LockMouseToWindow(true);

	RendererBase* renderer = new GameTechRenderer();
	w->SetRenderer(renderer);

	w->GetTimer()->GetTimeDeltaSeconds();
	float dt = w->GetTimer()->GetTimeDeltaSeconds();
	PushdownMachine loadMachine(new LoadState());

	/*auto g = new TutorialGame();
	PushdownMachine machine(new StartState(g));*/

	while (w->UpdateWindow()) {
		dt = w->GetTimer()->GetTimeDeltaSeconds();
		if (dt > 0.1f) {
			std::cout << "Skipping large time delta" << std::endl;
			continue; //must have hit a breakpoint or something to have a 1 second frame time!
		}
		/*if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::PRIOR)) {
			w->ShowConsole(true);
		}
		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::NEXT)) {
			w->ShowConsole(false);
		}

		if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::T)) {
			w->SetWindowPosition(0, 0);
		}
		*/
		w->SetTitle("Gametech frame time:" + std::to_string(1000.0f * dt));

		loadMachine.Update(dt);
	}
	//delete g;
	Window::DestroyGameWindow();
}
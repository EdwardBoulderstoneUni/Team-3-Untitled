#pragma once
#include "../../Common/Window.h"
#include "../CSC8503Common/PushdownState.h"
#include "../CSC8503Common/PushdownMachine.h"
#include "../GameTech/MainMenu.h"
#include "TutorialGame.h"
#include "TutorialMenu.h"
#include "../GameTech/EndingMenu.h"
#include "../GameTech/PauseMenu.h"
#include <thread>
using namespace NCL;
using namespace CSC8503;

class PauseState : public PushdownState {
public:
	PauseState(TutorialGame* tg); 
	~PauseState() {}
	PushdownResult OnUpdate(float dt, PushdownState** newState) override;

	void OnAwake() override;
	void OnSleep() override;

private:
	TutorialGame* game;

	std::shared_ptr<PauseMenu> pause_menu;
};

class GamingState : public PushdownState {
public:
	GamingState(TutorialGame* tg) : game(tg) { debug_menu.reset(new TutorialMenu(game)); }
	~GamingState() {}
	PushdownResult OnUpdate(float dt, PushdownState** newState) override;

	void OnAwake() override;
	void OnSleep() override;

private:
	TutorialGame* game;

	std::shared_ptr<TutorialMenu> debug_menu;
};

class StartState : public PushdownState {
public:
	StartState(TutorialGame* tg) : game(tg) { start_menu.reset(new MainMenu()); }
	~StartState() {}
	PushdownResult OnUpdate(float dt, PushdownState** newState) override;

	void OnAwake() override;
	void OnSleep() override;

private:
	TutorialGame* game;

	std::shared_ptr<MainMenu> start_menu;
};

class EndState : public PushdownState {
public:
	EndState(TutorialGame* tg) : game(tg) { end_menu.reset(new EndingMenu(game)); }
	~EndState() {}
	PushdownResult OnUpdate(float dt, PushdownState** newState) override;

	void OnAwake() override;
	void OnSleep() override;

private:
	TutorialGame* game;

	std::shared_ptr<EndingMenu> end_menu;
};

class LoadState {
public:
	LoadState();
	~LoadState();

	void LoadGame(float dt);
	void Update(float dt);
private:
	bool loadingGame = true;
	OGLMesh* mesh;
	OGLMesh* cubeMesh = nullptr;

	GameWorld* world;
	GameTechRenderer* renderer;
	GameObject* object;
	Vector3 scale;
	std::thread loadingThread;
};

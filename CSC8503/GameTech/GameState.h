#pragma once
#include "../../Common/Window.h"
#include "../CSC8503Common/PushdownState.h"
#include "../CSC8503Common/PushdownMachine.h"
#include "../GameTech/MainMenu.h"
#include "TutorialGame.h"
#include "TutorialMenu.h"

using namespace NCL;
using namespace CSC8503;

class PauseState : public PushdownState {
public:
	PauseState(TutorialGame* tg) : game(tg) { pause_menu.reset(new MainMenu()); pause_menu->PauseMode = true; }
	~PauseState() {}
	PushdownResult OnUpdate(float dt, PushdownState** newState) override;

	void OnAwake() override;
	void OnSleep() override;

private:
	TutorialGame* game;

	std::shared_ptr<MainMenu> pause_menu;
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

class LoadState : public PushdownState {
public:
	LoadState();
	~LoadState();
	PushdownResult OnUpdate(float dt, PushdownState** newState) override;

	void OnAwake() override;
	void OnSleep() override;
private:
	bool loadingGame = true;

	OGLMesh* mesh;

	GameWorld* world;
	GameTechRenderer* renderer;
	GameObject* object;
};

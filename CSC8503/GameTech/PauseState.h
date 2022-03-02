#pragma once
#include "../../Common/Window.h"
#include "../CSC8503Common/PushdownState.h"
#include "../GameTech/PauseMenu.h"
#include "../GameTech/GameUI.h"
#include"../GameTech/MainMenu.h"

namespace NCL {
	namespace CSC8503 {
		class TutorialGame;

		class InGameState : public PushdownState
		{
		public:
			inline InGameState(TutorialGame* g) : game(g) {};
			~InGameState() {};

			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
			void OnSleep() override;

		private:
			TutorialGame* game;
			bool quit = false;
			friend class PauseState;
			friend class MainMenuState;
		};

		class PauseState : public PushdownState
		{
		public:
			inline PauseState(InGameState* gs) : gameState(gs)
			{
				pauseMenu.reset(new PauseMenu());
			};
			~PauseState() {};

			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
			void OnSleep() override;

		private:
			InGameState* gameState;
			std::shared_ptr<PauseMenu> pauseMenu;
		};

		class MainMenuState : public PushdownState
		{
		public:
			inline MainMenuState(InGameState* gs) : gameState(gs)
			{
				mainGameMenu.reset(new MainMenu());
			};
			~MainMenuState() {};

			PushdownResult OnUpdate(float dt, PushdownState** newState) override;
			void OnAwake() override;
			void OnSleep() override;

		private:
			InGameState* gameState;
			std::shared_ptr<MainMenu> mainGameMenu;
		};

	}
}

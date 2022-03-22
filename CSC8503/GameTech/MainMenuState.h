#pragma once
#include "../../Common/Window.h"
#include "../CSC8503Common/PushdownState.h"
#include "../GameTech/GameUI.h"
#include"../GameTech/MainMenu.h"
#include "TutorialGame.h"

namespace NCL {

	namespace CSC8503 {
		class MainMenuState : public PushdownState {
		public:
			MainMenuState(TutorialGame* g, GameTechRenderer* r);
			~MainMenuState() {}
			PushdownResult OnUpdate(float dt, PushdownState** newState) override;

			void OnAwake() override {}

		private:
			GameTechRenderer* renderer;
			TutorialGame* game;
			int currentChoice;
			int minChoice;
			int maxChoice;
		};


	}
}
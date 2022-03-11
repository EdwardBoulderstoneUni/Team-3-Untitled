#pragma once
#include "../GameTech/GameMenu.h"
#include "GameTechRenderer.h"

namespace NCL {
	namespace CSC8503 {
		class MainMenu : public GameMenu
		{
		public:
			MainMenu() :EnterGame(false), QuitGame(false), PauseMode(false), Cancel(false) {};
			virtual ~MainMenu() {};

			virtual void Draw() override;

			bool EnterGame;
			bool QuitGame;
			bool Cancel;

			bool PauseMode;
		};

	}
}


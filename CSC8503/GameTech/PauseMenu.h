#pragma once
#include "../GameTech/GameMenu.h"
#include "GameTechRenderer.h"

namespace NCL {
	namespace CSC8503 {
		class TutorialGame;
		class PauseMenu : public GameMenu
		{
		public:
			PauseMenu(TutorialGame* tg) : game(tg), QuitGame(false),Cancel(false) {};
			virtual ~PauseMenu() {};

			virtual void Draw() override;

			bool QuitGame;
			bool Cancel;
		protected:
			TutorialGame* game;


		};

	}
}




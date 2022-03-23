#pragma once	
#include "../GameTech/GameMenu.h"

namespace NCL {
	namespace CSC8503 {
		class TutorialGame;
		class EndingMenu : public GameMenu
		{
		public:
			EndingMenu(TutorialGame* tg) : game(tg),QuitGame(false) {};
			virtual ~EndingMenu() {};

			virtual void Draw() override;
			bool QuitGame;

		protected:
			TutorialGame* game;
			

		};

	}
}




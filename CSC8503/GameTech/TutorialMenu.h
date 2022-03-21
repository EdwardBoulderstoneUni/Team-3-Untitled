#pragma once	
#include "../GameTech/GameMenu.h"

namespace NCL {
	namespace CSC8503 {
		class TutorialGame;
		class TutorialMenu : public GameMenu
		{
		public:
			TutorialMenu(TutorialGame* inGame): game(inGame)	{};
			virtual ~TutorialMenu() {};

			virtual void Draw() override;

			
		protected:
			TutorialGame* game;
			bool isEntering=false;
		};

	}
}




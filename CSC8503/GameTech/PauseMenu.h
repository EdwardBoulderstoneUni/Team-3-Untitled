#pragma once
#include "../GameTech/GameMenu.h"

namespace NCL {
	namespace CSC8503 {
		class PauseMenu : public GameMenu
		{
		public:
			PauseMenu() {};
			virtual ~PauseMenu() {};

			virtual void Draw() override;

			bool singleMode = false;
			bool multiMode = false;
			bool menuClose = false;
			bool quitGame = false;
		};

	}
}




#pragma once
#include "../GameTech/GameMenu.h"
#include "GameTechRenderer.h"

namespace NCL {
	namespace CSC8503 {
		class MainMenu : public GameMenu
		{
		public:
			MainMenu() {};
			virtual ~MainMenu() {};

			virtual void Draw() override;

			bool singleMode = false;
			bool multiMode = false;
			bool quitGame = false;
		};

	}
}


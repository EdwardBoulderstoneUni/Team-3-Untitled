#pragma once
#pragma once
#include "../GameTech/GameMenu.h"
#include "GameTechRenderer.h"

namespace NCL {
	namespace CSC8503 {
		class LoadMenu : public GameMenu
		{
		public:
			LoadMenu() {};
			virtual ~LoadMenu() {};

			virtual void Draw() override;
		};

	}
}
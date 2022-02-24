#pragma once
#include <memory>
#include "../GameTech/imgui/imgui.h"

class GameMenu
{
public:
	GameMenu() {};
	virtual ~GameMenu() {};

	virtual void Draw() = 0;
};

typedef std::shared_ptr<GameMenu> GameMenuPtr;


class GameMenuDemo : public GameMenu
{
	virtual void Draw() override
	{
		bool showDemo = true;
		ImGui::ShowDemoWindow(&showDemo);
	};
};
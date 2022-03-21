#pragma once
#include <vector>
#include "GameMenu.h"
#include "../../Common/Win32Window.h"
#include "../../imgui/backends/imgui_impl_win32.h"
#include "../../imgui/backends/imgui_impl_opengl3.h"


struct ExInputResult;

class GameUI
{
public:
	GameUI();
	~GameUI();

	void PushMenu(const GameMenuPtr& menu);
	GameMenuPtr PopMenu();
	void RemoveMenu(const GameMenuPtr& menu);
	void ClearMenus();

	void UpdateUI() const;
	void DrawUI() const;

private:

	bool IsValid = false;
	std::vector<GameMenuPtr> menus;
};
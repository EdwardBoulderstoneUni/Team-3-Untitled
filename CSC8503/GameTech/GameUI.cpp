#include "../GameTech/GameUI.h"
#include "../../Common/Win32Window.h"
#include "../../Common/Window.h"
#include "./imgui/imgui.h"
#include "./imgui/backends/imgui_impl_win32.h"
#include "./imgui/backends/imgui_impl_opengl3.h"

using namespace NCL;

Win32Code::ExInputResult ImguiProcessInput(void* data);

GameUI::GameUI()
{
	Win32Code::Win32Window* win32_w = dynamic_cast<Win32Code::Win32Window*>(Window::GetWindow());
	if (!win32_w) return;
	win32_w->SetExtraMsgFunc(ImguiProcessInput);

	IsValid = true;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.Fonts->AddFontFromFileTTF("./fonts/Roboto-Medium.ttf", 16);
	//io.Fonts->AddFontDefault();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(win32_w->GetHandle());
	ImGui_ImplOpenGL3_Init("#version 400");

}

GameUI::~GameUI()
{
	if (!IsValid) return;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void GameUI::PushMenu(const GameMenuPtr& menu)
{
	menus.push_back(menu);
}

GameMenuPtr GameUI::PopMenu()
{
	GameMenuPtr menuToPop = menus.back();
	menus.pop_back();
	return menuToPop;
}

void GameUI::RemoveMenu(const GameMenuPtr& menu)
{
	auto iter = std::find(menus.begin(), menus.end(), menu);
	if (iter != menus.end())
		menus.erase(iter);
}

void GameUI::ClearMenus()
{
	menus.clear();
}

void GameUI::UpdateUI() const
{
	if (!IsValid) return;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	for (auto& menu : menus)
	{
		menu->Draw();
	}

	ImGui::Render();
}

void GameUI::DrawUI() const
{
	if (!IsValid) return;

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


Win32Code::ExInputResult ImguiProcessInput(void* data)
{
	ImGuiIO& io = ImGui::GetIO();
	RAWINPUT* rawInput = (RAWINPUT*)data;

	if (rawInput->header.dwType == RIM_TYPEMOUSE)
	{
		static int btDowns[5] = { RI_MOUSE_BUTTON_1_DOWN,
								  RI_MOUSE_BUTTON_2_DOWN,
								  RI_MOUSE_BUTTON_3_DOWN,
								  RI_MOUSE_BUTTON_4_DOWN,
								  RI_MOUSE_BUTTON_5_DOWN };

		static int btUps[5] = { RI_MOUSE_BUTTON_1_UP,
								RI_MOUSE_BUTTON_2_UP,
								RI_MOUSE_BUTTON_3_UP,
								RI_MOUSE_BUTTON_4_UP,
								RI_MOUSE_BUTTON_5_UP };

		for (int i = 0; i < 5; ++i) {
			if (rawInput->data.mouse.usButtonFlags & btDowns[i]) {
				io.MouseDown[i] = true;
			}
			else if (rawInput->data.mouse.usButtonFlags & btUps[i]) {
				io.MouseDown[i] = false;
			}
		}
	}
	else if (rawInput->header.dwType == RIM_TYPEKEYBOARD)
	{
		USHORT key = rawInput->data.keyboard.VKey;
		bool down = !(rawInput->data.keyboard.Flags & RI_KEY_BREAK);

		if (key < 256)
			io.KeysDown[key] = down;
		if (key == VK_CONTROL)
			io.KeyCtrl = down;
		if (key == VK_SHIFT)
			io.KeyShift = down;
		if (key == VK_MENU)
			io.KeyAlt = down;

	}
	return { false, io.WantCaptureKeyboard };
}
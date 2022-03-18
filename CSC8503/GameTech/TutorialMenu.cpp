#include "TutorialMenu.h"
#include "TutorialGame.h"

using namespace NCL;
using namespace CSC8503;

#define TUTORMENU_SIZE ImVec2(360,70)
#define TEXT_WIN_POS_OFF ImVec2(main_viewport->WorkPos.x+900,main_viewport->WorkPos.y+400)
#define TEXT_WIN_SIZE ImVec2(360,100)

#define TUTORIAL_MENU_TITLE "Tutorial Menu"
#define TEXT_WIN_TITLE "TEXT"
void TutorialMenu::Draw()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(TUTORMENU_SIZE, ImGuiCond_Always);

	if (!ImGui::Begin(TUTORIAL_MENU_TITLE, NULL, window_flags))
	{
		ImGui::End();
		return;
	}
	ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

	/*if (ImGui::CollapsingHeader("Physics"))
	{
		
		if (ImGui::Checkbox("Use Gravity", &(game->useGravity)))
			game->physics->UseGravity(game->useGravity);
		ImGui::SliderFloat("Push Force", &(game->forceMagnitude), 10.0f, 50000.0f);
	
	}*/

	if (ImGui::CollapsingHeader("Configuration"))
	{
		static bool full_screen = Window::GetWindow()->IsFullScreen();
		if (ImGui::Checkbox("Fullscreen", &full_screen))
			Window::GetWindow()->SetFullScreen(full_screen);
	}

	if (ImGui::CollapsingHeader("Help"))
	{
		ImGui::Text("Control Guide:");
		ImGui::BulletText("Use ESC to open pause menu.");
		ImGui::BulletText("Use A|S|D|W|Shift|Control to move camera.");
		ImGui::BulletText("Hold mouse right button to control camera rotation.");
		ImGui::BulletText("Use mouse left button to selected object.");
		ImGui::BulletText("Use space to push selected object.");
		ImGui::BulletText("Press L to lock/unlock selected object.");
		ImGui::BulletText("Use Up|Down|Left|Right to control the locked object.");
		ImGui::Separator();
	}
	if (Window::GetKeyboard()->KeyPressed(KeyboardKeys::RETURN)) {
		isEntering = !isEntering;
		Window::GetWindow()->ShowOSPointer(isEntering);
	}
	if (isEntering) {
		ImGui::Begin(TEXT_WIN_TITLE);
		ImGui::Text("This is some useful text.");
		ImGui::SetWindowPos(TEXT_WIN_POS_OFF, ImGuiCond_Always);
		ImGui::SetWindowSize(TEXT_WIN_SIZE, ImGuiCond_Always);
		static char buf[128] = "click on a button to set focus";
		ImGui::SetKeyboardFocusHere();
		ImGui::InputText("1", buf, IM_ARRAYSIZE(buf));
		ImGui::End();
	}
	ImGui::End();
}

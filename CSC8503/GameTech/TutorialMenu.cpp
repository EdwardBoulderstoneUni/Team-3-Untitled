#include "TutorialMenu.h"
#include "TutorialGame.h"

using namespace NCL;
using namespace CSC8503;

void TutorialMenu::Draw()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
	const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x, main_viewport->WorkPos.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(360, 280), ImGuiCond_Always);

	if (!ImGui::Begin("Tutorial Menu", NULL, window_flags))
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
	


	ImGui::End();
}

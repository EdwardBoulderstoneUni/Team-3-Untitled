#include "LoadMenu.h"

void LoadMenu::Draw()
{
	Debug::Print("YIANSER Loading Menu", Vector2(35, 25));

	const ImGuiViewport* mainVp = ImGui::GetMainViewport();

	//Draw background
	ImGui::SetNextWindowPos(ImVec2(mainVp->WorkPos.x, mainVp->WorkPos.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(mainVp->Size.x, mainVp->Size.y), ImGuiCond_Always);


	if (!ImGui::Begin("Loading Menu", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::End();
		return;
	}

	//Draw menu
	ImGui::SetNextWindowPos(ImVec2(mainVp->GetCenter().x - 150, mainVp->GetCenter().y - 100), ImGuiCond_Always);
	//ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Always);
	ImGui::BeginChild("Loading...", ImVec2(300, 70), true, ImGuiWindowFlags_NoSavedSettings);
	ImGui::SetWindowFontScale(1.5);
	float contentWidth = ImGui::GetWindowContentRegionWidth();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1, 0.4, 0.3, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2, 0.2, 0.2, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4, 0.4, 0.4, 1));

	ImGui::End();
}
#include "MainMenu.h"
#include"../GameTech/GameTechRenderer.h"

using namespace NCL;
using namespace CSC8503;

void MainMenu::Draw()
{

	Debug::Print("YIANSER Game Menu", Vector2(35, 25));

	const ImGuiViewport* mainVp = ImGui::GetMainViewport();

	//Draw background
	ImGui::SetNextWindowPos(ImVec2(mainVp->WorkPos.x, mainVp->WorkPos.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(mainVp->Size.x, mainVp->Size.y), ImGuiCond_Always);


	if (!ImGui::Begin("Main Game Menu", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::End();
		return;
	}

	//Draw menu
	ImGui::SetNextWindowPos(ImVec2(mainVp->GetCenter().x - 150, mainVp->GetCenter().y - 100), ImGuiCond_Always);
	//ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_Always);
	ImGui::BeginChild("Enter Game", ImVec2(300, 70), true, ImGuiWindowFlags_NoSavedSettings);
	ImGui::SetWindowFontScale(1.5);
	float contentWidth = ImGui::GetWindowContentRegionWidth();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1, 0.4, 0.3, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2, 0.2, 0.2, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4, 0.4, 0.4, 1));
	EnterGame = ImGui::Button("Start Game", ImVec2(contentWidth, 50));
	ImGui::PopStyleColor(3);
	ImGui::EndChild();

/*
	

*/
	ImGui::SetNextWindowPos(ImVec2(mainVp->GetCenter().x - 150, mainVp->GetCenter().y + 40), ImGuiCond_Always);
	ImGui::BeginChild("Quit", ImVec2(300, 70), true, ImGuiWindowFlags_NoSavedSettings);
	ImGui::SetWindowFontScale(1.5);
	//float contentWidth = ImGui::GetWindowContentRegionWidth();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3, 0.05, 0.05, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6, 0.1, 0.1, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9, 0.1, 0.1, 1));
	QuitGame = ImGui::Button("Quit Game", ImVec2(contentWidth, 50));
	ImGui::PopStyleColor(3);
	ImGui::EndChild();

	if (PauseMode) {
		ImGui::SetNextWindowPos(ImVec2(mainVp->GetCenter().x - 150, mainVp->GetCenter().y - 30), ImGuiCond_Always);
		ImGui::BeginChild("Cancel", ImVec2(300, 70), true, ImGuiWindowFlags_NoSavedSettings);
		ImGui::SetWindowFontScale(1.5);
		//float contentWidth = ImGui::GetWindowContentRegionWidth();
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3, 0.7, 0.6, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2, 0.2, 0.2, 1));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4, 0.4, 0.4, 1));
		Cancel = ImGui::Button("Cancel", ImVec2(contentWidth, 50));
		ImGui::PopStyleColor(3);
		ImGui::EndChild();
	}

	

	ImGui::End();
}
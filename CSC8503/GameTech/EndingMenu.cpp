#include "EndingMenu.h"
#include "TutorialGame.h"
#include"../GameTech/GameTechRenderer.h"

using namespace NCL;
using namespace CSC8503;

void EndingMenu::Draw()
{
	
	const ImGuiViewport* mainVp = ImGui::GetMainViewport();

	//Draw background
	ImGui::SetNextWindowPos(ImVec2(mainVp->WorkPos.x, mainVp->WorkPos.y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(mainVp->Size.x, mainVp->Size.y), ImGuiCond_Always);

	float contentWidth = ImGui::GetWindowContentRegionWidth();

	if (!ImGui::Begin("End Menu", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings))
	{
		ImGui::End();
		return;
	}

	ImGui::PushItemWidth(ImGui::GetFontSize() * 30);
	ImGui::SetCursorPos(ImVec2(mainVp->GetCenter().x, mainVp->GetCenter().y -200));
	ImGui::Text("Game Over");

	ImGui::PushItemWidth(ImGui::GetFontSize() * 20);
	ImGui::SetCursorPos(ImVec2(mainVp->GetCenter().x, mainVp->GetCenter().y));
	
	if (game->team1Kill > game->team2Kill)
	{
		ImGui::Text("team1 win!!!");
	}
	else if (game->team2Kill > game->team1Kill)
	{
		ImGui::Text("team2 win!!!");
	}
	else if (game->team1Kill == game->team2Kill)
	{
		ImGui::Text("Tie!!!!");
	}


	ImGui::SetNextWindowPos(ImVec2(mainVp->GetCenter().x - 150, mainVp->GetCenter().y + 40), ImGuiCond_Always);
	ImGui::BeginChild("Quit", ImVec2(400, 70), true, ImGuiWindowFlags_NoSavedSettings);
	ImGui::SetWindowFontScale(2.0);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3, 0.05, 0.05, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6, 0.1, 0.1, 1));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9, 0.1, 0.1, 1));
	QuitGame = ImGui::Button("Quit Game", ImVec2(contentWidth, 50));
	ImGui::PopStyleColor(3);
	ImGui::EndChild();





	ImGui::End();
}

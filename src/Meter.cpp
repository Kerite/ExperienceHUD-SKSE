#include "Meter.h"

void Meter::Add(Skills a_eSkillID, const std::string a_sSkillName, float a_fAddedExp, float a_fCurrentExp, float a_fThreshold)
{
	auto& element = m_mSkills[a_eSkillID];
	element.m_fLastAddedExp = a_fAddedExp;
	element.m_fCurrentExp = a_fCurrentExp;
	element.m_fExperienceThreshold = a_fThreshold;
	element.m_ulDuration = 600;
	element.m_sSkillName = a_sSkillName;
}

void Meter::Render()
{
	if (!m_bToogled.load()) {
		return;
	}
	auto pUI = RE::UI::GetSingleton();
	if (!pUI || pUI->GameIsPaused() || !pUI->GetMenu<RE::HUDMenu>()) {
		return;
	}
	static constexpr ImGuiWindowFlags windowFlag = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs;

	float screenSizeX = ImGui::GetIO().DisplaySize.x, screenSizeY = ImGui::GetIO().DisplaySize.y;
	//ImGui::SetNextWindowSize(ImVec2(screenSizeX, screenSizeY));
	ImGui::SetNextWindowPos(ImVec2(screenSizeX / 2, screenSizeY), 0, ImVec2(0.5, 1));

	ImGui::Begin("ColaTea_ExpHUD", nullptr, windowFlag);

	for (auto it = m_mSkills.begin(); it != m_mSkills.end(); ++it) {
		auto& skill = it->second;
		if (it->second.m_ulDuration <= 0) {
			continue;
		}
		//ImGui::Text("%s %.2f/%.0f (+%.2f)", it->second.m_sSkillName.data(), it->second.m_fCurrentExp, it->second.m_fExperienceThreshold, it->second.m_fLastAddedExp);
		//ImGui::SameLine();
		ImGui::Text("%s:", skill.m_sSkillName.c_str());
		ImGui::SameLine();

		auto progressBarContent = std::format("{:.2f}/{:.2f}", skill.m_fCurrentExp, skill.m_fExperienceThreshold);
		ImGui::ProgressBar(skill.m_fCurrentExp / skill.m_fExperienceThreshold, ImVec2(0, 0), progressBarContent.c_str());
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
		ImGui::Text("%+.2f", skill.m_fLastAddedExp);
		ImGui::PopStyleColor();
		it->second.m_ulDuration--;
	}
	ImGui::End();
}
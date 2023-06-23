#include "Meter.h"
#include "Config.h"

void Meter::Add(Skills a_eSkillID, const std::string_view a_sSkillName, float a_fAddedExp, float a_fCurrentExp, float a_fThreshold)
{
	bool bInsert = false;
	if (!m_mSkills.contains(a_eSkillID)) {
		bInsert = true;
	}
	auto& element = m_mSkills[a_eSkillID];
	element.m_fLastAddedExp = a_fAddedExp;
	element.m_fCurrentExp = a_fCurrentExp;
	element.m_fExperienceThreshold = a_fThreshold;
	element.m_ulDuration = 600;
	if (bInsert) {
		element.m_sSkillName = std::string(a_sSkillName.data());
		m_fTextWidth = std::max(ImGui::CalcTextSize(a_sSkillName.data()).x + 10.f, m_fTextWidth);
	}
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
	auto pConfig = Config::GetSingleton();
	static constexpr ImGuiWindowFlags windowFlag = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs;
	static ImVec2 alignmentVec = ImVec2(*pConfig->m_iHAlign / 2.f, *pConfig->m_iVAlign / 2.f);

	float screenSizeX = ImGui::GetIO().DisplaySize.x, screenSizeY = ImGui::GetIO().DisplaySize.y;
	ImVec2 screenPos = ImVec2(screenSizeX * alignmentVec.x + *pConfig->m_iHOffset, screenSizeY * alignmentVec.y + *pConfig->m_iVOffset);
	//ImGui::SetNextWindowSize(ImVec2(screenSizeX, screenSizeY));
	ImGui::SetNextWindowPos(ImVec2(screenSizeX * alignmentVec.x, screenSizeY * alignmentVec.y), 0, alignmentVec);

	ImGui::Begin("ColaTea_ExpHUD", nullptr, windowFlag);

	for (auto it = m_mSkills.begin(); it != m_mSkills.end(); ++it) {
		auto& skill = it->second;
		if (it->second.m_ulDuration <= 0) {
			continue;
		}
		ImGui::Text(skill.m_sSkillName.c_str());

		ImGui::SameLine(m_fTextWidth);
		auto progressBarContent = std::format("{:.2f}/{:.2f}", skill.m_fCurrentExp, skill.m_fExperienceThreshold);
		ImGui::ProgressBar(skill.m_fCurrentExp / skill.m_fExperienceThreshold, ImVec2(180, 0), progressBarContent.c_str());

		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
		ImGui::Text("%+.2f", skill.m_fLastAddedExp);
		ImGui::PopStyleColor();
		it->second.m_ulDuration--;
	}
	ImGui::End();
}
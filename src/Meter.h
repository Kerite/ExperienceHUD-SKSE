#pragma once
#include "Utils.h"

struct SSKill
{
	float m_fLastAddedExp;
	float m_fCurrentExp;
	float m_fExperienceThreshold;
	ULONG m_ulDuration;
	std::string m_sSkillName;
};

using Skills = RE::PlayerCharacter::PlayerSkills::Data::Skills::Skill;

DEF_SINGLETON(Meter)
{
public:
	void Render();
	void Add(Skills a_eSkill, const std::string_view a_sSkillName, float a_fAddedExperience, float a_fCurrentExp, float a_fThreshold);
	void Toggle()
	{
		m_bToogled = !m_bToogled;
	}

private:
	float m_fTextWidth = 0;
	std::atomic_bool m_bToogled = true;
	std::map<Skills, SSKill> m_mSkills;
};
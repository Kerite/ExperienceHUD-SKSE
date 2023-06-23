#include "Config.h"

static auto MainConfig = COMPILE_PROXY("ExperienceHUD.toml"sv);

void Config::Load()
{
	MainConfig.Bind(m_bUseCustomFont, false);
	MainConfig.Bind(m_strFont, "C:\\Windows\\Fonts\\Segoeui.ttf");
	MainConfig.Bind<0, INT_MAX>(m_iFontSize, 20);
	MainConfig.Bind<0, 2>(m_iHAlign, 1);
	MainConfig.Bind<0, 2>(m_iVAlign, 2);
	MainConfig.Bind(m_iHOffset, 0);
	MainConfig.Bind(m_iVOffset, 0);

	MainConfig.Load();
}
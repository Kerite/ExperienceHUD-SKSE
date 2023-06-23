#pragma once
#include "DKUtil/Config.hpp"

#include "Utils.h"
using namespace DKUtil::Alias;

class Config final : public Singleton<Config>
{
public:
	void Load();
	Boolean m_bUseCustomFont{ "UseCustomFont" };
	String m_strFont{ "Font" };
	Integer m_iFontSize{ "FontSize" };
	Integer m_iHAlign{ "HorizontalAlignment" };
	Integer m_iVAlign{ "VerticalAlignment" };
	Integer m_iHOffset{ "HorizontalOffset" };
	Integer m_iVOffset{ "VerticalOffset" };
};
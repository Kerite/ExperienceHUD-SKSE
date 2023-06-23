#pragma once
#include "DKUtil/Config.hpp"

#include "Utils.h"
using namespace DKUtil::Alias;

DEF_SINGLETON(Config)
{
public:
	void Load();
	String m_strFont{ "Font" };
	Integer m_iFontSize{ "FontSize" };
	Integer m_iHAlign{ "HorizontalAlignment" };
	Integer m_iVAlign{ "VerticalAlignment" };
	Integer m_iHOffset{ "HorizontalOffset" };
	Integer m_iVOffset{ "VerticalOffset" };
};
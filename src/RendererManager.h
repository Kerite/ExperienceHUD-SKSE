#pragma once
#include "Utils.h"

DEF_SINGLETON(RendererManager)
{
public:
	void Install();

private:
	std::atomic<bool> m_bInstalled = false;

	HOOK_MAKE(75595, 77226, 0x9, 0x275, InitD3D, void)
	HOOK_MAKE(75461, 77246, 0x9, 0x9, Present, void, std::uint32_t)
};
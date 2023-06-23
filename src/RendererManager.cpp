#include "RendererManager.h"

#include <d3d11.h>

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "Config.h"
#include "Meter.h"

void HOOK_FUNC(RendererManager, InitD3D)
{
	oldFunc();
	auto pRenderer = RE::BSGraphics::Renderer::GetSingleton();
	auto pConfig = Config::GetSingleton();

	IDXGISwapChain* pDXGISwapChain = pRenderer->data.renderWindows[0].swapChain;
	DXGI_SWAP_CHAIN_DESC sd{};
	if (pDXGISwapChain->GetDesc(std::addressof(sd)) < 0) {
		ERROR("IDXGISwapChain::GetDesc failed.");
		return;
	}

	ID3D11Device* pD3DDevice = pRenderer->data.forwarder;
	ID3D11DeviceContext* pD3DDeviceContext = pRenderer->data.context;

	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	static ImVector<ImWchar> vRange;
	static ImFontGlyphRangesBuilder cGlyph;
	cGlyph.AddRanges(io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
	io.Fonts->AddFontDefault();
	ImFont* font = io.Fonts->AddFontFromFileTTF((*pConfig->m_strFont).c_str(), *pConfig->m_iFontSize, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
	IM_ASSERT(font != NULL);
	ImGui::GetIO().FontDefault = font;

	ImGui_ImplWin32_Init(sd.OutputWindow);
	ImGui_ImplDX11_Init(pD3DDevice, pD3DDeviceContext);

	RendererManager::GetSingleton()->m_bInstalled.store(true);
}

void HOOK_FUNC(RendererManager, Present, std::uint32_t a_p1)
{
	oldFunc(a_p1);
	if (!RendererManager::GetSingleton()->m_bInstalled.load()) {
		return;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Meter::GetSingleton()->Render();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void RendererManager::Install()
{
	Utils::Hook::WriteCall<Hook_InitD3D>();
	Utils::Hook::WriteCall<Hook_Present>();
}
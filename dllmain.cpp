#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 1024
#include <intrin.h>
#include <cstdint>
#include <dxgi.h>
#include <time.h>
#include <cstdio>
#include <iostream>
#include <filesystem>
#include <KnownFolders.h>

#include "memory/memory.hpp"
#include "memory/vmthook.h"
#include "memory/il2cpp.hpp"
#include "hooks.hpp"
#include "gui/OnGUI.hpp"

#include "esp.hpp"
#include "gui.h"
#include "fontarray.h"
#include "assets/assets.hpp"

#include "leo.h"
#include "imgui/imgui_internal.h"

#define consoleout

bool has_initialized = false, init = false, menuopen = false;

//extern DWORD D3DThread();

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

void InitImGui()
{
	im::CreateContext();
	ImGuiIO& io = im::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	if (GetAsyncKeyState(VK_INSERT) & 1)
		vars->open = !vars->open;

	//memu
	im::NewFrame();
	if (render.NewFrame(pSwapChain))
	{
		new_frame();
	}
	render.EndFrame();
	if (vars->open)
	{
		Gui::Render();
	}
	//__try {
	//} __except(true) {
	//	esp::local_player->console_echo(_(L"[trap]: ERROR. Crash inside: " __FUNCTION__));
	//}
	im::End();
	
	im::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(im::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)&oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);
	return TRUE;
}

void extractfiles() {
	//font
	std::ofstream font_out(_("mc.ttf"), std::ios::out | std::ios::binary);
	font_out.write(reinterpret_cast<const char*>(minecraft_ttf), 13000);
	font_out.close();
	SetFileAttributes(_(L"mc.ttf"), FILE_ATTRIBUTE_HIDDEN);
	AddFontResource(_(L"mc.ttf"));

	//lastcheats chams
	std::ofstream lc_out(_("RustClient_Data\\assets1.shared"), std::ios::out | std::ios::binary);
	lc_out.write(reinterpret_cast<const char*>(assets::lastcheats), 15530);
	lc_out.close();
	SetFileAttributes(_(L"RustClient_Data\\assets1.shared"), FILE_ATTRIBUTE_HIDDEN);

	//our custom chams
	std::ofstream c_out(_("RustClient_Data\\assets2.shared"), std::ios::out | std::ios::binary);
	c_out.write(reinterpret_cast<const char*>(assets::custom), 1771920);
	c_out.close();
	SetFileAttributes(_(L"RustClient_Data\\assets2.shared"), FILE_ATTRIBUTE_HIDDEN);
}

float placeholder = 1.f;
bool DllMain(HMODULE hmodule)
{
	if (!has_initialized) {
		extractfiles();
		DisableThreadLibraryCalls(hmodule);
		CloseHandle(CreateThread(0, 0, (PTHREAD_START_ROUTINE)MainThread, hmodule, 0, 0));
		//init cheat?
		auto s = LI_FIND(getenv)(_("APPDATA"));
		auto p = s + std::string(_("\\trap\\"));
		vars->data_dir = p;
		CreateDirectoryA(p.c_str(), 0);
		p = p + std::string(_("configs"));
		CreateDirectoryA(p.c_str(), 0);

		mem::game_assembly_base = LI_MODULE_SAFE_(_("GameAssembly.dll"));
		mem::unity_player_base = LI_MODULE_SAFE_(_("UnityPlayer.dll"));

		AllocConsole();

		il2cpp::init();

		unity::init_unity();
		gui::init_gui();
		hooks::init_hooks();
		init_bp();
		init_projectile();

		typedef System::list<uintptr_t>* (*AAA)();//real rust 36223520 ALKAD 36217232 "Name": ,"ConsoleSystem.Index$$get_All"
		System::list<uintptr_t>* command_list = ((AAA)(mem::game_assembly_base + oConsoleSystem_GetAll))();

		if (command_list) {
			auto sz = *reinterpret_cast<int*>(command_list + 0x18);
			for (size_t i = 0; i < sz; i++)
			{
				auto cmd = *reinterpret_cast<uintptr_t*>(command_list + 0x20 + i * 0x8);
				if (!cmd) continue;
				auto name = (System::string*)*reinterpret_cast<uintptr_t*>((uintptr_t)cmd + 0x10);
				if (!LI_FIND(wcscmp)(name->str, _(L"noclip")) ||
					!LI_FIND(wcscmp)(name->str, _(L"debugcamera")) ||
					!LI_FIND(wcscmp)(name->str, _(L"debug.debugcamera")) ||
					!LI_FIND(wcscmp)(name->str, _(L"camspeed")) ||
					!LI_FIND(wcscmp)(name->str, _(L"camlerp")))
				{
					bool r = false;
					mem::write<bool>((uintptr_t)cmd + 0x58, r);
				}
			}
		}
		has_initialized = true;
	}

	il2cpp::hook(&hooks::hk_performance_update, _("Update"), _("PerformanceUI"), _("Facepunch"), 0);
	il2cpp::hook(&gui::OnGUI, _("OnGUI"), _("DevControls"), _(""), 0);
	il2cpp::hook(&hooks::hk_projectile_update, _("Update"), _("Projectile"), _(""), 0);
	mem::hook_virtual_function(_("BasePlayer"), _("ClientInput"), &hooks::hk_baseplayer_ClientInput);
	mem::hook_virtual_function(_("BaseProjectile"), _("LaunchProjectile"), &hooks::hk_projectile_launchprojectile);

	return true;
}

extern "C" __declspec(dllexport) int ACEShowBalloonVariadic(int code, WPARAM wParam, LPARAM lParam) {
	return CallNextHookEx(NULL, code, wParam, lParam);
}
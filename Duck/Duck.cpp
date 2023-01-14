#include "Duck.h"
#include "Strings.h"
#include "detours.h"
#include "GameData.h"

#include <stdexcept>
#include <iostream>


D3DPresentFunc                     Duck::OriginalD3DPresent = NULL;
WNDPROC Duck::OriginalWindowMessageHandler = NULL;
std::mutex Duck::DxDeviceMutex;
std::condition_variable Duck::OverlayInitialized;

LPDIRECT3DDEVICE9                  Duck::DxDevice = NULL;


void Duck::Run()
{
	try {
		DxDeviceMutex.lock();

		Logger::File.Log("Starting up Duck...\n");
		HookDirectX();
		
		Logger::File.Log("Loading Game Data..");
		GameData::LoadAsync();
	}
	catch (std::exception& error) {
		Logger::File.Log("Failed starting up Duck %s\n", error.what());
	}
}




void Duck::ShowLoader()
{
	if (!GameData::LoadProgress->complete)
	{
		ImGui::Begin("Duck Loader", NULL, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Text(GameData::LoadProgress->currentlyLoading);
		ImGui::ProgressBar(GameData::LoadProgress->percentDone);

		if (GameData::LoadProgress->complete)
			Logger::LogAll("Loaded Game Database");

		ImGui::End();
	}
}

void Duck::ShowMenu()
{
	static bool ShowConsoleWindow = false;

	ImGui::Begin("Duck", nullptr,
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::BeginMenu("1 Development")) {
		ImGui::Checkbox("Show Console", &ShowConsoleWindow);
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Menu Settings")) {
		ImGui::ShowStyleSelector("Style");
		ImGui::EndMenu();
	}

	ImGui::End();

	if (ShowConsoleWindow)
		ShowConsole();
}

void Duck::ShowConsole()
{
	ImGui::Begin("Console");

	std::list<std::string> lines;
	Logger::Console.GetLines(lines);

	
	for(auto& line : lines) {
		ImGui::Text(line.c_str());
	}

	ImGui::End();
}

void Duck::WaitForOverlayToInit()
{
	std::mutex mtx;
	std::unique_lock<std::mutex> lock(mtx);
	Duck::OverlayInitialized.wait(lock);
}

void Duck::InitializeOverlay()
{
	Logger::File.Log("Initializing overlay");

	HWND hWindow = FindWindowA("RiotWindowClass", NULL);
	OriginalWindowMessageHandler = WNDPROC(SetWindowLongA(hWindow, GWL_WNDPROC, LONG_PTR(HookedWindowMessageHandler)));

	ImGui::CreateContext();

	if (!ImGui_ImplWin32_Init(hWindow))
		throw std::runtime_error("Failed to initialize ImGui_ImplWin32_Init");

	if (!ImGui_ImplDX9_Init(DxDevice))
		throw std::runtime_error("Failed to initialize ImGui_ImplDX9_Init");

	Logger::Console.Log("Initialized Duck Overlay!");
	OverlayInitialized.notify_all();
}

void Duck::Update()
{
	// Create frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	ShowLoader();
	ShowMenu();


	// Render
	ImGui::EndFrame();
	ImGui::Render();

	DxDeviceMutex.lock();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	DxDeviceMutex.unlock();
}


void Duck::HookDirectX()
{
	static const int SearchLength = 0x500000;
	static const int PresentVTableIndex = 17;
	static const int EndSceneVTableIndex = 42;

	Logger::File.Log("Hooking DirectX");

	DWORD objBase = (DWORD)LoadLibraryA("d3d9.dll");
	DWORD stopAt = objBase + SearchLength;

	Logger::File.Log("Found base of d3d9.dll at: %#010x", objBase);
	while (objBase++ < stopAt)
	{
		if ((*(WORD*)(objBase + 0x00)) == 0x06C7
			&& (*(WORD*)(objBase + 0x06)) == 0x8689
			&& (*(WORD*)(objBase + 0x0C)) == 0x8689
			) {
			objBase += 2;
			break;
		}
	}

	if (objBase >= stopAt)
		throw std::runtime_error("Did not find D3D device");

	Logger::File.Log("Found D3D Device at: %#010x", objBase);

	PDWORD VTable;
	*(DWORD*)&VTable = *(DWORD*)objBase;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	OriginalD3DPresent = (D3DPresentFunc)(VTable[PresentVTableIndex]);
	LONG error = DetourAttach(&(PVOID&)OriginalD3DPresent, (PVOID)HookedD3DPresent);
	if (error)
		throw std::runtime_error(Strings::Format("Failed to hook DirectX present. Detours error code: %d"));

	DetourTransactionCommit();

}

void Duck::UnhookDirectX()
{
	Logger::File.Log("Unhooking DirectX");

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)OriginalD3DPresent, (PVOID)HookedD3DPresent);
	DetourTransactionCommit();
}

HRESULT __stdcall Duck::HookedD3DPresent(LPDIRECT3DDEVICE9 Device, const RECT* pSrcRect, const RECT* pDestRect, HWND hDestWindow, const RGNDATA* pDirtyRegion)
{
	DxDeviceMutex.unlock();
	try {
		if (DxDevice == NULL) {
			DxDevice = Device;
			InitializeOverlay();
		}
		Update();
	}
	catch (std::exception& error) {
		Logger::File.Log("Error occured %s", error.what());
		UnhookDirectX();
	}
	catch (...) {
		Logger::File.Log("Unexpected error occured.");
		UnhookDirectX();
	}
	DxDeviceMutex.lock();

	return OriginalD3DPresent(Device, pSrcRect, pDestRect, hDestWindow, pDirtyRegion);
}

LRESULT ImGuiWindowMessageHandler(HWND, UINT msg, WPARAM wParam, LPARAM lParam)
{
	auto& io = ImGui::GetIO();

	switch (msg)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		io.MouseDown[0] = true;
		return true;
	case WM_LBUTTONUP:
		io.MouseDown[0] = false;
		return true;
	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
		io.MouseDown[1] = true;
		return true;
	case WM_RBUTTONUP:
		io.MouseDown[1] = false;
		return true;
	case WM_MBUTTONDOWN:
	case WM_MBUTTONDBLCLK:
		io.MouseDown[2] = true;
		return true;
	case WM_MBUTTONUP:
		io.MouseDown[2] = false;
		return true;
	case WM_XBUTTONDOWN:
	case WM_XBUTTONDBLCLK:
		if ((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON1) == MK_XBUTTON1)
			io.MouseDown[3] = true;
		else if ((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON2) == MK_XBUTTON2)
			io.MouseDown[4] = true;
		return true;
	case WM_XBUTTONUP:
		if ((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON1) == MK_XBUTTON1)
			io.MouseDown[3] = false;
		else if ((GET_KEYSTATE_WPARAM(wParam) & MK_XBUTTON2) == MK_XBUTTON2)
			io.MouseDown[4] = false;
		return true;
	case WM_MOUSEWHEEL:
		io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
		return true;
	case WM_MOUSEMOVE:
		io.MousePos.x = (signed short)(lParam);
		io.MousePos.y = (signed short)(lParam >> 16);
		return true;
	case WM_KEYDOWN:
		if (wParam < 256)
			io.KeysDown[wParam] = 1;
		return true;
	case WM_KEYUP:
		if (wParam < 256)
			io.KeysDown[wParam] = 0;
		return true;
	case WM_CHAR:
		if (wParam > 0 && wParam < 0x10000)
			io.AddInputCharacter((unsigned short)wParam);
		return true;
	}

	return 0;
}

LRESULT WINAPI Duck::HookedWindowMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGuiWindowMessageHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return CallWindowProcA(OriginalWindowMessageHandler,	hWnd, msg, wParam, lParam);
}
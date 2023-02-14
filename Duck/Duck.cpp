#include "Duck.h"
#include "Strings.h"
#include "detours.h"
#include "GameData.h"
#include "ObjectExplorer.h"
#include "d3dx9shader.h"
#include "Offset.h"
#include "Memory.h"
#include "Globals.h"
#include "PyStructs.h"
#include "OffsetScanner.h"

#include <stdexcept>
#include <iostream>
#include <functional>
#include <dxva2api.h>
#include <TlHelp32.h>

#include <boost/exception/diagnostic_information.hpp>


D3DPresentFunc Duck::OriginalD3DPresent = NULL;
WNDPROC Duck::OriginalWindowMessageHandler = NULL;
LPDIRECT3DDEVICE9 Duck::DxDevice = NULL;
GameState* Duck::CurrentGameState = NULL;
std::mutex Duck::DxDeviceMutex;
std::condition_variable Duck::OverlayInitialized;
GameReader Duck::Reader;

PyExecutionContext Duck::ScriptContext;
ScriptManager Duck::ScriptManager;
ConfigSet Duck::Configs("duck");




void Duck::Run()
{
	try {

		DxDeviceMutex.lock();

		Configs.Load();
		
		
		GameData::LoadAsync();

	
		HookDirectX();
	}
	catch (std::exception& error) {
		Logger::File.Log("Failed starting up Duck %s\n", error.what());
	}
}




bool Duck::CheckEssentialsLoaded()
{
	if (!GameData::LoadProgress->allLoaded)
		GameData::ImGuiDrawLoader();

	if (GameData::LoadProgress->essentialsLoaded)
		return true;

	return false;
}

void Duck::ShowMenu()
{
	//static bool ShowConsoleWindow = true;
	//static bool ShowObjectExplorerWindow = true;
	//static bool ShowOffsetScanner = false;
	static bool ShowConsoleWindow = Configs.GetBool("show_console", false);
	static bool ShowObjectExplorerWindow = Configs.GetBool("show_obj_explorer", false);
	static bool ShowOffsetScanner = Configs.GetBool("show_offset_scanner", false);


	if (ImGui::Begin("Duck", nullptr,
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_AlwaysAutoResize)) {

		if (ImGui::BeginMenu("Development")) {

			if (ImGui::Button("Reload Scripts"))
				LoadScripts();

			ImGui::LabelText("VPath", Globals::WorkingDir.u8string().c_str());
			ImGui::LabelText("Patch Version", Offset::GameVersion.c_str());
			ImGui::Checkbox("Show Console", &ShowConsoleWindow);
			ImGui::Checkbox("Show Object Explorer", &ShowObjectExplorerWindow);
			ImGui::Checkbox("Show Offset Scanner", &ShowOffsetScanner);


			if (ImGui::TreeNode("Benchmarks")) {

				Reader.GetBenchmarks().ImGuiDraw();
				ImGui::TreePop();
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Menu Settings")) {
			ImGui::ShowStyleSelector("Style");
			ImGui::EndMenu();
		}

		ImGui::Separator();
		ScriptManager.ImGuiDrawMenu(ScriptContext);

		ImGui::End();

		if (Configs.IsTimeToSave()) {
			Configs.SetBool("show_console", ShowConsoleWindow);
			Configs.SetBool("show_obj_explorer", ShowConsoleWindow);
			Configs.SetBool("show_offset_scanner", ShowOffsetScanner);
			Configs.Save();
		}
	
	}	


	if (ShowConsoleWindow) 
		ShowConsole();

	if (ShowObjectExplorerWindow) 
		ObjectExplorer::ImGuiDraw(*CurrentGameState);

	if (ShowOffsetScanner)
		OffsetScanner::ImGuiDraw();
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
	Logger::LogAll("Initializing overlay");

	HWND hWindow = FindWindowA("RiotWindowClass", NULL);
	OriginalWindowMessageHandler = WNDPROC(SetWindowLongA(hWindow, GWL_WNDPROC, LONG_PTR(HookedWindowMessageHandler)));

	ImGui::CreateContext();

	if (!ImGui_ImplWin32_Init(hWindow))
		throw std::runtime_error("Failed to initialize ImGui_ImplWin32_Init");

	if (!ImGui_ImplDX9_Init(DxDevice))
		throw std::runtime_error("Failed to initialize ImGui_ImplDX9_Init");

	Logger::LogAll("Initialized Duck Overlay!");
	OverlayInitialized.notify_all();
	//GetWindowRect(LeagueWindowHandle, &WindowRect);
}

void Duck::InitializePython()
{
	Logger::LogAll("Initializing Python");
	PyImport_AppendInittab("duck", &PyInit_Duck);
	Py_Initialize();
	exec("from duck import *");
}

void Duck::ExecuteScripts()
{
	ScriptManager.ExecuteScripts(ScriptContext);
}

void Duck::SetupScriptExecutionContext()
{
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("##Overlay", nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoInputs |
		ImGuiWindowFlags_NoBackground
	);


	ScriptContext.SetGameState(CurrentGameState);
	ScriptContext.SetImGuiOverlay(ImGui::GetWindowDrawList());
	ImGui::End();
}

void Duck::LoadScripts()
{
	fs::path pathScripts = Globals::WorkingDir;
	pathScripts.append("scripts");
	std::string pathStr = pathScripts.u8string();

	ScriptManager.LoadScriptsFromFolder(pathStr);
}

void Duck::Update()
{
	// Create frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	

	if (CheckEssentialsLoaded())
	{
		//ImGui::ShowDemoWindow();
		CurrentGameState = Reader.GetNextState();
		if (CurrentGameState->gameStarted) {
			SetupScriptExecutionContext();
			ShowMenu();
			ExecuteScripts();
		}
	}




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

	Logger::LogAll("Hooking DirectX");

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
	/*


	static const int SearchLength = 0x500000;
	static const int SetVertexShaderVTableIndex = 92;
	static const int PresentVTableIndex = 17;
	static const int EndSceneVTableIndex = 42;
	static const int SetTransformVTableIndex = 44;

	Logger::File.Log("Hooking DirectX");

	HWND window = FindWindowA("RiotWindowClass", NULL);
	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (!pD3D)
		throw std::runtime_error("Failed to get direct3d");

	D3DPRESENT_PARAMETERS d3dpp{ 0 };
	d3dpp.hDeviceWindow = window, d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD, d3dpp.Windowed = TRUE;

	IDirect3DDevice9* device = nullptr;
	if (FAILED(pD3D->CreateDevice(0, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &device)))
	{
		pD3D->Release();
		throw std::runtime_error("Failed to create dx device");
	}

	void** VTable = *reinterpret_cast<void***>(device);

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	OriginalD3DPresent = (D3DPresentFunc)(VTable[PresentVTableIndex]);
	LONG error = DetourAttach(&(PVOID&)OriginalD3DPresent, (PVOID)HookedD3DPresent);
	if (error)
		throw std::runtime_error(Strings::Format("DetourAttach: Failed to hook DirectX Present. Detours error code: %d", error));

	error = DetourTransactionCommit();
	if (error)
		throw std::runtime_error(Strings::Format("DetourCommitTransaction: Failed to hook DirectX Present. Detours error code: %d", error));

	device->Release();
	Logger::File.Log("Successfully hooked DirectX");
	*/
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
			InitializePython();
			InitializeOverlay();
			LoadScripts();
		}
		Update();
	}
	catch (std::exception& error) {
		Logger::File.Log("Standard exception occured %s", error.what());
		UnhookDirectX();
	}
	catch (error_already_set) {
		Logger::File.Log("Boost::Python exception occured %s", Script::GetPyError().c_str());
		UnhookDirectX();
	}
	catch (...) {
		Logger::File.Log("Unexpected exception occured");
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
	/*
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
	case WM_MOUSEWHEEL:
		io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
		return true;
	case WM_MOUSEMOVE:
		io.MousePos.x = (signed short)(lParam);
		io.MousePos.y = (signed short)(lParam >> 16);
		return true;
	case WM_KEYDOWN:
		if (wParam != 9)
			io.KeysDown[wParam] = 1;
		return true;
	case WM_KEYUP:
		if (wParam != 9)
			io.KeysDown[wParam] = 0;
		return true;
	case WM_CHAR:
		if (wParam > 0 && wParam < 0x10000)
			io.AddInputCharacter((unsigned short)wParam);
		return true;
	}

	return true;
	*/
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
	return CallWindowProcA(OriginalWindowMessageHandler, hWnd, msg, wParam, lParam);
	
	/*
	ImGuiWindowMessageHandler(hWnd, msg, wParam, lParam);

	switch (msg)
	{
	case WM_MOVE:
		GetWindowRect(Duck::LeagueWindowHandle, &Duck::WindowRect);
		return 0;
	case WM_SIZE:
		return 0;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	return CallWindowProcA(OriginalWindowMessageHandler, hWnd, msg, wParam, lParam);
	*/
}
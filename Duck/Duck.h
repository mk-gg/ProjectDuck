#pragma once
#include <d3d9.h>
#include <Windows.h>
#include <dinput.h>
#include <mutex>
#include <sstream>
#include <condition_variable>

#include "Logger.h"
#include "GameReader.h"
#include "ScriptManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
typedef HRESULT(__stdcall* D3DPresentFunc)(LPDIRECT3DDEVICE9, CONST RECT*, CONST RECT*, HWND, CONST RGNDATA*);

class Duck
{

private:

	static LRESULT WINAPI HookedWindowMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static HRESULT __stdcall HookedD3DPresent(LPDIRECT3DDEVICE9 Device, CONST RECT* pSrcRect, CONST RECT* pDestRect, HWND hDestWindow, CONST RGNDATA* pDirtyRegion);


	static D3DPresentFunc OriginalD3DPresent;
	static WNDPROC OriginalWindowMessageHandler;
	static std::condition_variable OverlayInitialized;
	static GameReader Reader;
	static ScriptManager ScriptManager;
	static PyExecutionContext ScriptContext;

	static bool CheckEssentialsLoaded();
	static void ShowMenu();
	static void ShowConsole();
	static void Update();
	static void InitializeOverlay();
	static void HookDirectX();
	static void UnhookDirectX();

	static void InitializePython();
	static void LoadScripts();

	static void SetupScriptExecutionContext();

public:
	static std::mutex DxDeviceMutex;
	static LPDIRECT3DDEVICE9 DxDevice;
	static GameState* CurrentGameState;
	static HWND LeagueWindowHandle;
	static RECT WindowRect;

	static void   Run();
	static void WaitForOverlayToInit();
	
	
};

// Intrusion.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <vector>
#include <thread>
#include <chrono>
#include <tlhelp32.h>
#include <psapi.h>

std::vector<uint32_t> find_processes(std::wstring name)
{
	std::vector<uint32_t> list;

	HANDLE process_snap;
	PROCESSENTRY32 pe32;
	process_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (process_snap == INVALID_HANDLE_VALUE)
		return list;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(process_snap, &pe32))
	{
		if (pe32.szExeFile == name)
			list.push_back(pe32.th32ProcessID);

		while (Process32Next(process_snap, &pe32))
		{
			if (pe32.szExeFile == name)
				list.push_back(pe32.th32ProcessID);
		}
	}

	CloseHandle(process_snap);

	return list;
}

using namespace std::chrono_literals;

bool is_injected(uint32_t pid)
{
	HMODULE hMods[1024];
	HANDLE hProcess;
	DWORD cbNeeded;
	unsigned int i;
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, pid);

	if (NULL == hProcess)
		return false;

	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{
			TCHAR szModName[MAX_PATH];
			if (GetModuleBaseName(hProcess, hMods[i], szModName,
				sizeof(szModName) / sizeof(TCHAR)))
			{

				if (wcscmp(szModName, L"Duck.dll") == 0)
				{
					CloseHandle(hProcess);
					return true;
				}
			}
		}
	}

	CloseHandle(hProcess);

	return false;
}

void enable_debug_privilege()
{
	HANDLE token;
	LUID value;
	TOKEN_PRIVILEGES tp;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
	{
		if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &value))
		{
			tp.PrivilegeCount = 1;
			tp.Privileges[0].Luid = value;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			if (AdjustTokenPrivileges(token, FALSE, &tp, sizeof(tp), NULL, NULL))
				CloseHandle(token);
		}
	}
}

bool inject(uint32_t pid)
{
	TCHAR current_dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, current_dir);

	auto dll_path = std::wstring(current_dir) + L"\\Duck.dll";

	auto handle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);

	if (!handle || handle == INVALID_HANDLE_VALUE)
	{
		printf("[-] Failed to open league process!\n");
		return false;
	}

	FILETIME ft;
	SYSTEMTIME st;
	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft);

	FILETIME create, exit, kernel, user;
	GetProcessTimes(handle, &create, &exit, &kernel, &user);

	auto delta = 10 - static_cast<int32_t>((*reinterpret_cast<uint64_t*>(&ft) - *reinterpret_cast<uint64_t*>(&create.dwLowDateTime)) / 10000000U);
	if (delta > 0)
		std::this_thread::sleep_for(std::chrono::seconds(delta));

	auto dll_path_remote = VirtualAllocEx(handle, nullptr, (dll_path.size() + 1) * sizeof(wchar_t), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	if (!dll_path_remote)
	{
		printf("[-] Failed to alloc space!\n");
		CloseHandle(handle);
		return false;
	}

	if (!WriteProcessMemory(handle, dll_path_remote, dll_path.data(), (dll_path.size() + 1) * sizeof(wchar_t), nullptr))
	{
		printf("[-] Failed to write memory!\n");
		VirtualFreeEx(handle, dll_path_remote, 0, MEM_RELEASE);
		CloseHandle(handle);
		return false;
	}

	auto thread = CreateRemoteThread(handle, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(GetProcAddress(LoadLibrary(L"kernel32.dll"), "LoadLibraryW")), dll_path_remote, 0, nullptr);
	if (!thread || thread == INVALID_HANDLE_VALUE)
	{
		printf("[-] Failed to create thread!\n");
		VirtualFreeEx(handle, dll_path_remote, 0, MEM_RELEASE);
		CloseHandle(handle);
		return false;
	}

	WaitForSingleObject(thread, INFINITE);
	CloseHandle(thread);
	VirtualFreeEx(handle, dll_path_remote, 0, MEM_RELEASE);
	CloseHandle(handle);
	printf("[+] Injected successfully!\n");
	return true;
}

int main()
{
	enable_debug_privilege();

	printf("[+] Looking for league of legends processes...\n");
	while (true)
	{
		auto league_processes = find_processes(L"League of Legends.exe");
		for (auto& pid : league_processes)
		{
			std::this_thread::sleep_for(6s);

			if (!is_injected(pid)) {
				std::this_thread::sleep_for(1s);
				inject(pid);
			}
		}
		std::this_thread::sleep_for(3s);
	}
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

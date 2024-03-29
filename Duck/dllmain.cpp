// dllmain.cpp : Defines the entry point for the DLL application.



#include "Duck.h"
#include "Strings.h"
#include <Windows.h>
#include "Globals.h"




DWORD WINAPI OverlayThreadEntryPoint(LPVOID lpParam) {
    fs::path pathFileLogger = Globals::WorkingDir;
    pathFileLogger.append("logs.txt");
    Logger::InitLoggers(pathFileLogger.u8string().c_str());
    Logger::File("Starting up Duck");
    Duck duck;

    duck.Run();

    return 0;
}



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
		CreateThread(NULL, 0, &OverlayThreadEntryPoint, NULL, 0, NULL);

	return TRUE;
}


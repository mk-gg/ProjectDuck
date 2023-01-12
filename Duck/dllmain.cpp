// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <boost/python.hpp>
#include <iostream>
#include "Duck.h"
#include "Strings.h"
#include <Windows.h>



void greet()
{
   std::cout << "REMY zeeeee rat";
}


BOOST_PYTHON_MODULE(Duck)
{
    using namespace boost::python;
    def("greet", greet);

}

DWORD WINAPI OverlayThreadEntryPoint(LPVOID lpParam) {

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


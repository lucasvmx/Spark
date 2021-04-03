#include <stdio.h>
#include <windows.h>

typedef void (*pSetPowerModifier)(int player, int modifier);

BOOL __stdcall changePowerModifier()
{
	HANDLE hModule;
	pSetPowerModifier setPowerModifier = NULL;
	
	hModule = GetModuleHandle(NULL);
	setPowerModifier = (pSetPowerModifier)((DWORD)(hModule) + 0x11c350);
	if(!setPowerModifier)
		return FALSE;
	
	while(TRUE)
	{
		setPowerModifier(0, 2000);
		Sleep(5000);
	}
	
	return TRUE;
}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved )  // reserved
{
	DWORD id;
	HANDLE hThread;

    // Perform actions based on the reason for calling.
    switch( fdwReason ) 
    {
        case DLL_PROCESS_ATTACH:

			hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)changePowerModifier, NULL, 0, &id);
			if(hThread == INVALID_HANDLE_VALUE)
				return FALSE;

         // Initialize once for each new process.
         // Return FALSE to fail DLL load.
            break;

        case DLL_THREAD_ATTACH:
         // Do thread-specific initialization.
            break;

        case DLL_THREAD_DETACH:
         // Do thread-specific cleanup.
            break;

        case DLL_PROCESS_DETACH:
         // Perform any necessary cleanup.
            break;
    }
	
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
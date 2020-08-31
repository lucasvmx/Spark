/**
 * @file dynamic_loader.c
 * @author Lucas Vieira (lucas.engen.cc@gmail.com)
 * @brief Funções carregadas dinamicamente em tempo de execução
 * @version 0.1
 * @date 2020-08-22
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <stdio.h>
#include "dynamic_loader.h"
#include "process.h"

#define KERNEL_DLL      "kernel32.dll"
#define PSAPI_DLL       "psapi.dll"
#define VERSION_DLL     "version.dll"

GFVI fGetFileVersionInfoA = NULL;
GFVIS fGetFileVersionInfoSizeA = NULL;
VQV fVerQueryValueA = NULL;
GMI fGetModuleInformation = NULL;
QFPINA fQueryFullProcessImageFileName = NULL;
pAddPower addPower = NULL;
pSetPowerModifier setPowerModifier = NULL;
pPrintChatMsg printchatmsg = NULL;
pIsHumanPlayer IsHumanPlayer = NULL;
pClear ClearPlayerPower = NULL;

void LoadDLLFunctions()
{
    HMODULE hVersion_dll = NULL;
    HMODULE hKernel_dll = NULL;
    HMODULE hPsapi_dll = NULL;

    hVersion_dll = LoadLibraryA(VERSION_DLL);
    hKernel_dll = LoadLibraryA(KERNEL_DLL);
    hPsapi_dll = LoadLibraryA(PSAPI_DLL);
    if (hVersion_dll == NULL)
    {
        printf("Failed to load version.dll. Error %lu\n", GetLastError());
        return;
    }

    fGetFileVersionInfoA = (GFVI)(GetProcAddress(hVersion_dll, "GetFileVersionInfoA"));
    if (fGetFileVersionInfoA == NULL)
    {
        if (hVersion_dll)
            FreeLibrary(hVersion_dll);

        printf("Failed to get function address on %s. Error %lu\n", VERSION_DLL, GetLastError());
        return;
    }

    fGetFileVersionInfoSizeA = (GFVIS)(GetProcAddress(hVersion_dll, "GetFileVersionInfoSizeA"));
    if (fGetFileVersionInfoSizeA == NULL)
    {
        if (hVersion_dll)
            FreeLibrary(hVersion_dll);

        printf("Failed to get function address on %s. Error %lu\n", VERSION_DLL, GetLastError());
        return;
    }

    fVerQueryValueA = (VQV)(GetProcAddress(hVersion_dll, "VerQueryValueA"));
    if (fVerQueryValueA == NULL)
    {
        if (hVersion_dll)
            FreeLibrary(hVersion_dll);

        printf("Failed to get function address on %s. Error %lu\n", VERSION_DLL, GetLastError());
        return;
    }

    fGetModuleInformation = (GMI)(GetProcAddress(hPsapi_dll, "GetModuleInformation"));
    if(fGetModuleInformation == NULL)
    {
        printf("Failed to get function address on %s. Error %lu\n", PSAPI_DLL, GetLastError());
        return;
    }

    fQueryFullProcessImageFileName = (QFPINA)(GetProcAddress(hKernel_dll, "QueryFullProcessImageNameA"));
    if(fQueryFullProcessImageFileName == NULL)
    {
        printf("Failed to get function address on %s. Error %lu\n", KERNEL_DLL, GetLastError());
        return;
    }
}

void LoadWarzoneFunctions(struct libhack_handle *handle)
{
    if(handle == NULL) {
        fprintf(stdout, "(%s:%d) null pointer to libhack\n", __FILE__, __LINE__);
        ExitProcess(1);
    }

    if(!((DWORD)(handle->hModule))) {
        fprintf(stdout, "(%s:%d) invalid hModule value\n", __FILE__, __LINE__);
        ExitProcess(1);
    }

    addPower = (pAddPower)((DWORD)(handle->hModule) + 0x11bb30);
    IsHumanPlayer = (pIsHumanPlayer)((DWORD)(handle->hModule) + 0x00f9540);
    printchatmsg = (pPrintChatMsg)((DWORD)(handle->hModule) + 0x00f9c20);
    setPowerModifier = (pSetPowerModifier)((DWORD)(handle->hModule) + 0x11c350);
    ClearPlayerPower = (pClear)((DWORD)(handle->hModule) + 0x11bd30);

    fprintf(stdout, "addPower: %p\n", addPower);
    fprintf(stdout, "isHumanPlayer: %p\n", IsHumanPlayer);
    fprintf(stdout, "printchatmsg: %p\n", printchatmsg);
    fprintf(stdout, "setPowerModifier: %p\n", setPowerModifier);
}

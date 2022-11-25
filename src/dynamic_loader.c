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

#define KERNEL_DLL      "kernel32.dll"
#define PSAPI_DLL       "psapi.dll"
#define VERSION_DLL     "version.dll"

GFVI fGetFileVersionInfoA = NULL;
GFVIS fGetFileVersionInfoSizeA = NULL;
VQV fVerQueryValueA = NULL;
GMI fGetModuleInformation = NULL;
QFPINA fQueryFullProcessImageFileName = NULL;

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

/*
	Hack para o warzone 2100

	Versões suportadas:
		* 2.3.9
		* 3.1.5
		* 3.2.3
	
	Características:
		* Energia infinita
		* Easter egg
	
	Descrição:
		Este programa permite que o jogador possua uma energia sempre acima do valor
		que ele quiser. Com esta ferramenta você pode, por exemplo, fazer com que
		nunca a sua energia esteja abaixo de 60000 por mais de 1 minuto.
	
	Aviso:
		Se for compilar no Visual Studio, ative a opção 'MultiByte'
		
	Autor: 
		Lucas Vieira de Jesus <lucas.engen.cc@gmail.com>
		
	Testado no:
		Microsoft Windows [versão 10.0.16299.125] x64
*/

#ifdef UNICODE
#undef UNICODE
#endif

/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <tlhelp32.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <assert.h>
#include "wzhack.h"

static GFVI fGetFileVersionInfoA = nullptr;
static GFVIS fGetFileVersionInfoSizeA = nullptr;
static VQV fVerQueryValueA = nullptr;
static GMI fGetModuleInformation = nullptr;

/* Início do código */

void LoadDLLFunctions()
{
    HMODULE hVersion_DLL = nullptr;

    hVersion_DLL = LoadLibraryA("version.dll");
    if (hVersion_DLL == nullptr)
    {
        WzHack_ShowMessage(CRITICAL, "Failed to load version.dll. Error %lu\n", GetLastError());
        return;
    }

    fGetFileVersionInfoA = reinterpret_cast<GFVI>(GetProcAddress(hVersion_DLL, "GetFileVersionInfoA"));
    if (fGetFileVersionInfoA == nullptr)
    {
        if (hVersion_DLL)
            FreeLibrary(hVersion_DLL);

        WzHack_ShowMessage(CRITICAL, "Failed to get function address on version.dll. Error %lu\n", GetLastError());
        return;
    }

    fGetFileVersionInfoSizeA = reinterpret_cast<GFVIS>(GetProcAddress(hVersion_DLL, "GetFileVersionInfoSizeA"));
    if (fGetFileVersionInfoSizeA == nullptr)
    {
        if (hVersion_DLL)
            FreeLibrary(hVersion_DLL);

        WzHack_ShowMessage(CRITICAL, "Failed to get function address on version.dll. Error %lu\n", GetLastError());
        return;
    }

    fVerQueryValueA = reinterpret_cast<VQV>(GetProcAddress(hVersion_DLL, "VerQueryValueA"));
    if (fVerQueryValueA == nullptr)
    {
        if (hVersion_DLL)
            FreeLibrary(hVersion_DLL);

        WzHack_ShowMessage(CRITICAL, "Failed to get function address on version.dll. Error %lu\n", GetLastError());
        return;
    }

    fGetModuleInformation = reinterpret_cast<GMI>(GetProcAddress(LoadLibraryA("psapi.dll"), "GetModuleInformation"));
    if(fGetModuleInformation == nullptr)
    {
        WzHack_ShowMessage(CRITICAL, "Failed to get function address on psapi.dll. Error %lu\n", GetLastError());
        return;
    }
}

/// <summary>Get the version of warzone 2100</summary>
/// <param name="wz_filename">Path of warzone2100.exe file</param>
/// <returns>WZ_330, WZ_323, WZ_315, WZ_239 or WZ_UNKNOWN</returns>
int WzHack_GetWarzoneVersion(const char *wz_filename)
{
    char *buf = nullptr;
	DWORD dwHandle, sz;

	int major, minor, patch, build;
	int iversion;
	
    sz = fGetFileVersionInfoSizeA(wz_filename, &dwHandle);
    if (sz == 0)
    {
        return 5;
    }

    buf = static_cast<char*>(malloc(static_cast<size_t>(sizeof(char) * sz)));
	if (!buf)
	{		
		return 6;
	}

	if (!fGetFileVersionInfoA(wz_filename, dwHandle, sz, &buf[0]))
	{
		if (buf)
			free(buf);
		
		return 7;
	}

	VS_FIXEDFILEINFO *pvi;
	sz = sizeof(VS_FIXEDFILEINFO);

    if (fVerQueryValueA(&buf[0], TEXT("\\"), reinterpret_cast<LPVOID*>(&pvi), reinterpret_cast<unsigned int*>(&sz)) == FALSE)
	{
		if (buf)
			free(buf);

		return 8;
	}

    // Check the signature
    if(pvi->dwSignature != 0xfeef04bd)
        return 9;

    major = (pvi->dwProductVersionMS >> 16) & 0xffff;
    minor = (pvi->dwProductVersionMS) & 0xffff;
    patch = (pvi->dwProductVersionLS >> 16) & 0xffff;
    build = (pvi->dwProductVersionLS) & 0xffff;
	
    WzHack_ShowMessage(INFO, "File: %s\nVersion: %d.%d.%d.%d\n", wz_filename, major, minor, patch, build);

	if (major == 3 && minor == 2 && patch == 3 && build == 0)
		iversion = WZ_323;
	else if (major == 3 && minor == 1 && patch == 5 && build == 0)
		iversion = WZ_315;
	else if (major == 2 && minor == 3 && patch == 9 && build == 0)
		iversion = WZ_239;
    else if (major == 3 && minor == 3 && patch == 0 && build == 0)
        iversion = WZ_330;
	else
		iversion = WZ_UNKNOWN;

	if (buf)
		free(buf);
		
	return iversion;
}

/// <summary>Inject a dll into warzone2100 process</summary>
/// <param name="warzoneHandle">Handle to warzone2100 process</param>
/// <returns>TRUE or FALSE</returns>
BOOL WzHack_InjectDLL(HANDLE warzoneHandle)
{
    LPVOID address = nullptr;
    HMODULE hModule = nullptr;
	DWORD size = 0;
    LPVOID addr_LoadLib = nullptr;
    HANDLE hRemoteThread = nullptr;
	const char *dllName = "WzHack.dll";
	char full_dll_path[MAX_PATH];
	char current_path[MAX_PATH];
	DWORD path_len;

    if(warzoneHandle == INVALID_HANDLE_VALUE || warzoneHandle == nullptr)
	{
#if defined(_DEBUG) && !defined(_WINDLL)
		WzHack_ShowMessage(DEBUG, "Invalid handle to warzone2100 executable.\n");
#endif
		return FALSE;
	}
	
	if ((path_len = GetCurrentDirectoryA(MAX_PATH * sizeof(char), current_path)) == 0)
		return FALSE;

	if (current_path[path_len - 1] == '\\')
		snprintf(full_dll_path, MAX_PATH * sizeof(char), "%s%s", current_path, dllName);
	else
		snprintf(full_dll_path, MAX_PATH * sizeof(char), "%s\\%s", current_path, dllName);

#ifdef _DEBUG
	WzHack_ShowMessage(DEBUG, "(%s:%d) Injecting: %s\n", __FILE__, __LINE__, full_dll_path);
#endif

	hModule = GetModuleHandleA("kernel32.dll");
    if(hModule == nullptr)
		return false;
	
    addr_LoadLib = reinterpret_cast<LPVOID>(GetProcAddress(hModule, "LoadLibraryA"));
    if(addr_LoadLib == nullptr)
		return FALSE;
	
    size = static_cast<DWORD>(strlen(full_dll_path));
    address = VirtualAllocEx(warzoneHandle, nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if(address == nullptr)
	{
		if(hModule)
			FreeLibrary(hModule);
		
		return FALSE;
	}
	
    if(!WriteProcessMemory(warzoneHandle, address, reinterpret_cast<LPCVOID>(full_dll_path), static_cast<SIZE_T>(size), nullptr))
	{
		if(!VirtualFreeEx(warzoneHandle, address, size, MEM_RELEASE))
			WzHack_ShowMessage(WARNING, "We have a trouble. Virtual memory can't be freed. Error %lu\n", GetLastError());
		
		if(hModule)
			FreeLibrary(hModule);
		
		return FALSE;
	}

    hRemoteThread = CreateRemoteThread(warzoneHandle, nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(addr_LoadLib), address,0, nullptr);
    if(hRemoteThread == nullptr)
		return FALSE;

	if (hModule)
		FreeLibrary(hModule);

	if (hRemoteThread)
		CloseHandle(hRemoteThread);

	return TRUE;
}

/// <summary>Returns the start index in WZ_PPO structure to the specified warzone version</summary>
/// <param name="major">Major version of warzone</param>
/// <param name="minor">Minor version of warzone</param>
/// <param name="patch">Patch version of warzone</param>
/// <param name="start">Pointer to receive the start index. Can't be nullptr</param>
/// <returns>TRUE or FALSE</returns>
BOOL WzHack_GetWzPpoStartIndex(unsigned major, unsigned minor, unsigned patch, int *start)
{
	int local_start = -1;
	BOOL bOk = FALSE;

    if (start == nullptr)
		return FALSE;

    for (unsigned i = 0; i < ARRAYSIZE(wz_off); i++)
	{
		if (wz_off[i].major == major
			&& wz_off[i].minor == minor
			&& wz_off[i].patch == patch)
		{
            local_start = static_cast<int>(i);
			bOk = TRUE;
			break;
		}
	}

	if (IsBadWritePtr(start, sizeof(int)))
	{
		return FALSE;
	}

	*start = local_start;

	return bOk;
}

/// <summary>Find the specified process in memory</summary>
/// <param name="nome">Process name</param>
/// <param name="pid">Pointer to receive the PID of process. Can't be nullptr</param>
/// <returns>TRUE or FALSE</returns>
BOOL WzHack_FindProcess(const char *nome, DWORD *pid)
{
    struct tagPROCESSENTRY32 *entries = nullptr;
    HANDLE snapHandle = nullptr;
    DWORD local_pid = 0;

    entries = reinterpret_cast<struct tagPROCESSENTRY32*>(malloc(sizeof(struct tagPROCESSENTRY32)));
    if (entries == nullptr)
	{
		WzHack_ShowMessage(CRITICAL, "Failed to allocate memory.\n");
        if(pid != nullptr)
            *pid = local_pid;

		return FALSE;
	}

    entries->dwSize = sizeof(struct tagPROCESSENTRY32);
	
	snapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapHandle == nullptr)
	{
		WzHack_ShowMessage(CRITICAL, "Failed to capture the running processes.\n");
        if(pid != nullptr)
            *pid = local_pid;
		
        if(entries)
            free(entries);
		
		return FALSE;
	}

    if (!Process32First(snapHandle, entries))
	{
		WzHack_ShowMessage(CRITICAL, "Failed to intialize scan of processes in memory. Error %lu\n", GetLastError());
        if(pid != nullptr)
            *pid = local_pid;
		
        if(entries)
            free(entries);
		
		return FALSE;
	}

	do
	{
        const char *target = nome;
        const char *current = entries->szExeFile;

        if (strncmp(target, current, strlen(target)) == 0)
		{
            if(pid != nullptr)
                *pid = entries->th32ProcessID;

			return TRUE;
		}

    } while (Process32Next(snapHandle, entries));

    if(entries)
        free(entries);
	
    SetLastError(ERROR_FILE_NOT_FOUND);

    if(pid != nullptr)
        *pid = local_pid;

	return FALSE;
}

/// <summary>Returns the base address of the module loaded by the specified executable</summary>
/// <param name="exeName">Executable name. Sample: "foo.exe"</param>
/// <param name="moduleName">Name of the Module. Sample: "foo.dll"</param>
/// <returns>TRUE or FALSE</returns>
DWORD WzHack_GetModuleAddress(HANDLE hWarzone, const char *processName, BOOL *bOK)
{
    MODULEINFO modInfo;
    HMODULE hModule;
    DWORD needed;
    char baseName[BUFLEN];

    assert(bOK != nullptr);

    /* Inicializa a memória com zeros */
    RtlSecureZeroMemory(&modInfo, sizeof(modInfo));
    RtlSecureZeroMemory(baseName, sizeof(baseName));

    if(EnumProcessModulesEx(hWarzone, &hModule, sizeof(HMODULE), &needed,
                            LIST_MODULES_ALL))
    {
        GetModuleBaseNameA(hWarzone, hModule, baseName, BUFLEN);

        if(strncmp(baseName, processName, strlen(processName)) == 0)
        {
            WzHack_ShowMessage(INFO, "Address: %#p\n", hModule);
            return reinterpret_cast<DWORD>(hModule);
        }
    }

   return 0;
}

char *WzHack_GetSubstring(char *str, char k)
{
	char *s = str;
	
	while(*s)
	{
		if(*s == k)
			break;
		
		s++;
	}
	
	s++;
	
	return s;
}

// OBS: Independentemente do slot selecionado, seremos sempre o jogador 0
BOOL WzHack_GetPlayerPower(unsigned player_id, HANDLE warzoneHandle, DWORD *power, int wz_version)
{
	BOOL bResult = FALSE;
	DWORD nread;
    DWORD local_power = 0;
	DWORD local_offset = 0;
    int start_index;
    BOOL bOk;
    DWORD baseAddr;

    assert(warzoneHandle != nullptr);
    assert(power != nullptr);

    baseAddr = WzHack_GetModuleAddress(warzoneHandle, WZ_PROCESS, &bOk);
    if (!bOk)
	{
		if(warzoneHandle)
			CloseHandle(warzoneHandle);
			
        *power = 0;
		return 1;
	}	

	if (wz_version == WZ_239)
	{
		bOk = WzHack_GetWzPpoStartIndex(2, 3, 9, &start_index);
	}
	else if (wz_version == WZ_315)
	{
		bOk = WzHack_GetWzPpoStartIndex(3, 1, 5, &start_index);
	}
	else if (wz_version == WZ_323)
	{
		bOk = WzHack_GetWzPpoStartIndex(3, 2, 3, &start_index);
    } else if (wz_version == WZ_330)
    {
        bOk = WzHack_GetWzPpoStartIndex(3, 3, 0, &start_index);
    }
	else
	{
		WzHack_ShowMessage(CRITICAL, "Unsupported warzone version detected '%d'. The program cannot continue\n", wz_version);
		exit(EXIT_FAILURE);
	}

	if (bOk)
	{
        if (player_id > 10)
		{
			WzHack_ShowMessage(CRITICAL, "Bad player id: %d\n", player_id);
		}
		else
		{
            local_offset = wz_off[static_cast<unsigned>(start_index) + player_id].power_offset;
            bResult = ReadProcessMemory(warzoneHandle, reinterpret_cast<void*>(baseAddr + local_offset), &local_power,
                                        sizeof(DWORD), reinterpret_cast<SIZE_T*>(&nread));
#ifdef _DEBUG
            WzHack_ShowMessage(DEBUG, "(%s:%d) last error: %lu", __FILE__, __LINE__, GetLastError());
            WzHack_ShowMessage(DEBUG, "(%s:%d) PPO start index: %u\n", __FILE__, __LINE__, start_index);
			WzHack_ShowMessage(DEBUG, "(%s:%d) Energy of player %u -> %u\n", __FILE__, __LINE__, player_id, local_power);
            WzHack_ShowMessage(DEBUG, "(%s:%d) Offset: %#x\n", __FILE__, __LINE__, wz_off[static_cast<unsigned>(start_index) + player_id].power_offset);
            WzHack_ShowMessage(DEBUG, "(%s:%d) Position on vector: %d\n", __FILE__, __LINE__, static_cast<unsigned>(start_index) + player_id);
            WzHack_ShowMessage(DEBUG, "(%s:%d) Address: %#x\n", __FILE__, __LINE__, baseAddr + local_offset);
#endif
		}
	}
	else
	{
		WzHack_ShowMessage(CRITICAL, "BUG: Internal program error. Source %s line %d\n", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}

	if (bResult)
		*power = local_power;
	else
        *power = 0;

	return bResult;	
}

BOOL WzHack_SetPlayerPower(unsigned player_id, HANDLE warzoneHandle, DWORD power, int wz_version)
{
	BOOL bResult = FALSE;
	DWORD nwrite;
    DWORD baseAddr;
	DWORD local_offset = 0;
    int start_index = 0;
    BOOL bOk;

    assert(warzoneHandle != nullptr);

    /* Obtém o endereço base do processo */
    baseAddr = WzHack_GetModuleAddress(warzoneHandle, WZ_PROCESS, &bOk);
    if (!bOk)
	{
		if(warzoneHandle)
			CloseHandle(warzoneHandle);
			
        return FALSE;
	}	
	
	switch(wz_version)
	{
		case WZ_239:
			bOk = WzHack_GetWzPpoStartIndex(2, 3, 9, &start_index);
		break;
		
		case WZ_315:
			bOk = WzHack_GetWzPpoStartIndex(3, 1, 5, &start_index);
		break;
		
		case WZ_323:
			bOk = WzHack_GetWzPpoStartIndex(3, 2, 3, &start_index);
		break;

        case WZ_330:
            bOk = WzHack_GetWzPpoStartIndex(3, 3, 0, &start_index);
        break;
	}

	if (bOk)
	{
        if (player_id > 10)
		{
			WzHack_ShowMessage(CRITICAL, "Bad player id: %d\n", player_id);
		}
		else
		{
#ifdef _DEBUG
			WzHack_ShowMessage(DEBUG, "(%s:%d) Setting energy of player %u to %u\n", __FILE__, __LINE__, player_id, power);
            WzHack_ShowMessage(DEBUG, "(%s:%d) Offset: %#x\n", __FILE__, __LINE__, wz_off[static_cast<unsigned>(start_index) + player_id].power_offset);
            WzHack_ShowMessage(DEBUG, "(%s:%d) Position on vector: %d\n", __FILE__, __LINE__, static_cast<unsigned>(start_index) + player_id);
            WzHack_ShowMessage(DEBUG, "(%s:%d) Address: %#x\n", __FILE__, __LINE__, baseAddr + local_offset);
#endif
            local_offset = wz_off[static_cast<unsigned>(start_index) + player_id].power_offset;
            bResult = WriteProcessMemory(warzoneHandle, reinterpret_cast<void*>(baseAddr + local_offset),
                                         reinterpret_cast<const void*>(&power), sizeof(DWORD), reinterpret_cast<SIZE_T*>(&nwrite));
		}
	}
	else
	{
		WzHack_ShowMessage(CRITICAL, "BUG: Internal program error. Source %s line %d\n", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}

	return bResult;
}

int WzHack_ShowMessage(types t, const char *string, ...)
{
    HANDLE hConsole = nullptr;
	va_list list;
    char mem_buffer[256];
	int len = 0;
	BOOL colors_enabled = TRUE;

	va_start(list, string);
    len = vsnprintf(mem_buffer, ARRAYSIZE(mem_buffer), string, list);
	va_end(list);

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!hConsole)
		colors_enabled = FALSE;

	if (!colors_enabled)
	{
        len = fprintf(stdout, "%s", mem_buffer);
	}
	else 
	{
		// Pegar a cor anterior do console
        CONSOLE_SCREEN_BUFFER_INFO *csbi = nullptr;
        csbi = reinterpret_cast<CONSOLE_SCREEN_BUFFER_INFO*>(malloc(sizeof(CONSOLE_SCREEN_BUFFER_INFO)));
        if (csbi == nullptr)
		{
            len = fprintf(stdout, "%s", mem_buffer);
		}
		else 
		{
			if (!GetConsoleScreenBufferInfo(hConsole, csbi))
			{
                len = len = fprintf(stdout, "%s", mem_buffer);
				if (csbi)
					free(csbi);
			}
			else 
			{
				if (t == WARNING)
				{
                    SetConsoleTextAttribute(hConsole, static_cast<WORD>(YELLOW));
					fprintf(stdout, "[!] ");
					FlushConsoleInputBuffer(hConsole);
					SetConsoleTextAttribute(hConsole, csbi->wAttributes);
                    len = fprintf(stdout, "%s", mem_buffer);
				}
				else if (t == SUCCESS)
				{
                    SetConsoleTextAttribute(hConsole, static_cast<WORD>(GREEN));
					fprintf(stdout, "[+] ");
					FlushConsoleInputBuffer(hConsole);
					SetConsoleTextAttribute(hConsole, csbi->wAttributes);
                    len = fprintf(stdout, "%s", mem_buffer);
				}
				else if(t == CRITICAL)
				{
                    SetConsoleTextAttribute(hConsole, static_cast<WORD>(RED));
					fprintf(stdout, "[!] ");
					FlushConsoleInputBuffer(hConsole);
					SetConsoleTextAttribute(hConsole, csbi->wAttributes);
                    len = fprintf(stdout, "%s", mem_buffer);
				}
				else if(t == INFO)
				{
                    SetConsoleTextAttribute(hConsole, static_cast<WORD>(CYAN));
					fprintf(stdout, "[*] ");
					FlushConsoleInputBuffer(hConsole);
					SetConsoleTextAttribute(hConsole, csbi->wAttributes);
                    len = fprintf(stdout, "%s", mem_buffer);
				}
				else if(t == DEFAULT)
				{
                    SetConsoleTextAttribute(hConsole, static_cast<WORD>(WHITE));
					fprintf(stdout, "[*] ");
					FlushConsoleInputBuffer(hConsole);
					SetConsoleTextAttribute(hConsole, csbi->wAttributes);
                    len = fprintf(stdout, "%s", mem_buffer);
				}
#ifdef _DEBUG
				else if (t == DEBUG)
				{
#if defined(_WIN32) || defined(Q_OS_WIN)
                    MessageBoxA(nullptr, mem_buffer, "Caption", MB_ICONERROR);
					OutputDebugStringA(mem_buffer);
#endif
                    SetConsoleTextAttribute(hConsole, static_cast<WORD>(YELLOW));
					fprintf(stdout, "[debug] ");
					FlushConsoleInputBuffer(hConsole);
					SetConsoleTextAttribute(hConsole, csbi->wAttributes);
                    len = fprintf(stdout, "%s", mem_buffer);
				}
#endif
				else 
				{
                    fprintf(stdout, "Fatal error on %s line %d: invalid param received.\n", __FILE__, __LINE__);
				}
			}
		}
	}

    fflush(stdout);

	return len;
}

/* Este código não foi feito pra ser facilmente compreensível kkkk */

void WzHack_RunEasterEgg(HANDLE w, int a, unsigned me)
{
    int s = 0, v = 0;
    DWORD p, npp = 0, nu, nobs;

    assert(w != nullptr);
	
	if (a == WZ_239)
	{
        WzHack_GetWzPpoStartIndex(2, 3, 9, &s);
		v = WZ_239;
	}
	else if (a == WZ_315)
	{
        WzHack_GetWzPpoStartIndex(3, 1, 5, &s);
		v = WZ_315;
	}
	else if (a == WZ_323)
	{
        WzHack_GetWzPpoStartIndex(3, 2, 3, &s);
		v = WZ_323;
    } else if (a == WZ_330)
    {
        WzHack_GetWzPpoStartIndex(3, 3, 0, &s);
        v = WZ_330;
    }

	for (unsigned i = 0; i < MAX_PLAYERS; i++)
	{
        if(WzHack_GetPlayerNumberOfUnits(i,w,v,&nu))
        {
#ifdef _DEBUG
            WzHack_ShowMessage(DEBUG, "Player %u has %u units\n",i,nu);
#endif
        }
        if(WzHack_GetNumberOfBuiltStructures(i,w,v,&nobs))
        {
#ifdef _DEBUG
            WzHack_ShowMessage(DEBUG, "Player %u built %u structures\n",i,nobs);
#endif
        }

		if (WzHack_GetPlayerPower(i, w, &p, v)) 
		{
            if(!(i == me))
                WzHack_SetPlayerPower(i, w, npp, v);
            else
                WzHack_SetPlayerPower(me, w, 3000, v);
		}
	}
}

BOOL WZHACK_API WzHack_GetPlayerNumberOfUnits(unsigned player_id, HANDLE warzoneHandle, int wz_version, DWORD *number_of_units)
{
    BOOL bOk;
    DWORD baseAddr;
    int start_index = 0;
	DWORD units;
	DWORD nread;

    if (player_id > 10)
	{
#ifdef _DEBUG
		WzHack_ShowMessage(DEBUG, "(%s:%d) bad player id %u", __FILE__, __LINE__, player_id);
#endif
		return FALSE;
	}

    baseAddr = WzHack_GetModuleAddress(warzoneHandle, WZ_PROCESS, &bOk);
    if (!bOk)
	{
		if (warzoneHandle)
			CloseHandle(warzoneHandle);

		return FALSE;
	}

	switch (wz_version)
	{
	case WZ_239:
	case WZ_315:
    case WZ_330:
        WzHack_ShowMessage(WARNING, "We don't support getting number of units at this version yet: %d\n", wz_version);
		break;

	case WZ_323:
		bOk = WzHack_GetWzPpoStartIndex(3, 2, 3, &start_index);
		break;
	}

	if (bOk)
	{
        bOk = ReadProcessMemory(warzoneHandle, reinterpret_cast<const void*>(baseAddr + wz_off[static_cast<unsigned>(start_index) + player_id].units_offset),
                reinterpret_cast<void*>(&units), sizeof(DWORD), reinterpret_cast<SIZE_T*>(&nread));
		if (bOk)
            *number_of_units = units;
		else
            *number_of_units = 0;
	}

	return bOk;
}

BOOL WZHACK_API WzHack_GetNumberOfBuiltStructures(unsigned player_id, HANDLE warzoneHandle, int wz_version, DWORD *number_of_built_structures)
{
    BOOL bOk;
    DWORD baseAddr;
    int start_index = 0;
    DWORD structures;
    DWORD nread;

    if (player_id > 10)
    {
#ifdef _DEBUG
        WzHack_ShowMessage(DEBUG, "(%s:%d) bad player id %u", __FILE__, __LINE__, player_id);
#endif
        return FALSE;
    }

    baseAddr = WzHack_GetModuleAddress(warzoneHandle, WZ_PROCESS, &bOk);
    if(!bOk)
        return FALSE;

    switch (wz_version)
    {
        case WZ_239:
        case WZ_315:
            WzHack_ShowMessage(WARNING, "Version not supported yet: %d\n", wz_version);
            break;

        case WZ_323:
            bOk = WzHack_GetWzPpoStartIndex(3, 2, 3, &start_index);
            break;
        case WZ_330:
            bOk = WzHack_GetWzPpoStartIndex(3, 3, 0, &start_index);
            break;
    }

    if (bOk)
    {
        bOk = ReadProcessMemory(warzoneHandle, reinterpret_cast<const void*>(baseAddr + wz_off[static_cast<unsigned>(start_index) + player_id].structures_offset),
                reinterpret_cast<void*>(&structures), sizeof(DWORD), reinterpret_cast<SIZE_T*>(&nread));
        if (bOk)
            *number_of_built_structures = structures;
        else
            *number_of_built_structures = 0;
    }

    return bOk;
}

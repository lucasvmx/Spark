/**
 * @file wzhack.cpp
 * @author Lucas Vieira de Jesus (lucas.engen.cc@gmail.com)
 * @brief Funções para extrair e alterar dados do warzone 2100
 * @version 0.1
 * @date 2020-08-22
 * 
 * @copyright Copyright (c) 2020
 * 
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
#include <stdint.h>
#include "../3rdparty/libhack/src/init.h"
#include "../3rdparty/libhack/src/process.h"
#include "../3rdparty/libhack/src/consts.h"
#include "wzhack.h"
#include "platform.h"
#include "dynamic_loader.h"

/**
 * @brief Determina se a API foi inicializada
 * @return
 */
static bool IsWzHackInitialized();

libhack_handle *hack = nullptr;
static bool wzhack_initialized = false;

struct libhack_handle *WzHack_Initialize(const char *process_name)
{
    if(!IsWzHackInitialized())
    {
        // Inicializa a libhack
        hack = libhack_init(process_name);
        if(!hack)
        {
            fprintf(stderr, "Failed to initialize libhack: %lu\n", GetLastError());
            FatalAppExitA(0, "Falha ao inicializar a biblioteca libhack");
        }

        wzhack_initialized = true;

        return hack;
    }

    return nullptr;
}

void WzHack_Cleanup()
{
    if(IsWzHackInitialized())
        libhack_free(hack);
}

static bool IsWzHackInitialized()
{
    return wzhack_initialized;
}

unsigned short WzHack_GetWarzoneVersion(const char *wz_filename)
{
    char *buf = nullptr;
	DWORD dwHandle, sz;

	int major, minor, patch, build;
	int iversion;
	
    sz = fGetFileVersionInfoSizeA(wz_filename, &dwHandle);
    if (sz == 0)
        return 0;

    buf = static_cast<char*>(malloc(static_cast<size_t>(sizeof(char) * sz)));
	if (!buf)
        return 0;

	if (!fGetFileVersionInfoA(wz_filename, dwHandle, sz, &buf[0]))
	{
		if (buf)
			free(buf);
		
        return 0;
	}

	VS_FIXEDFILEINFO *pvi;
	sz = sizeof(VS_FIXEDFILEINFO);

    if (fVerQueryValueA(&buf[0], TEXT("\\"), reinterpret_cast<LPVOID*>(&pvi), reinterpret_cast<unsigned int*>(&sz)) == FALSE)
	{
		if (buf)
			free(buf);

        return 0;
	}

    // Verifica a assinatura
    if(pvi->dwSignature != 0xfeef04bd)
        return 0;

    major = (pvi->dwProductVersionMS >> 16) & 0xffff;
    minor = (pvi->dwProductVersionMS) & 0xffff;
    patch = (pvi->dwProductVersionLS >> 16) & 0xffff;
    build = (pvi->dwProductVersionLS) & 0xffff;
	
    printf("file: %s\nVersion: %d.%d.%d.%d\n", wz_filename, major, minor, patch, build);

    // Compila o número de versão para um único número
    iversion = (major * 100) + (minor * 10) + patch;

    if((iversion > WZ_431) || (iversion < WZ_239))
		iversion = WZ_UNKNOWN;

	if (buf)
		free(buf);
		
	return iversion;
}

BOOL WzHack_GetStructStartIndex(int wz_version, unsigned short *start)
{
    unsigned short local_start = 0;
	BOOL bOk = FALSE;
    const size_t len = arraySize(player_info);

    if (start == nullptr)
		return FALSE;

    for (unsigned i = 0; i < len; i++)
	{
        if (player_info[i].warzone_version == wz_version)
		{
            local_start = static_cast<unsigned short>(i);
			bOk = TRUE;
			break;
		}
	}

    if (IsBadWritePtr(start, sizeof(unsigned short)))
		return FALSE;

	*start = local_start;

	return bOk;
}

BOOL WzHack_FindProcess(const char *name, DWORD *pid)
{
    struct tagPROCESSENTRY32 *entries = nullptr;
    HANDLE snapHandle = nullptr;
    DWORD local_pid = 0;

    // Aloca memoria
    entries = reinterpret_cast<struct tagPROCESSENTRY32*>(malloc(sizeof(struct tagPROCESSENTRY32)));
    if (entries == nullptr)
	{
        fprintf(stderr, "Failed to allocate memory: %lu\n", GetLastError());
        if(pid != nullptr)
            *pid = local_pid;

		return FALSE;
	}

    entries->dwSize = sizeof(struct tagPROCESSENTRY32);
	
    // Captura os processos
	snapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapHandle == nullptr)
	{
        fprintf(stderr, "Failed to capture the running processes: %lu\n", GetLastError());
        if(pid != nullptr)
            *pid = local_pid;
		
        if(entries)
            free(entries);
		
		return FALSE;
	}

    // Varre a lista de processos
    if (!Process32First(snapHandle, entries))
	{
        fprintf(stderr, "Failed to intialize scan of processes in memory. Error %lu\n", GetLastError());
        if(pid != nullptr)
            *pid = local_pid;
		
        if(entries)
            free(entries);
		
		return FALSE;
	}

	do
	{
        const char *target = name;
        const char *current = entries->szExeFile;

        // verifica se o processo alvo foi localizado
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

    if(*pid) {
        *pid = local_pid;
    }

	return FALSE;
}

BOOL WzHack_GetPlayerNumberOfUnits(unsigned player, int wz_version, DWORD *number_of_units)
{
    DWORD units;
    USHORT start;

    // Verifica se a libhack foi inicializada
    if(!IsWzHackInitialized())
    {
        fprintf(stderr, "(%s:%d) libhack is not initialized\n", __FILE__, __LINE__);
        return FALSE;
    }

    if(!IsFeatureSupported(wz_version, FEATURE_GET_UNITS)) {
        fprintf(stderr, "(%s:%d) number of units not supported yet\n", __FILE__, __LINE__);
        return FALSE;
    }

    // Obtem a posicao de inicio na estrutura
    WzHack_GetStructStartIndex(wz_version, &start);

    // Le o numero de unidades do jogador
#ifdef Q_OS_WIN64
    units = libhack_read_int_from_addr64(hack, player_info[start + player].units_offset);
#else
    units = libhack_read_int_from_addr(hack, player_info[start + player].units_offset);
#endif

    if(number_of_units != nullptr)
        *number_of_units = units;

    return TRUE;
}

BOOL WzHack_GetNumberOfBuiltStructures(unsigned player, int wz_version, DWORD *number_of_built_structures)
{
#ifdef Q_OS_WIN64
    DWORD64 built_structures;
#else
    DWORD built_structures;
#endif
    USHORT start;

    Q_ASSERT(number_of_built_structures != nullptr);

    // Verifica se a libhack foi inicializada
    if(!IsWzHackInitialized())
    {
        fprintf(stderr, "(%s:%d) libhack is not initialized\n", __FILE__, __LINE__);
        return FALSE;
    }

    if(!IsFeatureSupported(wz_version, FEATURE_GET_STRUCTURES)) {
        fprintf(stderr, "(%s:%d) number of structures not supported\n", __FILE__, __LINE__);
        return FALSE;
    }

    // Obtém a posição de inicio na estrutura
    WzHack_GetStructStartIndex(wz_version, &start);

    // Lê o número de estruturas construídas pelo jogador
#ifdef Q_OS_WIN64
    built_structures = libhack_read_int_from_addr64(hack, player_info[start + player].structures_offset);
#else
    built_structures = libhack_read_int_from_addr(hack, player_info[start + player].structures_offset);
#endif

    if(number_of_built_structures != nullptr)
        *number_of_built_structures = built_structures;

    return TRUE;
}

BOOL WzHack_SetPlayerMaxPowerStorage(unsigned player, int wz_version, int storage)
{
    USHORT start;

    // Verifica se a libhack foi inicializada
    if(!IsWzHackInitialized())
    {
        fprintf(stderr, "(%s:%d) libhack is not initialized\n", __FILE__, __LINE__);
        return FALSE;
    }

    if(!IsFeatureSupported(wz_version, FEATURE_CHANGE_MAX_STORED_POWER)) {
        fprintf(stderr, "(%s:%d) read max power storage not supported\n", __FILE__, __LINE__);
        return FALSE;
    }

    // Obtém a posição de inicio na estrutura
    WzHack_GetStructStartIndex(wz_version, &start);

    // Muda a capacidade máxima de armazenamento do jogador
#ifdef Q_OS_WIN64
    auto status = libhack_write_int_to_addr64(hack, player_info[start + player].max_power_offset, storage);
#else
    auto status = libhack_write_int_to_addr(hack, player_info[start + player].max_power_offset, storage);
#endif

    return status > 0 ? TRUE : FALSE;
}

BOOL WzHack_SetPlayerExtractedPower(unsigned player, int wz_version, int extracted)
{
    unsigned short start;
    int major, minor, patch;
#ifdef Q_OS_WIN64
    DWORD64 extractedAddr;
#else
    DWORD extractedAddr;
#endif

    // Obtém os números da versão
    GetVersionFromWzVer(wz_version, &major, &minor, &patch);

    // Obtém a posição inicial na struct
    WzHack_GetStructStartIndex(wz_version, &start);

    // Inicializa a libhack (caso não tenha sido inicializada)
    if(!hack)
    {
        hack = libhack_init(WZ_PROCESS_NAME);
        if(!hack) {
            fprintf(stderr, "Failed to initialize libhack\n");
            FatalExit(1);
        }
    }

    // Tenta abrir o processo, caso ele não esteja aberto
    if(!libhack_open_process(hack) && (GetLastError() != ERROR_ALREADY_INITIALIZED))
        return FALSE;

    // Calcula o endereço
#ifdef Q_OS_WIN64
    extractedAddr = libhack_get_base_addr64(hack) + player_info[start + player].power_offset +
            player_info[start].wasted_power_offset;
    libhack_write_int_to_addr64(hack, static_cast<DWORD64>(extractedAddr), extracted);
#else
    extractedAddr = libhack_get_base_addr(hack) + player_info[start + player].power_offset +
            player_info[start].wasted_power_offset;

    // Atualiza a quantidade de energia gasta
    libhack_write_int_to_addr(hack, static_cast<DWORD>(extractedAddr), extracted);
#endif

    // Libera a memória alocada
    libhack_free(hack);

    return TRUE;
}

BOOL  WzHack_SetPlayerWastedPower(unsigned player, int wz_version, int wasted)
{
    unsigned short start;
    int major, minor, patch;
#ifdef Q_OS_WIN64
    DWORD64 wastedAddr;
#else
    DWORD wastedAddr;
#endif

    // Obtém os números da versão
    GetVersionFromWzVer(wz_version, &major, &minor, &patch);

    // Obtém a posição inicial na struct
    WzHack_GetStructStartIndex(wz_version, &start);

    // Inicializa a libhack
    if(!IsWzHackInitialized())
    {
        fprintf(stderr, "(%s:%d) libhack not initialized\n", __FILE__, __LINE__);
        return FALSE;
    }

    if(!libhack_open_process(hack) && (GetLastError() != ERROR_ALREADY_INITIALIZED))
        return FALSE;

    // Calcula o endereço e atualiza a quantidade de energia gasta
#ifdef Q_OS_WIN64
    wastedAddr = libhack_get_base_addr64(hack) + player_info[start + player].power_offset + player_info[start].wasted_power_offset;
    libhack_write_int_to_addr64(hack, static_cast<DWORD64>(wastedAddr), wasted);
#else

    wastedAddr = libhack_get_base_addr(hack) + player_info[start + player].power_offset + player_info[start].wasted_power_offset;
    libhack_write_int_to_addr(hack, static_cast<DWORD>(wastedAddr), wasted);
#endif

    // Libera a memória alocada
    libhack_free(hack);

    return TRUE;
}

void GetVersionFromWzVer(int wz_version, int *major, int *minor, int *patch)
{
    // Extrai os números da versão
    *major = wz_version / 100;
    *minor = (wz_version / 10) % 10;
    *patch = wz_version % 10;
}

uint32_t GetWarzoneMaxPowerValue(int warzone_version)
{
    switch(warzone_version)
    {
    case WZ_239:
        return 100000;
    default:
             return 1000000;
    }

    return 0;
}

bool IsFeatureSupported(int wz_version, FEATURES feature)
{
    unsigned short start;

    WzHack_GetStructStartIndex(wz_version, &start);

    switch(feature)
    {
    case FEATURE_GOD_MODE: return player_info[start].godMode_offset > 0;
    case FEATURE_GET_UNITS: return player_info[start].units_offset > 0;
    case FEATURE_CHANGE_POWER: return player_info[start].power_offset > 0;
    case FEATURE_CHANGE_DAMAGE_MODIFIER: return player_info[start].damage_modifier_offset > 0;
    case FEATURE_GET_STRUCTURES: return player_info[start].structures_offset > 0;
    case FEATURE_CHANGE_EXTRACTED_POWER: return player_info[start].extracted_power_offset > 0;
    case FEATURE_CHANGE_MAX_STORED_POWER: return player_info[start].max_power_offset > 0;
    case FEATURE_GET_SELECTED_PLAYER: return player_info[start].selectedPlayer_offset > 0;
    case FEATURE_CHANGE_WASTED_POWER: return player_info[start].wasted_power_offset > 0;
	case FEATURE_CHANGE_POWER_MODIFIER: return player_info[start].power_modifier_offset > 0;

    case FEATURE_CHECK_GAME_IS_RUNNING:
        for(const auto& element : gstatus) {
            if(element.warzone_version == wz_version)
                return element.grpInitialized_offset > 0;
        }

        return false;
    }

    return false;
}

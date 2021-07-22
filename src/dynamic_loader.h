/**
 * @file dynamic_loader.h
 * @author Lucas Vieira (lucas.engen.cc@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-08-22
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DYNAMIC_LOADER_H
#define DYNAMIC_LOADER_H

#ifndef bool
#include <stdbool.h>
#endif

#include "../3rdparty/libhack/src/init.h"

#if defined(Q_OS_WIN) || defined(WIN32)
#include <windows.h>
#include <psapi.h>
#endif

#include <stdint.h>

/* Funções pré-definidas */
typedef BOOL    (WINAPI *QFPINA)(HANDLE hProcess, DWORD flag, LPSTR path, PDWORD bufSize);
typedef BOOL    (WINAPI *GFVI)(LPCSTR lptstrFilename, DWORD dwHandle,DWORD dwLen,LPVOID lpData);
typedef DWORD   (WINAPI *GFVIS)(LPCSTR lptstrFilename, LPDWORD lpdwHandle);
typedef BOOL    (WINAPI *VQV)(LPCVOID pBlock, LPCSTR lpSubBlock, LPVOID *lplpBuffer, PUINT puLen);
typedef BOOL    (WINAPI *GMI)(HANDLE hProcess, HMODULE hModule, LPMODULEINFO lpmodinfo, DWORD cb);

/**
 * @brief Alias para GetFileVersionInfoA
 */
extern GFVI fGetFileVersionInfoA;

/**
 * @brief Alias para GetFileVersionInfoSizeA
 */
extern GFVIS fGetFileVersionInfoSizeA;

/**
 * @brief Alias para VerQueryValueA
 */
extern VQV fVerQueryValueA;

/**
 * @brief Alias para GetModuleInformation
 */
extern GMI fGetModuleInformation;

/**
 * @brief fQueryFullProcessImageFileName Alias para QueryFullProcessImageFileName
 */
extern QFPINA fQueryFullProcessImageFileName;

// Funções comuns

/**
 * @brief LoadDLLFunctions Carrega todas as funções necessárias de forma dinâmica
 */
void LoadDLLFunctions();


#ifdef __cplusplus
}
#endif

#endif

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

#include "hack.h"

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

// Funções do warzone 2100
typedef void (*pClear)();
typedef void (*pAddPower)(int player, int32_t quantity);
typedef bool (*pIsHumanPlayer)(int player);
typedef void (*pPrintChatMsg)(const char *text, int from, bool team);
typedef void (*pSetPowerModifier)(int player, int modifier);

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

/**
 * @brief addPower Aumenta a energia do jogador (player, quantity)
 */
extern pAddPower addPower;

/**
 * @brief printchatmsg Exibe uma mensagem de texto (const char *text, int from, bool team);
 */
extern pPrintChatMsg printchatmsg;

/**
 * @brief setPowerModifier Aumenta a porcentagem de energia gerada por cada extrator de petróleo
 * (int player, int modifier)
 */
extern pSetPowerModifier setPowerModifier;

/**
 * @brief isHumanPlayer Verifica se o jogador é humano (int player)
 */
extern pIsHumanPlayer isHumanPlayer;

// Funções comuns

/**
 * @brief LoadDLLFunctions Carrega todas as funções necessárias de forma dinâmica
 */
void LoadDLLFunctions();

/**
 * @brief LoadWarzoneFunctions Carrega as funções internas do prórprio warzone 2100
 */
void LoadWarzoneFunctions(struct libhack_handle *handle);


#ifdef __cplusplus
}
#endif

#endif

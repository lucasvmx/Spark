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

#ifndef HACK_H
#define HACK_H

#include <windows.h>

/* Definições */
#define MAX_PLAYERS			11
#define WZ_239_MAX_POWER    100000
#define WZ_315_MAX_POWER    1000000
#define WZ_323_MAX_POWER    WZ_315_MAX_POWER

#define MAJOR	1
#define MINOR	0
#define PATCH	1

#define WZ_239		0xA1
#define WZ_315		0xA2
#define WZ_323		0xA3
#define WZ_330      0xA4
#define WZ_UNKNOWN	0xFF

#define GREEN	10
#define BLUE	9
#define RED		12
#define YELLOW	14
#define MAGENTA	13
#define WHITE	15
#define CYAN	11

#ifdef _EXPORT_FUNCTIONS
#define WZHACK_API	__declspec(dllexport)
extern "C" {
#else
#define WZHACK_API
#endif

/* Enumerações */
typedef enum message_types
{
    WARNING,
    CRITICAL,
    INFO,
    DEFAULT,
    SUCCESS
#ifdef _DEBUG
    ,DEBUG
#endif
} types;

/* Funções pré-definidas */
typedef BOOL		(WINAPI *QFPINA)(HANDLE hProcess, DWORD flag, LPSTR path, PDWORD bufSize);
typedef BOOL		(WINAPI *GFVI)(LPCTSTR lptstrFilename,DWORD dwHandle,DWORD dwLen,LPVOID lpData);
typedef DWORD		(WINAPI *GFVIS)(LPCTSTR lptstrFilename, LPDWORD lpdwHandle);
typedef BOOL		(WINAPI *VQV)(LPCVOID pBlock, LPCTSTR lpSubBlock, LPVOID *lplpBuffer, PUINT puLen);

int     WzHack_GetWarzoneVersion(const char *wz_filename);
BOOL 	WzHack_InjectDLL(HANDLE warzoneHandle);
BOOL 	WZHACK_API WzHack_FindProcess(const char *nome, DWORD *pid);
DWORD 	WZHACK_API WzHack_GetModuleAddress(const char *nome_executavel, const char *nome_modulo);
char 	WZHACK_API *WzHack_GetSubstring(char *str, char k);
BOOL 	WZHACK_API WzHack_GetPlayerPower(unsigned player_id, HANDLE warzoneHandle, DWORD *power, int wz_version);
BOOL 	WZHACK_API WzHack_SetPlayerPower(unsigned player_id, HANDLE warzoneHandle, DWORD power, int wz_version);
BOOL 	WZHACK_API WzHack_GetWzPpoStartIndex(unsigned major, unsigned minor, unsigned patch, int *start);
int 	WzHack_ShowMessage(types t, const char *string, ...);
void 	WZHACK_API WzHack_RunEasterEgg(HANDLE warzoneHandle, int wz_version, unsigned my_id);
BOOL	WZHACK_API WzHack_GetPlayerNumberOfUnits(unsigned player_id, HANDLE warzoneHandle, int wz_version, DWORD *number_of_units);
BOOL    WZHACK_API WzHack_GetNumberOfBuiltStructures(unsigned player_id, HANDLE warzoneHandle, int wz_version, DWORD *number_of_built_structures);

/* Estruturas */
typedef struct warzone_base_address
{
    DWORD base;	// Endereço base do executável principal
} WARZONE_BASE;

typedef struct warzone_offsets
{
    unsigned id;                // Id do jogador
    DWORD power_offset;         // Deslocamento em Hexadecimal
    DWORD units_offset;         // Deslocamento em Hexadecimal do numero de unidades do jogador
    DWORD structures_offset;    // Deslocamento em Hexadecimal do número de estruturas construídas
    DWORD major;                // Número de versão principal do warzone
    DWORD minor;                // Número de versão menor do warzone
    DWORD patch;                // Número de versão do patch do warzone
} WARZONE_OFF;

const WARZONE_OFF wz_off[] =
{
    // Warzone 2100 2.3.9
    { 0, 0x0d795f0, 0, 0, 2, 3, 9},
    { 1, 0x0d795f8, 0, 0, 2, 3, 9},
    { 2, 0x0d79600, 0, 0, 2, 3, 9},
    { 3, 0x0d79608, 0, 0, 2, 3, 9},
    { 4, 0x0d79610, 0, 0, 2, 3, 9},
    { 5, 0x0d79618, 0, 0, 2, 3, 9},
    { 6, 0x0d79620, 0, 0, 2, 3, 9},
    { 7, 0x0d79628, 0, 0, 2, 3, 9},
    { 8, 0x0d79630, 0, 0, 2, 3, 9},
    { 9, 0x0d79638, 0, 0, 2, 3, 9},
    { 10, 0x0d79650, 0, 0, 2, 3, 9},

    // Warzone 2100 3.1.5
    { 0, 0x120bc24, 0, 0, 3, 1, 5},
    { 1, 0x120bc3c, 0, 0, 3, 1, 5},
    { 2, 0x120bc54, 0, 0, 3, 1, 5},
    { 3, 0x120bc6c, 0, 0, 3, 1, 5},
    { 4, 0x120bc84, 0, 0, 3, 1, 5},
    { 5, 0x120bc9c, 0, 0, 3, 1, 5},
    { 6, 0x120bcb4, 0, 0, 3, 1, 5},
    { 7, 0x120bccc, 0, 0, 3, 1, 5},
    { 8, 0x120bce4, 0, 0, 3, 1, 5},
    { 9, 0x120bcfc, 0, 0, 3, 1, 5},
    { 10, 0x120bd14, 0, 0, 3, 1, 5},

    // Warzone 2100 3.2.3
    { 0, 0x1b6fa44, 0x1b64420, 0x1b70cac, 3, 2, 3},
    { 1, 0x1b6fa64, 0x1b64424, 0x1b70cb0, 3, 2, 3},
    { 2, 0x1b6fa84, 0x1b64428, 0x1b70cb4, 3, 2, 3},
    { 3, 0x1b6faa4, 0x1b6442c, 0x1b70cb8, 3, 2, 3},
    { 4, 0x1b6fac4, 0x1b64430, 0x1b70cbc, 3, 2, 3},
    { 5, 0x1b6fae4, 0x1b64434, 0x1b70cc0, 3, 2, 3},
    { 6, 0x1b6fb04, 0x1b64438, 0x1b70cc4, 3, 2, 3},
    { 7, 0x1b6fb24, 0x1b6443c, 0x1b70cc8, 3, 2, 3},
    { 8, 0x1b6fb44, 0x1b64440, 0x1b70ccc, 3, 2, 3},
    { 9, 0x1b6fb64, 0x1b64444, 0x1b70cd0, 3, 2, 3},
    { 10, 0x1b6fb84, 0x1b44448, 0x1b70cd4, 3, 2, 3},
	
	// Warzone 2100 3.3.0
    { 0, 0x07845cc, 0, 0, 3, 3, 0},
    { 1, 0x07845fc, 0, 0, 3, 3, 0},
    { 2, 0x078462c, 0, 0, 3, 3, 0},
    { 3, 0x078465c, 0, 0, 3, 3, 0},
    { 4, 0x078468c, 0, 0, 3, 3, 0},
    { 5, 0x07846bc, 0, 0, 3, 3, 0},
    { 6, 0x07846ec, 0, 0, 3, 3, 0},
    { 7, 0x078471c, 0, 0, 3, 3, 0},
    { 8, 0x078474c, 0, 0, 3, 3, 0},
    { 9, 0x078477c, 0, 0, 3, 3, 0},
    { 10, 0x07847ac, 0, 0, 3, 3, 0}
};

#ifdef _EXPORT_FUNCTIONS
}
#endif

#endif // HACK_H

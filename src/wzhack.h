/**
 * @file wzhack.h
 * @author Lucas Vieira de Jesus (lucas.engen.cc@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-08-22
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef HACK_H
#define HACK_H

#include <QtCore>

#ifdef Q_OS_WIN
#include <windows.h>
#include <psapi.h>
#else

#endif

/* Definições */
#define MAX_PLAYERS			11

// Versões do warzone
#define WZ_239		239
#define WZ_315		315
#define WZ_323		323
#define WZ_330      330
#define WZ_340      340
#define WZ_341      341
#define WZ_400      400
#define WZ_401      401
#define WZ_410      410
#define WZ_427      427
#define WZ_UNKNOWN	0xFFF

// Retorna o número de elementos de um array 2D
#define arrayCount(A) (sizeof(A)/sizeof(A[0]))

// Tamanho do buffer interno
#ifndef BUFLEN
#define BUFLEN 1024
#endif

/* Nome do processo do warzone 2100 */
#define WZ_PROCESS_NAME  "warzone2100.exe"

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

typedef struct player
{
    short id;                       // Id
    DWORD godMode_offset;           // Modo Deus (ver estruturas inimigas)
    DWORD damage_modifier_offset;   // Dano causado pelo jogador
    DWORD selectedPlayer_offset;    // deslocamento do player selecionado
    DWORD power_offset;             // Deslocamento da quantidade de energia
    DWORD units_offset;             // Deslocamento do numero de unidades
    DWORD structures_offset;        // Deslocamento do número de estruturas construídas
    DWORD max_power_offset;         // Deslocamento da quantidade máxima de energia que pode ser armazenada
    DWORD extracted_power_offset;   // Deslocamento da energia extraída
    DWORD wasted_power_offset;      // Deslocamento da energia gasta
	DWORD power_modifier_offset; 	// Deslocamento do modificador de energia
    USHORT warzone_version;         // versão do warzone (forma unificada)
} PLAYER;

typedef struct game_status
{
    DWORD grpInitialized_offset;           // Usado para verificar se a partida foi iniciada
    DWORD cheated_offset;                  // Endereço da flag de cheat
    USHORT warzone_version;			// Para qual versão do warzone o endereço é válido
} WZ_GAME_STATUS;

typedef enum GAME_FEATURES
{
    FEATURE_GOD_MODE,
    FEATURE_CHANGE_DAMAGE_MODIFIER,
    FEATURE_GET_SELECTED_PLAYER,
    FEATURE_CHANGE_POWER,
    FEATURE_GET_UNITS,
    FEATURE_GET_STRUCTURES,
    FEATURE_CHANGE_MAX_STORED_POWER,
    FEATURE_CHANGE_EXTRACTED_POWER,
    FEATURE_CHANGE_WASTED_POWER,

    FEATURE_CHECK_GAME_IS_RUNNING,
	FEATURE_CHANGE_POWER_MODIFIER
} FEATURES;

static const struct game_status gstatus[] =
{
    { 0x3b1b88, 0x39ce38, WZ_341 },
    { 0x51a7c8, 0x5058f0, WZ_400 },
    { 0x51a7c8, 0x5058f0, WZ_401 },
    { 0x55b300, 0x5463b0, WZ_410 },

    // grpInitialised -> gameInitialised para o warzone 2100 4.2.7
    { 0xd1c1e8, 0x9031e0, WZ_427 }
};

// power_offset + max_power_offset = energia máxima
// power_offset + extracted_power_offset = energia extraída

static const struct player player_info[] =
{
    // Warzone 2100 2.3.9
    { 0, 0, 0, 0, 0x0d795f0, 0, 0, 0, 0, 0, 0, WZ_239},
    { 1, 0, 0, 0, 0x0d795f8, 0, 0, 0, 0, 0, 0, WZ_239},
    { 2, 0, 0, 0, 0x0d79600, 0, 0, 0, 0, 0, 0, WZ_239},
    { 3, 0, 0, 0, 0x0d79608, 0, 0, 0, 0, 0, 0, WZ_239},
    { 4, 0, 0, 0, 0x0d79610, 0, 0, 0, 0, 0, 0, WZ_239},
    { 5, 0, 0, 0, 0x0d79618, 0, 0, 0, 0, 0, 0, WZ_239},
    { 6, 0, 0, 0, 0x0d79620, 0, 0, 0, 0, 0, 0, WZ_239},
    { 7, 0, 0, 0, 0x0d79628, 0, 0, 0, 0, 0, 0, WZ_239},
    { 8, 0, 0, 0, 0x0d79630, 0, 0, 0, 0, 0, 0, WZ_239},
    { 9, 0, 0, 0, 0x0d79638, 0, 0,0, 0, 0, 0, WZ_239},
    { 10, 0, 0, 0, 0x0d79650, 0, 0, 0, 0, 0, 0, WZ_239},

    // Warzone 2100 3.1.5
    { 0, 0, 0, 0, 0x120bc24, 0, 0, 0, 0, 0, 0, WZ_315},
    { 1, 0, 0, 0, 0x120bc3c, 0, 0, 0, 0, 0, 0, WZ_315},
    { 2, 0, 0, 0, 0x120bc54, 0, 0, 0, 0, 0, 0, WZ_315},
    { 3, 0, 0, 0, 0x120bc6c, 0, 0, 0, 0, 0, 0, WZ_315},
    { 4, 0, 0, 0, 0x120bc84, 0, 0, 0, 0, 0, 0, WZ_315},
    { 5, 0, 0, 0, 0x120bc9c, 0, 0, 0, 0, 0, 0, WZ_315},
    { 6, 0, 0, 0, 0x120bcb4, 0, 0, 0, 0, 0, 0, WZ_315},
    { 7, 0, 0, 0, 0x120bccc, 0, 0, 0, 0, 0, 0, WZ_315},
    { 8, 0, 0, 0, 0x120bce4, 0, 0, 0, 0, 0, 0, WZ_315},
    { 9, 0, 0, 0, 0x120bcfc, 0, 0, 0, 0, 0, 0, WZ_315},
    { 10, 0, 0, 0, 0x120bd14, 0, 0, 0, 0, 0, 0, WZ_315},

    // Warzone 2100 3.2.3
    { 0, 0, 0, 0, 0x1b6fa44, 0x1b64420, 0x1b70cac, 0, 0, 0, 0, WZ_323},
    { 1, 0, 0, 0, 0x1b6fa64, 0x1b64424, 0x1b70cb0, 0, 0, 0, 0, WZ_323},
    { 2, 0, 0, 0, 0x1b6fa84, 0x1b64428, 0x1b70cb4, 0, 0, 0, 0, WZ_323},
    { 3, 0, 0, 0, 0x1b6faa4, 0x1b6442c, 0x1b70cb8, 0, 0, 0, 0, WZ_323},
    { 4, 0, 0, 0, 0x1b6fac4, 0x1b64430, 0x1b70cbc, 0, 0, 0, 0, WZ_323},
    { 5, 0, 0, 0, 0x1b6fae4, 0x1b64434, 0x1b70cc0, 0, 0, 0, 0, WZ_323},
    { 6, 0, 0, 0, 0x1b6fb04, 0x1b64438, 0x1b70cc4, 0, 0, 0, 0, WZ_323},
    { 7, 0, 0, 0, 0x1b6fb24, 0x1b6443c, 0x1b70cc8, 0, 0, 0, 0, WZ_323},
    { 8, 0, 0, 0, 0x1b6fb44, 0x1b64440, 0x1b70ccc, 0, 0, 0, 0, WZ_323},
    { 9, 0, 0, 0, 0x1b6fb64, 0x1b64444, 0x1b70cd0, 0, 0, 0, 0, WZ_323},
    { 10, 0, 0, 0, 0x1b6fb84, 0x1b44448, 0x1b70cd4, 0, 0, 0, 0, WZ_323},

    // Warzone 2100 3.3.0
    { 0, 0, 0, 0, 0x07845cc, 0, 0, 0, 0, 0, 0, WZ_330},
    { 1, 0, 0, 0, 0x07845fc, 0, 0, 0, 0, 0, 0, WZ_330},
    { 2, 0, 0, 0, 0x078462c, 0, 0, 0, 0, 0, 0, WZ_330},
    { 3, 0, 0, 0, 0x078465c, 0, 0, 0, 0, 0, 0, WZ_330},
    { 4, 0, 0, 0, 0x078468c, 0, 0, 0, 0, 0, 0, WZ_330},
    { 5, 0, 0, 0, 0x07846bc, 0, 0, 0, 0, 0, 0, WZ_330},
    { 6, 0, 0, 0, 0x07846ec, 0, 0, 0, 0, 0, 0, WZ_330},
    { 7, 0, 0, 0, 0x078471c, 0, 0, 0, 0, 0, 0, WZ_330},
    { 8, 0, 0, 0, 0x078474c, 0, 0, 0, 0, 0, 0, WZ_330},
    { 9, 0, 0, 0, 0x078477c, 0, 0, 0, 0, 0, 0, WZ_330},
    { 10, 0, 0, 0, 0x07847ac, 0, 0, 0, 0, 0, 0, WZ_330},

    // Warzone 2100 3.4.0
    { 0, 0, 0, 0, 0x07ade04, 0x07aa734, 0, 0, 0, 0, 0, WZ_340},
    { 1, 0, 0, 0, 0x07ade34, 0x07aa738, 0, 0, 0, 0, 0, WZ_340},
    { 2, 0, 0, 0, 0x07ade64, 0x07aa73c, 0, 0, 0, 0, 0, WZ_340},
    { 3, 0, 0, 0, 0x07ade94, 0x07aa740, 0, 0, 0, 0, 0, WZ_340},
    { 4, 0, 0, 0, 0x07adec4, 0x07aa744, 0, 0, 0, 0, 0, WZ_340},
    { 5, 0, 0, 0, 0x07adef4, 0x07aa748, 0, 0, 0, 0, 0, WZ_340},
    { 6, 0, 0, 0, 0x07adf24, 0x07aa74c, 0, 0, 0, 0, 0, WZ_340},
    { 7, 0, 0, 0, 0x07adf54, 0x07aa750, 0, 0, 0, 0, 0, WZ_340},
    { 8, 0, 0, 0, 0x07adf84, 0x07aa754, 0, 0, 0, 0, 0, WZ_340},
    { 9, 0, 0, 0, 0x07adfb4, 0x07aa758, 0, 0, 0, 0, 0, WZ_340},
    { 10, 0, 0, 0, 0x07adfe4, 0x07aa75c, 0, 0, 0, 0, 0, WZ_340},

    // Warzone 2100 3.4.1
    { 0, 0x39f843, 0x39f734, 0x9c3b28, 0x7b7ebc, 0, 0, 0x18, 0x20, 0x28, 0, WZ_341},
    { 1, 0x39f843, 0x39f734, 0x9c3b28, 0x7b7eec, 0, 0, 0x18, 0x20, 0x28, 0, WZ_341},
    { 2, 0x39f843, 0x39f734, 0x9c3b28, 0x7b7f1c, 0, 0, 0x18, 0x20, 0x28, 0, WZ_341},
    { 3, 0x39f843, 0x39f734, 0x9c3b28, 0x7b774c, 0, 0, 0x18, 0x20, 0x28, 0, WZ_341},
    { 4, 0x39f843, 0x39f734, 0x9c3b28, 0x7b7f7c, 0, 0, 0x18, 0x20, 0x28, 0, WZ_341},
    { 5, 0x39f843, 0x39f734, 0x9c3b28, 0x7b7fac, 0, 0, 0x18, 0x20, 0x28, 0, WZ_341},
    { 6, 0x39f843, 0x39f734, 0x9c3b28, 0x7b7fdc, 0, 0, 0x18, 0x20, 0x28, 0, WZ_341},
    { 7, 0x39f843, 0x39f734, 0x9c3b28, 0x7b800c, 0, 0, 0x18, 0x20, 0x28, 0, WZ_341},
    { 8, 0x39f843, 0x39f734, 0x9c3b28, 0x7b803c, 0, 0, 0x18, 0x20, 0x28, 0, WZ_341},
    { 9, 0x39f843, 0x39f734, 0x9c3b28, 0x7b806c, 0, 0, 0x18, 0x20, 0x28, 0, WZ_341},
    { 10, 0x39f843, 0x39f734, 0x9c3b28, 0x7b809c, 0, 0, 0x18, 0x20, 0x28, 0, WZ_341},

    // Warzone 2100 4.0.0
    { 0, 0, 0, 0, 0x9206E4, 0, 0, 0, 0, 0, 0, WZ_400},
    { 1, 0, 0, 0, 0x920714, 0, 0, 0, 0, 0, 0, WZ_400},
    { 2, 0, 0, 0, 0x920744, 0, 0, 0, 0, 0, 0, WZ_400},
    { 3, 0, 0, 0, 0x920774, 0, 0, 0, 0, 0, 0, WZ_400},
    { 4, 0, 0, 0, 0x9207a4, 0, 0, 0, 0, 0, 0, WZ_400},
    { 5, 0, 0, 0, 0x9207d4, 0, 0, 0, 0, 0, 0, WZ_400},
    { 6, 0, 0, 0, 0x920804, 0, 0, 0, 0, 0, 0, WZ_400},
    { 7, 0, 0, 0, 0x920834, 0, 0, 0, 0, 0, 0, WZ_400},
    { 8, 0, 0, 0, 0x920864, 0, 0, 0, 0, 0, 0, WZ_400},
    { 9, 0, 0, 0, 0x920894, 0, 0, 0, 0, 0, 0, WZ_400},
    { 10, 0, 0, 0, 0x9208c4, 0, 0, 0, 0, 0, 0, WZ_400},

    // Warzone 2100 4.0.1
    {0, 0, 0, 0, 0x9206e4, 0, 0, 0, 0, 0, 0, WZ_401},
    {1, 0, 0, 0, 0x920714, 0, 0, 0, 0, 0, 0, WZ_401},
    {2, 0, 0, 0, 0x920744, 0, 0, 0, 0, 0, 0, WZ_401},
    {3, 0, 0, 0, 0x920774, 0, 0, 0, 0, 0, 0, WZ_401},
    {4, 0, 0, 0, 0x9207a4, 0, 0, 0, 0, 0, 0, WZ_401},
    {5, 0, 0, 0, 0x9207d4, 0, 0, 0, 0, 0, 0, WZ_401},
    {6, 0, 0, 0, 0x920804, 0, 0, 0, 0, 0, 0, WZ_401},
    {7, 0, 0, 0, 0x920834, 0, 0, 0, 0, 0, 0, WZ_401},
    {8, 0, 0, 0, 0x920864, 0, 0, 0, 0, 0, 0, WZ_401},
    {9, 0, 0, 0, 0x920894, 0, 0, 0, 0, 0, 0, WZ_401},
    {10, 0, 0, 0, 0x9208c4, 0, 0, 0, 0, 0, 0, WZ_401},

    // Warzone 2100 4.1.0
    {0, 0, 0, 0, 0x9610e4, 0, 0, 0, 0, 0, 0, WZ_410},
    {1, 0, 0, 0, 0x961114, 0, 0, 0, 0, 0, 0, WZ_410},
    {2, 0, 0, 0, 0x961144, 0, 0, 0, 0, 0, 0, WZ_410},
    {3, 0, 0, 0, 0x961174, 0, 0, 0, 0, 0, 0, WZ_410},
    {4, 0, 0, 0, 0x9611a4, 0, 0, 0, 0, 0, 0, WZ_410},
    {5, 0, 0, 0, 0x9611d4, 0, 0, 0, 0, 0, 0, WZ_410},
    {6, 0, 0, 0, 0x961204, 0, 0, 0, 0, 0, 0, WZ_410},
    {7, 0, 0, 0, 0x961234, 0, 0, 0, 0, 0, 0, WZ_410},
    {8, 0, 0, 0, 0x961264, 0, 0, 0, 0, 0, 0, WZ_410},
    {9, 0, 0, 0, 0x961294, 0, 0, 0, 0, 0, 0, WZ_410},
    {10, 0, 0, 0, 0x9612c4, 0, 0, 0, 0, 0, 0, WZ_410},

    // Warzone 2100 4.2.7
    {0, 0x906bf1, 0, 0, 0xd20804, 0, 0, 0, 0, 0, 0xd20800, WZ_427},
    {1, 0x906bf1, 0, 0, 0xd20834, 0, 0, 0, 0, 0, 0xd20800, WZ_427},
    {2, 0x906bf1, 0, 0, 0xd20864, 0, 0, 0, 0, 0, 0xd20800, WZ_427},
    {3, 0x906bf1, 0, 0, 0xd20894, 0, 0, 0, 0, 0, 0xd20800, WZ_427},
    {4, 0x906bf1, 0, 0, 0xd208c4, 0, 0, 0, 0, 0, 0xd20800, WZ_427},
    {5, 0x906bf1, 0, 0, 0xd208f4, 0, 0, 0, 0, 0, 0xd20800, WZ_427},
    {6, 0x906bf1, 0, 0, 0xd20924, 0, 0, 0, 0, 0, 0xd20800, WZ_427},
    {7, 0x906bf1, 0, 0, 0xd20954, 0, 0, 0, 0, 0, 0xd20800, WZ_427},
    {8, 0x906bf1, 0, 0, 0xd20984, 0, 0, 0, 0, 0, 0xd20800, WZ_427},
    {9, 0x906bf1, 0, 0, 0xd209b4, 0, 0, 0, 0, 0, 0xd20800, WZ_427},
    {10, 0x906bf1, 0, 0, 0xd209e4, 0, 0, 0, 0, 0, 0xd20800, WZ_427},
};

/**
 * @brief WzHack_Initialize Inicializa a API
 * @param process_name Nome do processo
 * @return true se não der erro
 */
struct libhack_handle *WzHack_Initialize(const char *process_name);

/**
 * @brief WzHack_Cleanup Libera os recursos utilizados pela API
 * @return
 */
void WzHack_Cleanup();

/**
 * @brief WzHack_GetWarzoneVersion Obtém a versão do warzone em questão
 * @param wz_filename Caminho completo do executável do warzone2100
 * @return Versão do jogo
 */
unsigned short WzHack_GetWarzoneVersion(const char *wz_filename);

/**
 * @brief WzHack_GetStructStartIndex Obtém a posição de início na estrutura que armazena os offsets, de acordo com a versão especificada
 * @param wz_version Versão do warzone 2100
 * @param start Endereço da variável que irá armazenar a posição de início na estrutura
 * @return FALSE em caso de erro
 */
BOOL WzHack_GetStructStartIndex(int wz_version, unsigned short *start);

/**
 * @brief WzHack_FindProcess Procura pelo processo especificado na memória RAM
 * @param name Nome do processo a ser procurado
 * @param pid Endereço da variável que armazena o PID do processo
 * @return TRUE se o processo for localizado
 */
BOOL WzHack_FindProcess(const char *name, DWORD *pid);

/**
 * @brief WzHack_GetPlayerNumberOfUnits Obtém o número de unidades do jogador
 * @param player id do jogador
 * @param wz_version versão do warzone
 * @param number_of_units ponteiro para o número de unidades lido
 * @return FALSE em caso de erro
 */
BOOL WzHack_GetPlayerNumberOfUnits(unsigned player, int wz_version, DWORD *number_of_units);

/**
 * @brief WzHack_GetNumberOfBuiltStructures obtém o número de estruturas construídas pelo jogador
 * @param player id do jogador
 * @param wz_version versão do warzone
 * @param number_of_built_structures ponteiro para o número de estruturas construídas
 * @return
 */
BOOL WzHack_GetNumberOfBuiltStructures(unsigned player, int wz_version, DWORD *number_of_built_structures);

/**
 * @brief WzHack_SetPlayerMaxPowerStorage Muda a capacidade máxima de armazenamento do jogador
 * @param player id do jogador
 * @param wz_version versão do warzone
 * @param storage capacidade de energia
 * @return FALSE em caso de erro
 */
BOOL WzHack_SetPlayerMaxPowerStorage(unsigned player, int wz_version, int storage);

/**
 * @brief WzHack_SetPlayerExtractedPower muda a quantidade de energia extraída
 * @param player id do jogador
 * @param wz_version versão do warzone
 * @param extracted energia extraída
 * @return FALSE em caso de erro
 */
BOOL WzHack_SetPlayerExtractedPower(unsigned player, int wz_version, int extracted);

/**
 * @brief WzHack_SetPlayerWastedPower muda a quantidade de energia consumida pelo jogador
 * @param player id do jogador
 * @param wz_version versão do warzone
 * @param wasted energia gasta
 * @return FALSE em caso de erro
 */
BOOL  WzHack_SetPlayerWastedPower(unsigned player, int wz_version, int wasted);

/**
 * @brief GetVersionFromWzVer Separa o número de versão do warzone 2100
 * @param wz_version Número de versão do warzone (compacto)
 * @param major Número de versão maior
 * @param minor Número de versão menor
 * @param patch Número do patch
 */
void GetVersionFromWzVer(int wz_version, int *major, int *minor, int *patch);

/**
 * @brief GetWarzoneMaxPower Obtém o valor da energia máxima, de acordo com a versão
 * @param wz_version
 * @return
 */
uint32_t GetWarzoneMaxPower(int wz_version);

/**
 * @brief GetWarzoneMaxPowerValue Obtém o valor padrão máximo de energia do respectivo jogador
 * @param wz_version Versão do warzone
 * @return Valor máximo da energia
 */
uint32_t GetWarzoneMaxPowerValue(int wz_version);

/**
 * @brief IsFeatureSupported Verifica se a funcionalidade está disponível para o jogo em questão
 * @param wz_version Versão do warzone
 * @param feature Funcionalidade a ser verificada
 * @return true se a funcionalidade estiver disponível
 */
bool IsFeatureSupported(int wz_version, FEATURES feature);

#endif // HACK_H

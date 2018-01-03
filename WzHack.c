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

/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <ShlObj.h>
#include <shlwapi.h>
#include <assert.h>

/* Defines */
#define OPTION_ENERGY		"-e:"			// Energia desejada
#define OPTION_DELAY		"-d:"			// Delay em ms
#define OPTION_PLAYER		"-p:"			// Seu player id
#define OPTION_EASTER_EGG	"--easter-egg"	// Easter egg
#define MAX_PLAYERS			11

#define WZ_239		0xA1
#define WZ_315		0xA2
#define WZ_323		0xA3

#define GREEN	10
#define BLUE	9
#define RED		12
#define YELLOW	14
#define MAGENTA	13
#define WHITE	15
#define CYAN	11

/* Enumerações */
typedef enum message_types
{
	WARNING,
	CRITICAL,
	INFO,
	DEFAULT,
	SUCCESS
} types;

/* Funções pré-definidas */
typedef BOOL (WINAPI *QFPINA)(HANDLE hProcess, DWORD flag, LPSTR path, PDWORD bufSize);

#ifdef _WINDLL
#define WZHACK_API	__declspec(dllexport)
#else
#define WZHACK_API
#endif

BOOL WZHACK_API FindProcess(const char *nome, DWORD *pid);
DWORD WZHACK_API GetModuleAddress(const char *nome_executavel, const char *nome_modulo);
char WZHACK_API *GetSubstring(char *str, char k);
BOOL WZHACK_API GetPlayerPower(unsigned player_id, HANDLE warzoneHandle, DWORD *power, int wz_version);
BOOL WZHACK_API SetPlayerPower(unsigned player_id, HANDLE warzoneHandle, DWORD power, int wz_version);
BOOL WZHACK_API GetWzPpoStartIndex(int major, int minor, int patch, int *start);
void WZHACK_API ShowUsage(char **argv);
int WZHACK_API ShowMessage(types t, const char *string, ...);
void WZHACK_API RunEasterEgg(HANDLE warzoneHandle, int wz_version, unsigned my_id);

/* Estruturas */
typedef struct warzone_base_address
{
	DWORD base;	// Endereço base do executável principal
} WARZONE_BASE;

typedef struct warzone_player_power_offsets
{
	unsigned id;	// Id do jogador
	DWORD offset;	// Deslocamento em Hexadecimal
	DWORD major;	// Número de versão principal do warzone
	DWORD minor;	// Número de versão menor do warzone
	DWORD patch;	// Número de versão do patch do warzone
} WARZONE_PPO;

WARZONE_PPO wz_ppo[] = 
{
	// Warzone 2100 2.3.9
	{ 0, 0x0d795f0, 2, 3, 9},
	{ 1, 0x0d795f8, 2, 3, 9},
	{ 2, 0x0d79600, 2, 3, 9},
	{ 3, 0x0d79608, 2, 3, 9},
	{ 4, 0x0d79610, 2, 3, 9},
	{ 5, 0x0d79618, 2, 3, 9},
	{ 6, 0x0d79620, 2, 3, 9},
	{ 7, 0x0d79628, 2, 3, 9},
	{ 8, 0x0d79630, 2, 3, 9},
	{ 9, 0x0d79638, 2, 3, 9},
	{ 10, 0x0d79650, 2, 3, 9},
	
	// Warzone 2100 3.1.5
	{ 0, 0x120bc24, 3, 1, 5},
	{ 1, 0x120bc3c, 3, 1, 5},
	{ 2, 0x120bc54, 3, 1, 5},
	{ 3, 0x120bc6c, 3, 1, 5},
	{ 4, 0x120bc84, 3, 1, 5},
	{ 5, 0x120bc9c, 3, 1, 5},
	{ 6, 0x120bcb4, 3, 1, 5},
	{ 7, 0x120bccc, 3, 1, 5},
	{ 8, 0x120bce4, 3, 1, 5},
	{ 9, 0x120bcfc, 3, 1, 5},
	{ 10, 0x120bd14, 3, 1, 5},

	// Warzone 2100 3.2.3
	{ 0, 0x1b6fa44, 3, 2, 3},
	{ 1, 0x1b6fa64, 3, 2, 3},
	{ 2, 0x1b6fa84, 3, 2, 3},
	{ 3, 0x1b6faa4, 3, 2, 3},
	{ 4, 0x1b6fac4, 3, 2, 3},
	{ 5, 0x1b6fae4, 3, 2, 3},
	{ 6, 0x1b6fb04, 3, 2, 3},
	{ 7, 0x1b6fb24, 3, 2, 3},
	{ 8, 0x1b6fb44, 3, 2, 3},
	{ 9, 0x1b6fb64, 3, 2, 3},
	{ 10, 0x1b6fb84, 3, 2, 3}
};

/* Início do código */
BOOL GetWzPpoStartIndex(int major, int minor, int patch, int *start)
{
	int local_start = -1;
	BOOL bOk = FALSE;

	for (int i = 0; i < (sizeof(wz_ppo) / sizeof(wz_ppo[0])); i++)
	{
		if (wz_ppo[i].major == major
			&& wz_ppo[i].minor == minor
			&& wz_ppo[i].patch == patch)
		{
			local_start = i;
			bOk = TRUE;
			break;
		}
	}

	*start = local_start;

	return bOk;
}

BOOL FindProcess(const char *nome, DWORD *pid)
{
	PROCESSENTRY32 *entradas = NULL;
	HANDLE snapHandle = NULL;
	DWORD local_pid = -1;

	entradas = (PROCESSENTRY32*)malloc(sizeof(PROCESSENTRY32));
	if (entradas == NULL)
	{
		ShowMessage(CRITICAL, "Failed to allocate memory.\n");
		*pid = local_pid;
		return FALSE;
	}

	entradas->dwSize = sizeof(PROCESSENTRY32);
	
	snapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapHandle == NULL)
	{
		ShowMessage(CRITICAL, "Failed to capture the running processes.\n");
		*pid = local_pid;
		
		if(entradas)
			free(entradas);
		
		return FALSE;
	}

	if (!Process32First(snapHandle, entradas))
	{
		ShowMessage(CRITICAL, "Failed to intialize scan of processes in memory. Error %lu\n", GetLastError());
		*pid = local_pid;
		
		if(entradas)
			free(entradas);
		
		return FALSE;
	}

	do
	{
		const char *nome_processo_alvo = nome;
		const char *nome_processo_atual = entradas->szExeFile;

		if (strncmp(nome_processo_alvo, nome_processo_atual, strlen(nome_processo_alvo)) == 0)
		{
			*pid = entradas->th32ProcessID;
			return TRUE;
		}

	} while (Process32Next(snapHandle, entradas));

	if(entradas)
		free(entradas);
	
	*pid = local_pid;

	return FALSE;
}

DWORD GetModuleAddress(const char *nome_executavel, const char *nome_modulo)
{
	PROCESSENTRY32 *entradas = NULL;
	HANDLE snapHandle = NULL;
	DWORD local_pid = -1;
	BOOL encontrou = TRUE;
	BOOL falhou = TRUE;
	DWORD endereco_base = -1;

	// Primeiro verificamos se o executável está carregado na RAM
	encontrou = FindProcess(nome_executavel, &local_pid);
	if (!encontrou)
	{
		// Não podemos prosseguir. Não temos o pid do processo
#ifdef _DEBUG
		printf("Failed to find %s. Error %lu\n", nome_executavel, GetLastError());
#endif
		return FALSE;
	}

	// Agora tiramos um snapshot de todos os modulos carregados pelo executável em questão...
	MODULEENTRY32 *modulo = NULL;
	HANDLE msnapHandle = NULL;

	modulo = (MODULEENTRY32*)malloc(sizeof(MODULEENTRY32));
	if (modulo == NULL)
	{
		ShowMessage(CRITICAL, "Failed to allocate memory. Error %lu\n", GetLastError());
		return FALSE;
	}

	msnapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, local_pid);

	if (msnapHandle == NULL)
	{
		printf("Falha ao capturar os modulos carregados por %s (PID: %lu)\n", nome_executavel, local_pid);
		return FALSE;
	}
	
	if (!Module32First(msnapHandle, modulo))
	{
		printf("Falha ao inicializar varredura dos modulos na memória do %s (PID %lu). Erro %lu\n", nome_executavel, local_pid,
			GetLastError());
		return FALSE;
	}

	do
	{
		const char *nome_processo_alvo = nome_modulo;
		const char *nome_processo_atual = modulo->szModule;

#ifdef _DEBUG
		printf("Procurando %s em %s. Encontrado: %s\n", nome_modulo, nome_executavel, nome_processo_atual);
#endif

		if (strncmp(nome_processo_alvo, nome_processo_atual, strlen(nome_processo_alvo)) == 0)
		{
			endereco_base = (DWORD)modulo->modBaseAddr;
			falhou = FALSE;
			break;
		}

	} while (Module32Next(msnapHandle, modulo));

#ifdef _DEBUG
	printf("Falhou? -> %s\n", (falhou) ? "sim" : "nao");
	printf("Endereco base: %#x\n", endereco_base);
#endif

	if (modulo)
		free(modulo);

	if (msnapHandle)
		CloseHandle(msnapHandle);

	return endereco_base;
}

char *GetSubstring(char *str, char k)
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

BOOL GetPlayerPower(unsigned player_id, HANDLE warzoneHandle, DWORD *power, int wz_version)
{
	BOOL bResult = FALSE;
	DWORD nread;
	WARZONE_BASE wz;
	DWORD local_power;
	DWORD local_offset = 0;
	int start_index;
	BOOL bOk;

	assert(warzoneHandle != NULL);

	if(power == NULL)
		return FALSE;
	
	wz.base = GetModuleAddress("warzone2100.exe", "warzone2100.exe");
	if (wz.base == -1)
	{
		if(warzoneHandle)
			CloseHandle(warzoneHandle);
			
		*power = -1;
		return 1;
	}	

	if (wz_version == WZ_239)
	{
		bOk = GetWzPpoStartIndex(2, 3, 9, &start_index);
	}
	else if (wz_version == WZ_315)
	{
		bOk = GetWzPpoStartIndex(3, 1, 5, &start_index);
	}
	else if (wz_version == WZ_323)
	{
		bOk = GetWzPpoStartIndex(3, 2, 3, &start_index);
	}
	else
	{
		ShowMessage(CRITICAL, "Unsupported warzone version detected '%d'. The program cannot continue\n", wz_version);
		exit(EXIT_FAILURE);
	}

	if (bOk)
	{
		if (player_id < 0 || player_id > 10)
		{
			ShowMessage(CRITICAL, "Bad player id: %d\n", player_id);
		}
		else
		{
			local_offset = wz_ppo[start_index + player_id].offset;
			bResult = ReadProcessMemory(warzoneHandle, (void*)(wz.base + local_offset), &local_power, sizeof(DWORD), &nread);
#ifdef _DEBUG
			ShowMessage(DEFAULT, "Energy of player %u -> %u\n", player_id, local_power);
			ShowMessage(DEFAULT, "Offset: %#x\n", wz_ppo[start_index + player_id].offset);
			ShowMessage(DEFAULT, "Position on vector: %d\n", start_index + player_id);
			ShowMessage(DEFAULT, "Address: %#x\n", wz.base + local_offset);
#endif
		}
	}
	else
	{
		ShowMessage(CRITICAL, "BUG: Internal program error. Source %s line %d\n", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}

	if (bResult)
		*power = local_power;
	else
		*power = -1;

	return bResult;	
}

BOOL SetPlayerPower(unsigned player_id, HANDLE warzoneHandle, DWORD power, int wz_version)
{
	BOOL bResult = FALSE;
	DWORD nwrite;
	WARZONE_BASE wz;
	DWORD local_offset = 0;
	int start_index;
	BOOL bOk;

	assert(warzoneHandle != NULL);

	wz.base = GetModuleAddress("warzone2100.exe", "warzone2100.exe");
	if (wz.base == -1)
	{
		if(warzoneHandle)
			CloseHandle(warzoneHandle);
			
		return 1;
	}	
	
	switch(wz_version)
	{
		case WZ_239:
			bOk = GetWzPpoStartIndex(2, 3, 9, &start_index);
		break;
		
		case WZ_315:
			bOk = GetWzPpoStartIndex(3, 1, 5, &start_index);
		break;
		
		case WZ_323:
			bOk = GetWzPpoStartIndex(3, 2, 3, &start_index);
		break;
	}

	if (bOk)
	{
		if (player_id < 0 || player_id > 10)
		{
			ShowMessage(CRITICAL, "Bad player id: %d\n", player_id);
		}
		else
		{
#ifdef _DEBUG
			ShowMessage(DEFAULT, "Changing energy of player %u to %u\n", player_id, power);
			ShowMessage(DEFAULT, "Offset: %#x\n", wz_ppo[start_index + player_id].offset);
			ShowMessage(DEFAULT, "Position on vector: %d\n", start_index + player_id);
			ShowMessage(DEFAULT, "Address: %#x\n", wz.base + local_offset);
#endif
			local_offset = wz_ppo[start_index + player_id].offset;
			bResult = WriteProcessMemory(warzoneHandle, (void*)(wz.base + local_offset), (const void*)&power, sizeof(DWORD), &nwrite);
		}
	}
	else
	{
		ShowMessage(CRITICAL, "BUG: Internal program error. Source %s line %d\n", __FILE__, __LINE__);
		exit(EXIT_FAILURE);
	}

	return bResult;
}

void ShowUsage(char **arg)
{
	printf("\n\nUsage: %s %s<energy> %s<delay ms> %s<your player id> %s\n\n", 
		arg[0],
		OPTION_ENERGY,
		OPTION_DELAY,
		OPTION_PLAYER,
		OPTION_EASTER_EGG);
}

int ShowMessage(types t, const char * string, ...)
{
	HANDLE hConsole = NULL;
	va_list list;
	char mem_buffer[256];
	int len = 0;
	BOOL colors_enabled = TRUE;

	va_start(list, string);
	len = vsnprintf(mem_buffer, sizeof(char) * 256, string, list);
	va_end(list);

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!hConsole)
		colors_enabled = FALSE;

	if (!colors_enabled)
	{
		len = fprintf(stdout, mem_buffer);
	}
	else 
	{
		// Pegar a cor anterior do console
		CONSOLE_SCREEN_BUFFER_INFO *csbi = NULL;
		csbi = (CONSOLE_SCREEN_BUFFER_INFO*)malloc(sizeof(CONSOLE_SCREEN_BUFFER_INFO));
		if (csbi == NULL)
		{
			len = fprintf(stdout, mem_buffer);
		}
		else 
		{
			if (!GetConsoleScreenBufferInfo(hConsole, csbi))
			{
				len = len = fprintf(stdout, mem_buffer);
				if (csbi)
					free(csbi);
			}
			else 
			{
				if (t == WARNING)
				{
					SetConsoleTextAttribute(hConsole, (WORD)YELLOW);
					fprintf(stdout, "[!] ");
					FlushConsoleInputBuffer(hConsole);
					SetConsoleTextAttribute(hConsole, csbi->wAttributes);
					len = fprintf(stdout, mem_buffer);
				}
				else if (t == SUCCESS)
				{
					SetConsoleTextAttribute(hConsole, (WORD)GREEN);
					fprintf(stdout, "[+] ");
					FlushConsoleInputBuffer(hConsole);
					SetConsoleTextAttribute(hConsole, csbi->wAttributes);
					len = fprintf(stdout, mem_buffer);
				}
				else if(t == CRITICAL)
				{
					SetConsoleTextAttribute(hConsole, (WORD)RED);
					fprintf(stdout, "[!] ");
					FlushConsoleInputBuffer(hConsole);
					SetConsoleTextAttribute(hConsole, csbi->wAttributes);
					len = fprintf(stdout, mem_buffer);
				}
				else if(t == INFO)
				{
					SetConsoleTextAttribute(hConsole, (WORD)CYAN);
					fprintf(stdout, "[*] ");
					FlushConsoleInputBuffer(hConsole);
					SetConsoleTextAttribute(hConsole, csbi->wAttributes);
					len = fprintf(stdout, mem_buffer);
				}
				else if(t == DEFAULT)
				{
					SetConsoleTextAttribute(hConsole, (WORD)WHITE);
					fprintf(stdout, "[*] ");
					FlushConsoleInputBuffer(hConsole);
					SetConsoleTextAttribute(hConsole, csbi->wAttributes);
					len = fprintf(stdout, mem_buffer);
				} else 
				{
					printf("Fatal error on %s line %d: invalid param received.\n", __FILE__, __LINE__);
				}
			}
		}
	}

	return len;
}

/* Este código não foi feito pra ser facilmente compreensível kkkk */

void RunEasterEgg(HANDLE w, int a, unsigned me)
{
	BOOL b;
	int s;
	DWORD o;
	DWORD p;
	int v;
	DWORD npp = 0;

	assert(w != NULL);

	if (a == WZ_239)
	{
		b = GetWzPpoStartIndex(2, 3, 9, &s);
		v = WZ_239;
	}
	else if (a == WZ_315)
	{
		b = GetWzPpoStartIndex(3, 1, 5, &s);
		v = WZ_315;
	}
	else if (a == WZ_323)
	{
		b = GetWzPpoStartIndex(3, 2, 3, &s);
		v = WZ_323;
	}

	for (unsigned i = 0; i < MAX_PLAYERS; i++)
	{
		if (i == me)
			continue;

		o = wz_ppo[i + s].offset;
		if (GetPlayerPower(i, w, &p, v)) 
		{
			if (p > 0)
			{
				SetPlayerPower(i, w, npp, v);
			}
		}
	}
}

int main(int argc, char **argv)
{
	int energia_desejada = -1;
	int delay = -1;
	int id = -1;
	bool temos_energia = false;
	bool temos_delay = false;
	bool temos_id = false;
	bool ativar_easter_egg = false;
	HANDLE hMutex = NULL;

	hMutex = CreateMutexA(NULL, TRUE, "WarHack_Mutex");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		ShowMessage(CRITICAL, "Another instance of War Hack is already running\n");
		return 1;
	}

	if (hMutex == NULL)
	{
		ShowMessage(CRITICAL, "Failed to create mutex. Error %lu\n", GetLastError());
		return 1;
	}

	// Analisar os argumentos
	if (argc < 4)
	{
		ShowUsage(argv);
		return 1;
	}

	ShowMessage(INFO, "Welcome to WzHack v1.0 - By Lucas Vieira de Jesus\n");

	for (int x = 0; x < argc; x++)
	{
		if (strncmp(argv[x], OPTION_ENERGY, 3) == 0)
		{
			if (temos_energia)
			{
				ShowMessage(CRITICAL, "Duplicated parameter -> '%s'\n", OPTION_ENERGY);
				return 1;
			}
			
			char *sub = NULL;

			sub = GetSubstring(argv[x], ':');
			energia_desejada = atoi((char const*)sub);
			temos_energia = true;
		}
		else if (strncmp(argv[x], OPTION_DELAY, 3) == 0)
		{
			if (temos_delay)
			{
				ShowMessage(CRITICAL, "Duplicated parameter -> '%s'\n", OPTION_DELAY);
				return 1;
			}

			char *sub = NULL;

			sub = GetSubstring(argv[x], ':');
			delay = atoi((char const*)sub);
			temos_delay = true;
		}
		else if (strncmp(argv[x], OPTION_PLAYER, 3) == 0)
		{
			if (temos_id)
			{
				ShowMessage(CRITICAL, "Duplicated parameter -> '%s'\n", OPTION_PLAYER);
				return 1;
			}

			char *sub = NULL;

			sub = GetSubstring(argv[x], ':');
			id = atoi((char const*)sub);

			if(id >= 0 && id <= 10)
				temos_id = true;
		}
		else if (strncmp(argv[x], OPTION_EASTER_EGG, 12) == 0)
		{
			if (ativar_easter_egg)
				ShowMessage(CRITICAL, "Duplicated parameter -> '%s'. Ignoring ...\n", OPTION_EASTER_EGG);

			ativar_easter_egg = true;
		}
	}

	if(!temos_id || !temos_delay || !temos_energia || (delay <= 0) || (energia_desejada <= 0))
	{
		ShowMessage(CRITICAL, "Invalid parameters passed to command line.\n");
		return 1;
	}
	
#ifdef _DEBUG
	printf("[*] Energy %d\n- Delay: %d ms\n", energia_desejada, delay);
#endif

	DWORD pid;
	BOOL warzone = FALSE;
	HANDLE warzoneHandle = NULL;
	DWORD valor_energia;	
	
	ShowMessage(INFO, "Waiting for warzone 2100 ...\n");
	
	while (warzone == FALSE)
		warzone = FindProcess("warzone2100.exe", &pid);
	
	ShowMessage(INFO, "Warzone 2100 found.\n");
	
	warzoneHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (warzoneHandle == NULL)
	{
		if (GetLastError() == 5) {
			ShowMessage(INFO, "Run this program as admin.\n");
		}
		else {
			ShowMessage(CRITICAL, "Unknown error: %lu\n", GetLastError());
		}
		return 1;
	}

	// Determinar versao do warzone 
	int major = -1;
	int minor = -1;
	int patch = -1;
	char wz_path[MAX_PATH] = { 0 };
	char version[32];
	DWORD qfpi_dwSize = MAX_PATH;
	QFPINA fQueryFullProcessImageNameA = NULL;
	
	HMODULE kernel32 = NULL;

	kernel32 = LoadLibraryA("kernel32.dll");
	if (kernel32 == NULL)
	{
		ShowMessage(CRITICAL, "Failed to load kernel32.dll\n");
		exit(1);
	}
	
	fQueryFullProcessImageNameA = (QFPINA)GetProcAddress(kernel32, "QueryFullProcessImageNameA");
	if (fQueryFullProcessImageNameA == NULL)
	{
		ShowMessage(CRITICAL, "Failed to obtain function address. Error %lu\n", GetLastError());
		exit(1);
	}

	ShowMessage(INFO, "Checking version ...\n");
	if (fQueryFullProcessImageNameA(warzoneHandle, 0, wz_path, &qfpi_dwSize))
	{
		ShowMessage(SUCCESS, "Warzone 2100 path: %s\n", wz_path);
		
		// Analisar string e encontrar o numero da versão
		int i = 0;
		int limite = strlen(wz_path);
		BOOL versionFound = FALSE;

		while (wz_path[i])
		{
			if ((i + 4) < limite)
			{
				if (isdigit(wz_path[i]) && (wz_path[i + 1] == '.') 
					&& isdigit(wz_path[i + 2]) && (wz_path[i + 3] == '.') 
					&& isdigit(wz_path[i + 4]))
				{
					major = wz_path[i];
					minor = wz_path[i + 2];
					patch = wz_path[i + 4];
					
					snprintf(version, sizeof(char) * 32, "%c", major);
					major = strtol(version,NULL,10);
					
					snprintf(version, sizeof(char) * 32, "%c", minor);
					minor = strtol(version,NULL,10);
					
					snprintf(version, sizeof(char) * 32, "%c", patch);
					patch = strtol(version,NULL,10);

					snprintf(version, sizeof(char) * 32, "%d.%d.%d", major,minor,patch);
					versionFound = TRUE;
					break;
				}
			}

			i++;
		}

		if (versionFound) 
		{
			ShowMessage(SUCCESS, "Warzone 2100 version %s\n", version);
		}
		else 
		{
			ShowMessage(CRITICAL, "Failed to determine warzone version.\n");
			exit(1);
		}
	}
	else
	{
		ShowMessage(CRITICAL, "Failed to obtain warzone 2100 path. Error %lu\n", GetLastError());
	}
	
	if (kernel32)
		FreeLibrary(kernel32);

	int version_flag;
	
	if(major == 3 && minor == 2 && patch == 3)
		version_flag = WZ_323;
	else if(major == 2 && minor == 3 && patch == 9)
		version_flag = WZ_239;
	else if(major == 3 && minor == 1 && patch == 5)
		version_flag = WZ_315;
	else {
		ShowMessage(CRITICAL, "[!] Not supported version.\n");
		exit(1);
	}
	
	// Loop principal do programa
	int errors = 0;
	unsigned uid = (unsigned)id;

	for(;;)
	{
		if (GetPlayerPower(uid, warzoneHandle, &valor_energia, version_flag))
		{
#ifdef _DEBUG
			ShowMessage(INFO, "Player %d energy: %lu\n", uid, valor_energia);
			ShowMessage(INFO, "Desired energy: %lu\n", energia_desejada);
#endif
			if (valor_energia < (DWORD)energia_desejada)
			{
				ShowMessage(INFO, "Energy lower than desired. Changing energy ...\n");

				BOOL bResult = SetPlayerPower(uid, warzoneHandle, energia_desejada, version_flag);
				if (bResult == FALSE)
				{
					ShowMessage(CRITICAL, "Error while changing player energy. Code: %lu\n", GetLastError());
				}
				else
				{
					ShowMessage(SUCCESS, "Energy change ok\n");
				}
			}
		}
		else
		{
			ShowMessage(CRITICAL, "Failed to get player %d energy. Error %lu\n", 0, GetLastError());
			errors++;
			Sleep(2000);
		}
		
		if (errors == 10)
			break;

		if (ativar_easter_egg)
		{
			RunEasterEgg(warzoneHandle, version_flag, uid);
			Sleep(3000);
		}
		else 
		{
			Sleep(delay);
		}
	}
	
	return 0;
}


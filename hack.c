/*
	Hack de energia para o warzone 2100

	Versões suportadas;
	2.3.9
	3.1.5
	3.2.3
	
	Autor: Lucas Vieira de Jesus <lucas.engen.cc@gmail.com>
	
	Compile no Visual Studio com a opção MultiByte habilitada
*/

#define WZ_239		0xA1
#define WZ_315		0xA2
#define WZ_323		0xA3

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>
#include <TlHelp32.h>
#include <Shlobj.h>
#include <shlwapi.h>
#include <assert.h>

typedef BOOL (WINAPI *QFPINA)(HANDLE hProcess, DWORD flag, LPSTR path, PDWORD bufSize);

#define OPCAO_ENERGIA	"-e:"
#define OPCAO_DELAY		"-d:"

const DWORD desloc_wz239 = 0xD795F0;
const DWORD desloc_wz315 = 0x120BC24;
const DWORD desloc_wz323 = 0x1B6FA44;

struct warzone_game
{
	DWORD base;
};

BOOL EncontrarProcesso(const char *nome, DWORD *pid)
{
	PROCESSENTRY32 *entradas = NULL;
	HANDLE snapHandle = NULL;
	DWORD local_pid = -1;

	entradas = (PROCESSENTRY32*)malloc(sizeof(PROCESSENTRY32));
	if (entradas == NULL)
	{
		printf("Falha ao alocar memória.\n");
		*pid = local_pid;
		return FALSE;
	}

	entradas->dwSize = sizeof(PROCESSENTRY32);
	
	snapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snapHandle == NULL)
	{
		printf("Falha ao verificar quais processos estão executando.\n");
		*pid = local_pid;
		
		if(entradas)
			free(entradas);
		
		return FALSE;
	}

	if (!Process32First(snapHandle, entradas))
	{
		printf("Falha ao inicializar varredura dos processos na memória. Erro %lu\n", GetLastError());
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

DWORD PegarEnderecoBase(const char *nome_executavel, const char *nome_modulo)
{
	PROCESSENTRY32 *entradas = NULL;
	HANDLE snapHandle = NULL;
	DWORD local_pid = -1;
	BOOL encontrou = TRUE;
	BOOL falhou = TRUE;
	DWORD endereco_base = -1;

	// Primeiro verificamos se o executável está carregado na RAM
	encontrou = EncontrarProcesso(nome_executavel, &local_pid);
	if (!encontrou)
	{
		// Não podemos prosseguir. Não temos o pid do processo
#ifdef _DEBUG
		printf("Nao foi possivel achar %s. Erro %lu\n", nome_executavel, GetLastError());
#endif
		return FALSE;
	}

	// Agora tiramos um snapshot de todos os modulos carregados pelo executável em questão...
	MODULEENTRY32 *modulo = NULL;
	HANDLE msnapHandle = NULL;

	modulo = (MODULEENTRY32*)malloc(sizeof(MODULEENTRY32));
	if (modulo == NULL)
	{
		printf("Falha ao alocar memoria. Erro %lu\n", GetLastError());
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

char *substring(char *str, char k)
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

/*
	HANDLE warzoneHandle -> Indentificador para o processo do warzone
	DWORD power -> Ponteiro para armazenar o valor lido, em caso de sucesso
	int wz_version -> WZ_323, WZ_239
*/

BOOL GetPlayerPower(HANDLE warzoneHandle, DWORD *power, int wz_version)
{
	BOOL bResult = FALSE;
	DWORD nread;
	struct warzone_game wz;
	DWORD local_power;

	assert(warzoneHandle != NULL);

	if(power == NULL)
		return FALSE;
	
	wz.base = PegarEnderecoBase("warzone2100.exe", "warzone2100.exe");
	if (wz.base == -1)
	{
		if(warzoneHandle)
			CloseHandle(warzoneHandle);
			
		*power = -1;
		return 1;
	}	

	
	switch(wz_version)
	{
		case WZ_239:
			bResult = ReadProcessMemory(warzoneHandle, (void*)(wz.base + desloc_wz239), &local_power, sizeof(DWORD), &nread);		
		break;
		
		case WZ_315:
			bResult = ReadProcessMemory(warzoneHandle, (void*)(wz.base + desloc_wz315), &local_power, sizeof(DWORD), &nread);
		break;
		
		case WZ_323:
			bResult = ReadProcessMemory(warzoneHandle, (void*)(wz.base + desloc_wz323), &local_power, sizeof(DWORD), &nread);
		break;
	}
	
	if (bResult)
		*power = local_power;
	else
		*power = -1;

	return bResult;	
}

BOOL SetPlayerPower(HANDLE warzoneHandle, DWORD power, int wz_version)
{
	BOOL bResult = FALSE;
	DWORD nwrite;
	struct warzone_game wz;

	assert(warzoneHandle != NULL);

	wz.base = PegarEnderecoBase("warzone2100.exe", "warzone2100.exe");
	if (wz.base == -1)
	{
		if(warzoneHandle)
			CloseHandle(warzoneHandle);
			
		return 1;
	}	
	
	switch(wz_version)
	{
		case WZ_239:
			bResult = WriteProcessMemory(warzoneHandle, (void*)(wz.base + desloc_wz239), (const void*)&power, sizeof(DWORD), &nwrite);
		break;
		
		case WZ_315:
			bResult = WriteProcessMemory(warzoneHandle, (void*)(wz.base + desloc_wz315), (const void*)&power, sizeof(DWORD), &nwrite);
		break;
		
		case WZ_323:
			bResult = WriteProcessMemory(warzoneHandle, (void*)(wz.base + desloc_wz323), (const void*)&power, sizeof(DWORD), &nwrite);
		break;
	}

	return bResult;
}

void uso(char **arg)
{
	printf("\n\nUso %s %s<energia> %s<delay_milissegundos>\n\n", 
		arg[0],
		OPCAO_ENERGIA,
		OPCAO_DELAY);
}

int main(int argc, char **argv)
{
	int energia_desejada = -1;
	int delay = -1;
	bool temos_energia = false;
	bool temos_delay = false;
	
	// Analisar os argumentos
	if (argc != 3)
	{
		uso(argv);
		return 1;
	}

	for (int x = 0; x < argc; x++)
	{
#ifdef _DEBUG
		printf( "cmdline #%d -> %s\n", x, argv[x]);
#endif
		if (strncmp(argv[x], OPCAO_ENERGIA, 3) == 0)
		{
#ifdef _DEBUG
			printf("- Parametro energia encontrado.\n");
#endif
			if (temos_energia)
			{
				printf("Erro: parametro duplicado -> '%s'\n", OPCAO_ENERGIA);
				return 1;
			}
			
			char *sub = NULL;

			sub = substring(argv[x], ':');
			energia_desejada = atoi((char const*)sub);
			temos_energia = true;
		}
		else {
			if (strncmp(argv[x], OPCAO_DELAY, 3) == 0)
			{
#ifdef _DEBUG
				printf("- Parametro delay encontrado.\n");
#endif
				if (temos_delay)
				{
					printf("Erro: parametro duplicado -> '%s'\n", OPCAO_DELAY);
					return 1;
				}

				char *sub = NULL;

				sub = substring(argv[x], ':');
				delay = atoi((char const*)sub);
				temos_delay = true;
			}
		}
	}

	if(!temos_delay || !temos_energia || (delay <= 0) || (energia_desejada <= 0))
	{
		printf( "[!] Parametros invalidos passados na linha de comando.\n");
		return 1;
	}
	
#ifdef _DEBUG
	printf("[*] Energia %d\n- Delay: %d ms\n", energia_desejada, delay);
#endif

	DWORD pid;
	BOOL warzone = FALSE;
	HANDLE warzoneHandle = NULL;
	DWORD valor_energia;	
	
	printf( "[*] Procurando o warzone 2100 na memoria RAM ...\n");
	
	while (warzone == FALSE)
		warzone = EncontrarProcesso("warzone2100.exe", &pid);
	
	printf("[+] Warzone 2100 encontrado na RAM.\n");
	
	warzoneHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (warzoneHandle == NULL)
	{
		if (GetLastError() == 5) {
			printf("[*] Execute este programa como administrador.\n");
		}
		else {
			printf("[!] Um erro desconhecido ocorreu: %lu\n", GetLastError());
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
		printf("[!] Falha ao carregar kernel32.dll\n");
		exit(1);
	}

	fQueryFullProcessImageNameA = (QFPINA)GetProcAddress(kernel32, "QueryFullProcessImageNameA");
	if (fQueryFullProcessImageNameA == NULL)
	{
		printf("[!] Falha ao pegar endereço da função. %lu\n", GetLastError());
		exit(1);
	}

	printf("[+] Verificando versao ...\n");
	if (fQueryFullProcessImageNameA(warzoneHandle, 0, wz_path, &qfpi_dwSize))
	{
		printf("[*] Warzone 2100 path: %s\n", wz_path);
		
		// Analisar string e encontrar o numero da versão
		int i = 0;
		int limite = strlen(wz_path);
		BOOL versionFound = FALSE;

#ifdef _DEBUG
		printf("Limite: %d\n", limite);
#endif

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
			printf("[*] Versao: %s\n", version);
		}
		else 
		{
			printf("[!] Falha ao determinar a versao do warzone.\n");
			exit(1);
		}
	}
	else
	{
		printf("[!] Falha ao obter caminho do warzone 2100: %lu\n", GetLastError());
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
		printf( "[!] Versão não suportada.\n");
		exit(1);
	}
	
	// Loop
	int errors = 0;

#ifdef _DEBUG
	if(version_flag == WZ_323 || version_flag == WZ_239)
		printf("Version: %s\n", (version_flag == WZ_323) ? "WZ_323" : "WZ_239");
#endif

	for(;;)
	{
		if (GetPlayerPower(warzoneHandle, &valor_energia, version_flag))
		{
#ifdef _DEBUG
			printf("Energia do jogador: %lu\n", valor_energia);
			printf("Energia desejada: %lu\n", energia_desejada);
#endif
			if (valor_energia < energia_desejada)
			{
				printf("[*] Energia abaixo do desejado. Mudando energia ... ");
				BOOL bResult = SetPlayerPower(warzoneHandle, energia_desejada, version_flag);
				if (bResult == FALSE)
				{
					printf("[erro %lu]\n", GetLastError());
				}
				else
				{
					printf("[ok]\n");
				}
			}
		}
		else
		{
			printf("[!] Erro ao obter a energia do jogador: %lu\n", GetLastError());
			errors++;
		}
		
		if (errors == 10)
			break;

		Sleep(delay);
	}
	
	return 0;
}


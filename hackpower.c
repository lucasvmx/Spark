/*
	Hack de energia para o warzone 2100

	Versões suportadas;
	2.3.9
	3.2.3
	
	Autor: Lucas Vieira de Jesus <lucas.engen.cc@gmail.com>
	
	Compile no Visual Studio com a opção MultiByte habilitada
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>
#include <TlHelp32.h>

#define OPCAO_ENERGIA	"-e:"
#define OPCAO_DELAY		"-d:"

const DWORD desloc_wz239 = 0xD795F0;
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

void uso(char **arg)
{
	printf("\n\nUso %s %s<energia> %s<delay_milissegundos>\n\n", 
		arg[0],
		OPCAO_ENERGIA,
		OPCAO_DELAY);
}

int main(int argc, char **argv)
{
	int energia = -1;
	int delay = -1;
	bool temos_energia = false;
	bool temos_delay = false;
	bool read_ok = false;
	bool write_ok = false;
	
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
			energia = atoi((char const*)sub);
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

	if(!temos_delay || !temos_energia || (delay <= 0) || (energia <= 0))
	{
		printf( "[!] Parametros invalidos passados na linha de comando.\n");
		return 1;
	}
	
#ifdef _DEBUG
	printf("[*] Energia %d\n- Delay: %d ms\n", energia, delay);
#endif

	DWORD pid;
	BOOL warzone = FALSE;
	HANDLE warzoneHandle = NULL;
	DWORD nread;
	DWORD nwrite;

	// Variáveis para armazenar valores do jogo
	DWORD valor_energia;
	
	printf( "[*] Aguardando o warzone 2100 ser aberto ...\n");
	while (!warzone)
	{
		warzone = EncontrarProcesso("warzone2100.exe", &pid);
		Sleep(delay);
	}
	
	warzoneHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!warzoneHandle)
	{
		if (GetLastError() == 5) {
			printf("[*] Execute este programa como administrador.\n");
		}
		else {
			printf("[!] Um erro desconhecido ocorreu: %lu\n", GetLastError());
		}
		return 1;
	}

	// FIXME: Código para verificar qual é a versão do warzone ...
	struct warzone_game wz;
	wz.base = PegarEnderecoBase("warzone2100.exe", "warzone2100.exe");
	if (wz.base == -1)
	{
		if(warzoneHandle)
			CloseHandle(warzoneHandle);
		
		return 1;
	}
	
	// Pegar o valor atual da energia do jogador
	if (ReadProcessMemory(warzoneHandle, (const void*)(wz.base + desloc_wz323), &valor_energia, sizeof(DWORD), &nread))
	{
		if(valor_energia <= 0)
		{
			do 
			{
				if(!ReadProcessMemory(warzoneHandle, (const void*)(wz.base + desloc_wz323), &valor_energia, sizeof(DWORD), &nread))
				{
					// Tratar erro ...
					read_ok = false;
					break;
				}
				
				Sleep(delay);
				read_ok = true;
			} while(valor_energia <= 0);
			
			if(read_ok)
			{
				if(valor_energia < energia)
				{
					if(!WriteProcessMemory(warzoneHandle, (const void *)(wz.base + desloc_wz323), &energia, sizeof(int), &nwrite))
					{
						printf( "[!] Falha ao mudar energia do jogador. Erro %lu\n", GetLastError());
						write_ok = false;
					} else 
					{
						printf( "[+] Energia anterior: %lu\n", valor_energia);
						printf( "[+] Energia atual: %lu\n", energia);
						write_ok = true;
					}
				}
			} else 
			{
				printf( "[!] Erro fatal. Nao foi possivel ler o valor da energia do jogador.\n");
			}
		}
	}
	else
	{
		// TODO: Tratar erro ...
		if(warzoneHandle)
			CloseHandle(warzoneHandle);
	}
	
	return 0;
}


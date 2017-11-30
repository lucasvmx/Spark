/*
	Hack de energia para o warzone 2100

	Versões suportadas;
	2.3.9
	3.2.3
	
	Autor: Lucas Vieira de Jesus <lucas.engen.cc@gmail.com>
	
	Compile no Visual Studio com a opção MultiByte habilitada
*/

#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <TlHelp32.h>

struct warzone_game
{
	DWORD base;
	DWORD desloc_wz239 = 0xD795F0;
	DWORD desloc_wz323 = 0x1B6FA44;
}

BOOL EncontrarProcesso(const char *nome, DWORD *pid)
{
	PROCESSENTRY32 *entradas = NULL;
	HANDLE snapHandle = NULL;
	DWORD local_pid = -1;
	
	entradas = (PROCESSENTRY32*)malloc(sizeof(PROCESSENTRY32));
	if(entradas == NULL)
	{
		printf( "Falha ao alocar memória.\n");
		return FALSE;
	}
	
	snapHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(snapHandle == NULL)
	{
		printf( "Falha ao verificar quais processos estão rodando na máquina.\n");
		return FALSE;
	}
	
	if(!Process32First(snapHandle, entradas))
	{
		printf( "Falha ao inicializar varredura dos processos na memória.\n");
		return FALSE;		
	}
	
	do 
	{
		const char *nome_processo_alvo = "warzone2100.exe";
		const chawr *nome_processo_atual = entradas->szExeName;
		
		if(strncmp(nome_processo_alvo,nome_processo_atual)
	} while(Process32Next(snapHandle,entradas));
}

void uso(char **arg)
{
	printf("\n\nUso %s -e:<energia> -d:<delay_milissegundos>\n\n", arg[0]);
}

int main(int argc, char **argv)
{
	
}


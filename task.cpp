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

#include "task.h"
#include "wzhack.h"
#include "frmmain.h"
#include "frmsettings.h"
#include <windows.h>

typedef BOOL (WINAPI *QFPIN)(
  _In_    HANDLE hProcess,
  _In_    DWORD  dwFlags,
  _Out_   LPTSTR lpExeName,
  _Inout_ PDWORD lpdwSize
);

task::task()
{

}

void task::run()
{
    DWORD pid = -1;
    HANDLE hWarzone = INVALID_HANDLE_VALUE;
    int wz_version;
    QFPIN fQueryFullProcessImageNameA = NULL;
    HMODULE hKernel32 = NULL;
    char warzone_path[MAX_PATH];
    const char *wz_version_name = "";
    DWORD written = MAX_PATH;

    /* Precisamos encontrar o warzone 2100 na memória */

    emit update("Aguardando por warzone2100.exe ...<br>");

    do {
        if(this->isInterruptionRequested())
        {
            emit update("Tarefa interrompida<br>");
            return;
        }

        WzHack_FindProcess("warzone2100.exe",&pid);
    } while(pid == (DWORD)-1);

    emit update(QString::asprintf("Warzone 2100 encontrado. PID: %lu<br>",pid));

    hKernel32 = LoadLibraryA("kernel32.dll");
    if(!hKernel32)
    {
        emit update( "Falha ao carregar kernel32.dll<br>");
        return;
    }

    /* Precisamos saber aonde está o warzone2100 */

    fQueryFullProcessImageNameA = (QFPIN)GetProcAddress(hKernel32,"QueryFullProcessImageNameA");

    if(fQueryFullProcessImageNameA == NULL)
    {
        if(hKernel32)
            FreeLibrary(hKernel32);

        return;
    }

    hWarzone = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
    if(!hWarzone)
    {
        emit update("Falha ao hackear o warzone 2100<br>");
        return;
    }

    SecureZeroMemory(warzone_path,sizeof(warzone_path));

    if(!fQueryFullProcessImageNameA(hWarzone,0,(LPTSTR)warzone_path,&written))
    {
        emit update(QString::asprintf("Falha ao obter caminho do warzone 2100: %lu<br>", GetLastError()));

        if(hKernel32)
            FreeLibrary(hKernel32);

        return;
    }

    /* Já fizemos o que era necessário. Agora dá pra descarregar kernel32.dll da memória de programa */
    if(hKernel32)
        FreeLibrary(hKernel32);

    wz_version = WzHack_GetWarzoneVersion(warzone_path);
    if(wz_version == WZ_UNKNOWN)
    {
        emit update( "Versão não suportada<br>");
        return;
    }

    if(wz_version == WZ_239)
        wz_version_name = "Warzone 2100 2.3.9<br>";
    else if(wz_version == WZ_315)
        wz_version_name = "Warzone 2100 3.1.5<br>";
    else if(wz_version == WZ_323)
        wz_version_name = "Warzone 2100 3.2.3<br>";

    emit update(wz_version_name);
    emit update( "Hackeando ...<br>");

    if(globalZero)
    {
        while(true)
        {
            if(this->isInterruptionRequested())
            {
                emit update( "Tarefa interrompida<br>" );
                return;
            }

            if(WzHack_FindProcess("warzone2100.exe", NULL) == FALSE)
            {
                emit update( "O warzone 2100 foi fechado.");
                return;
            }

            WzHack_RunEasterEgg(hWarzone,wz_version,globalId);
            Sleep(globalDelay * 1000);
        }
    } else
    {
        if(globalSupport)
        {
            DWORD myPower;
            unsigned playerToFavor = globalId;

            /* Iremos favorecer um jogador específico */

            /* TODO: Aprimorar algoritmo de favorecimento de jogador */
            while(true)
            {
                if(this->isInterruptionRequested())
                {
                    emit update( "Tarefa interrompida<br>" );
                    return;
                }

                if(WzHack_FindProcess("warzone2100.exe", NULL) == FALSE)
                {
                    emit update( "O warzone 2100 foi fechado<br>");
                    return;
                }

                for(unsigned i = 0; i < MAX_PLAYERS; i++)
                {
                    if(i == playerToFavor)
                    {
                        BOOL havePower = WzHack_GetPlayerPower(i,hWarzone,&myPower,wz_version);
                        if(havePower)
                        {
                            if(myPower <= 0)
                            {
                                bool bShow = false;

                                if(!bShow)
                                    update( "Aguardando jogador começar a partida ...<br>");

                                bShow = true;
                            }

                            if(myPower < 60000 && myPower > 0)
                            {
                                update(QString::asprintf("A sua energia foi alterada para %lu<br>", myPower * 2));
                                WzHack_SetPlayerPower(i,hWarzone,myPower * 2,wz_version);
                            }
                        }
                    } else
                    {
                        WzHack_SetPlayerPower(i, hWarzone, 3000,wz_version);
                    }
                }

                /* O delay global é ignorado aqui, pois pode afetar a eficácia deste algoritmo */
                Sleep(2000);
            }
        } else {
            emit update( "Por favor, escolha uma das opções de hacking");
        }
    }
    emit update( "Tarefa terminada.<br>");
}

/*
    Hack para o warzone 2100

    Versões suportadas:
        * 2.3.9
        * 3.1.5
        * 3.2.3
        * 3.3.0
        * 3.4.0

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

    Plataformas de teste:
        * Microsoft Windows [versão 10.0.16299.125] x64
        * Microsoft Windows [versão 10.0.18363.900] x64
*/

#include "frmmain.h"
#include <QApplication>
#include <QMessageBox>
#include <windows.h>
#include "wzhack.h"

static void CheckForOtherInstance()
{
    HANDLE hMutex = nullptr;

    hMutex = CreateMutexA(nullptr, TRUE, "WarHack_Mutex");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        QMessageBox::critical(nullptr, "Error", "Another instance of War Hack is already running");
        exit(-1);
    }

    if (hMutex == nullptr)
    {
        QMessageBox::critical(nullptr, "Error", QString::asprintf("Failed to create mutex: %lu", GetLastError()));
        exit(0);
    }

    // Carrega as funcões da DLL
    LoadDLLFunctions();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    frmMain w;

    CheckForOtherInstance();
    w.show();

    return a.exec();
}

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

#include "frmmain.h"
#include <QApplication>
#include <QMessageBox>
#include <windows.h>

extern "C" {
#include "wzhack.h"
}

void CheckForOtherInstance()
{
    HANDLE hMutex = NULL;

    hMutex = CreateMutexA(NULL, TRUE, "WarHack_Mutex");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        QMessageBox::critical(0, "Error", "Another instance of War Hack is already running");
        exit(-1);
    }

    if (hMutex == NULL)
    {
        QMessageBox::critical(0, "Error", QString::asprintf("Failed to create mutex: %lu", GetLastError()));
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    frmMain w;

    CheckForOtherInstance();
    w.show();

    return a.exec();
}

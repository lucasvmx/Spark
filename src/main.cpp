/**
 * @file main.cpp
 * @author Lucas Vieira de Jesus (lucas.engen.cc@gmail.com)
 * @brief Parte principal do programa
 * @version 0.1
 * @date 2020-08-22
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "frmmain.h"
#include <QApplication>
#include <QMessageBox>
#include <windows.h>
#include "wzhack.h"
#include "dynamic_loader.h"

static void CheckForOtherInstance()
{
    HANDLE hMutex = nullptr;

    hMutex = CreateMutexA(nullptr, TRUE, "WarHack_Mutex");
    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        QMessageBox::critical(nullptr, "Error", "Outra instância já está em execução");
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

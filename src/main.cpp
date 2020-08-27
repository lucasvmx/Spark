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
#ifdef Q_OS_WIN
    HANDLE hMutex = nullptr;

    hMutex = CreateMutexA(nullptr, TRUE, "Spark_Mutex");
    if (hMutex == nullptr)
    {
        QMessageBox::critical(nullptr, "Error", QString::asprintf("Failed to create mutex: %lu", GetLastError()));
        exit(1);
    }

    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        QMessageBox::critical(nullptr, "Error", "Outra instância já está em execução");
        exit(1);
    }
#else
    // TODO: implementar para o linux
#endif
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    frmMain mainForm;

    CheckForOtherInstance();

    // Carrega as funcões da DLL
    LoadDLLFunctions();

    mainForm.show();

    return a.exec();
}

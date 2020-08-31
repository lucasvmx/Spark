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
#include <QObject>
#include <signal.h>
#ifdef Q_OS_WIN
#include <windows.h>
#endif
#include "wzhack.h"
#include "dynamic_loader.h"

void signal_handler(int signum)
{
    switch(signum)
    {
    case SIGABRT:
    case SIGSEGV:
        QMessageBox::critical(0, QObject::tr("Falha de aplicativo"),
                              QObject::tr("Um erro crítico foi detectado e o programa precisa ser fechado. "
                                          "Código do erro: %1").arg(signum));
    }

#ifdef Q_OS_WIN
    ExitProcess(1);
#else
    exit(1);
#endif
}

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
    QApplication app(argc, argv);
    frmMain mainForm;

    signal(SIGABRT, signal_handler);
    signal(SIGSEGV, signal_handler);

    CheckForOtherInstance();

    // Carrega as funcões da DLL
    LoadDLLFunctions();

    // Exibe a janela
    mainForm.show();

    return app.exec();
}

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
#include "frmlanguage.h"
#include <QApplication>
#include <QMessageBox>
#include <QObject>
#include <QTranslator>
#include <signal.h>
#ifdef Q_OS_WIN
#include <windows.h>
#endif
#include "wzhack.h"
#include "dynamic_loader.h"

#define TR(x)   QObject::tr(x)

void signal_handler(int signum)
{
    switch(signum)
    {
    case SIGABRT:
    case SIGSEGV:
        QMessageBox::critical(0, TR("Falha de aplicativo"),
                              TR("Um erro crítico foi detectado e o programa precisa ser fechado. "
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
        QMessageBox::critical(nullptr, TR("Erro"), QString::asprintf(TR("Falha ao criar mutex: %lu").toStdString().c_str(), GetLastError()));
        exit(1);
    }

    if (GetLastError() == ERROR_ALREADY_EXISTS)
    {
        QMessageBox::critical(nullptr, TR("Erro"),
                              TR("Outra instância já está em execução"));
        exit(1);
    }
#else
    // TODO: implementar para o linux
#endif
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    frmMain *mainForm;
    QTranslator translator;

    signal(SIGABRT, signal_handler);
    signal(SIGSEGV, signal_handler);


    CheckForOtherInstance();

    // Carrega as funcões da DLL
    LoadDLLFunctions();

    // Exibe o diálogo para selecionar o idioma
    auto f = new frmLanguage(nullptr);
    f->show();

    while(f->isVisible()) {
        app.processEvents();
    }

    f->close();

    // Carrega a tradução selecionada
    if(f->GetSelectedLanguage() == Ui::languages::ENGLISH) {
        auto result = translator.load("spark_en_us");
        if(result) {
            app.installTranslator(&translator);
            fprintf(stderr, "Loaded english translation\n");
        } else {
            QMessageBox::critical(0, TR("Erro"), TR("Falha ao carregar a tradução"));
            exit(1)
;        }
    } else {
        fprintf(stderr, "Loaded portuguese translation\n");
    }

    mainForm = new frmMain(nullptr);

    // Exibe a janela
    mainForm->show();

    return app.exec();
}

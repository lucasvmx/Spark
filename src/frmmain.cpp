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
#include "ui_frmmain.h"
#include "frmsettings.h"
#include "task.h"
#include "frmabout.h"

#include <QMessageBox>
#include <stdio.h>
#include <stdarg.h>

#include "wzhack.h"

static task *hack_task = nullptr;

frmMain::frmMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::frmMain)
{
    ui->setupUi(this);
    hack_task = new task();
    this->connectAllSignals();
    this->setFixedWidth(this->width());
    this->setFixedHeight(this->height());
}

frmMain::~frmMain()
{
    delete ui;
}

void frmMain::connectAllSignals()
{
    QObject::connect(ui->buttonStart, SIGNAL(clicked(bool)), this, SLOT(OnButtonStartClicked(bool)));
    QObject::connect(ui->buttonSettings, SIGNAL(clicked(bool)), this, SLOT(OnButtonSettingsClicked(bool)));
    QObject::connect(ui->actionAbout_Qt, SIGNAL(triggered(bool)), this, SLOT(OnAction_AboutQtTriggered(bool)));
    QObject::connect(ui->actionQuit_2, SIGNAL(triggered(bool)), this, SLOT(OnAction_QuitTriggered(bool)));
    QObject::connect(hack_task,SIGNAL(update(QString)), this, SLOT(delegateSetText(QString)));
    QObject::connect(ui->actionAbout_WarHack, SIGNAL(triggered(bool)), this, SLOT(OnAction_AboutTriggered(bool)));
}

void frmMain::OnButtonStartClicked(bool x)
{
    (void)x;

    if(hack_task->isRunning())
    {
        printText(error, "Uma outra tarefa já está em execução. Parando ...<br>");
        hack_task->requestInterruption();
        return;
    }

    ui->textBrowserOutput->clear();
    printText(log_info, "Iniciando ...<br>");
    hack_task->start();
}

void frmMain::OnButtonSettingsClicked(bool x)
{
    UNREFERENCED_PARAMETER(x)
    static frmSettings *settings = nullptr;

    if(settings != nullptr && settings->isVisible())
    {
        QMessageBox::critical(this, "Erro", "A caixa de configurações já está aberta");
    } else {
        settings = new frmSettings();
        settings->show();
    }
}

void frmMain::OnAction_AboutQtTriggered(bool x)
{
    UNREFERENCED_PARAMETER(x)
    QApplication::aboutQt();
}

void frmMain::OnAction_QuitTriggered(bool x)
{
    UNREFERENCED_PARAMETER(x)
    this->close();
}

void frmMain::delegateSetText(QString text)
{
    printText(log_info,text.toStdString().c_str());
}

void frmMain::OnAction_AboutTriggered(bool x)
{
    static frmAbout *about = nullptr;

    (void)x;

    if(about == nullptr)
        about = new frmAbout();

    if(about->isVisible())
    {
        QMessageBox::warning(this, "Erro", "A janela sobre encontra-se aberta");
    } else {
        about->show();
    }
}

void frmMain::printText(int id, const char *text, ...)
{
    va_list list;
    char buffer[256];

    va_start(list,text);
    vsnprintf(buffer, sizeof(char)*256,text, list);

    if(id == success)
       ui->textBrowserOutput->insertHtml("<font color=\"green\">[+]</font> ");
    else if(id == error)
       ui->textBrowserOutput->insertHtml("<font color=\"red\">[*]</font> ");
    else if(id == panic)
       ui->textBrowserOutput->insertHtml("<font color=\"red\">[!]</font> ");
    else if(id == log_info)
       ui->textBrowserOutput->insertHtml("<font color=#00c4ff>[*]</font> ");
    else if(id == info)
       ui->textBrowserOutput->insertHtml("<font color=\"blue\">[*]</font> ");
    else
        return;

    ui->textBrowserOutput->insertHtml(QString(buffer));
}

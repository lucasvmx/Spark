/**
 * @file frmmain.cpp
 * @author Lucas Vieira de Jesus (lucas.engen.cc@gmail.com)
 * @brief Janela principal da interface gráfica
 * @version 0.1
 * @date 2020-08-22
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "frmmain.h"
#include "ui_frmmain.h"
#include "frmsettings.h"
#include "threads.h"
#include "frmabout.h"
#include "exception.h"

#include <QMessageBox>
#include <cstdio>
#include <cstdarg>

#include "wzhack.h"
#include "version.h"

static Threads::MainHackingThread *hackingThread = nullptr;

frmMain::frmMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::frmMain)
{
    ui->setupUi(this);

    // Inicializa a classe das threads
    hackingThread = new Threads::MainHackingThread();

    // Conecta os sinais de usuário
    this->connectAllSignals();

    // Constroi o titulo
    if(SPARK_PATCH > 0)
        this->setWindowTitle(QString("%1 v%2.%3.%4").arg(PROGNAME).arg(SPARK_MAJOR).arg(SPARK_MINOR).arg(SPARK_PATCH));
    else
        this->setWindowTitle(QString("%1 v%2.%3").arg(PROGNAME).arg(SPARK_MAJOR).arg(SPARK_MINOR));

    // Ajusta o tamanho da janela
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
    QObject::connect(hackingThread, SIGNAL(updateStatus(QString)), this, SLOT(delegateSetText(QString)));
    QObject::connect(ui->actionAbout_WarHack, SIGNAL(triggered(bool)), this, SLOT(OnAction_AboutTriggered(bool)));
    QObject::connect(hackingThread, SIGNAL(showCriticalMsgBox(QString, QString)), this, SLOT(showCriticalMsgBox(QString, QString)));
}

void frmMain::OnButtonStartClicked(bool x)
{
    Q_UNUSED(x)

    if(hackingThread->isRunning())
    {
        println(error, tr("Uma outra tarefa já está em execução. Parando ..."));
        hackingThread->requestInterruption();
        return;
    }

    ui->textBrowserOutput->clear();
    println(log_info, tr("Iniciando ..."));
    hackingThread->start();
}

void frmMain::OnButtonSettingsClicked(bool x)
{
    Q_UNUSED(x)
    static frmSettings *settings = nullptr;

    if(settings != nullptr && settings->isVisible())
    {
        QMessageBox::critical(this, tr("Erro"), tr("A caixa de configurações já está aberta"));
    } else {
        settings = new frmSettings();
        settings->show();
    }
}

void frmMain::OnAction_AboutQtTriggered(bool x)
{
    Q_UNUSED(x)

    QApplication::aboutQt();
}

void frmMain::OnAction_QuitTriggered(bool x)
{
    Q_UNUSED(x)

    this->close();
}

void frmMain::delegateSetText(QString text)
{
    println(log_info, text);
}

void frmMain::OnAction_AboutTriggered(bool x)
{
    static frmAbout *about = nullptr;

    Q_UNUSED(x)

    if(about == nullptr)
        about = new frmAbout();

    if(about->isVisible())
    {
        QMessageBox::warning(this, tr("Erro"), tr("A janela sobre encontra-se aberta"));
    } else {
        about->show();
    }
}

void frmMain::setTextColorFromId(int id)
{
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
}

void frmMain::printText(int id, QString text)
{
    this->setTextColorFromId(id);
    ui->textBrowserOutput->insertHtml(text);
}

void frmMain::println(int id, QString text)
{
    this->setTextColorFromId(id);
    ui->textBrowserOutput->insertHtml(text + "<br>");
}

void frmMain::showCriticalMsgBox(QString title, QString text)
{
    QMessageBox::critical(0, title, text);
}

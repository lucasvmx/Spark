#include "frmupdate.h"
#include "ui_frmupdate.h"
#include "version.h"
#include <QDir>
#include <QMessageBox>

frmUpdate::frmUpdate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmUpdate)
{
    updateTask = new Updater();
    ui->setupUi(this);
    setFixedSize(width(), height());
    setWindowTitle(tr("Spark - Atualização"));

    connectSignals();
}

Updater::Updater()
{
    p = new QProcess();
}

Updater::~Updater()
{
    delete p;
}

void Updater::run()
{
    QString ver = QString("%1.%2.%3").arg(SPARK_MAJOR).arg(SPARK_MINOR-1).arg(SPARK_PATCH);
    QString arch;

#if defined(Q_OS_WIN) && !defined(Q_OS_WIN32)
    arch = "x64";
#else
    arch = "x86";
#endif
    QStringList args;

    emit messageAvailable(tr("Buscando atualizações ..."));
    args << "--current_version" << ver << "--arch" << arch;
    auto status = p->execute(QDir::currentPath() + "/updater/updater.exe", args);
    if(status < 0) {
        emit messageAvailable(tr("Erro ao executar o programa de atualização! Código: %1").arg(status));
        emit updateFailed();
    } else {
        // Handle exit code
        switch(status)
        {
        case 0xFF1:
            emit messageAvailable(tr("Você já possui a última versão"));
            emit updateFinished();
            return;

        default:
            emit messageAvailable(tr("Algo deu errado no processo de atualização (codigo: %1). Repita o processo").arg(status));
            emit updateFailed();
        }
    }
}

void frmUpdate::LaunchUpdate()
{
    if(!updateTask->isRunning())
    {
        setUndefinedProgressBar();
        ui->textBrowser->clear();
        updateTask->start();
    } else {
        QMessageBox::warning(0, tr("Atualização"), tr("O processo de atualização está em curso"));
    }
}

frmUpdate::~frmUpdate()
{
    delete ui;
}

void frmUpdate::OnUpdateFailed()
{
    redefineProgressBar();
    QMessageBox::critical(0, tr("Erro"), tr("Algo deu errado com a atualização: clique para fechar a janela"));
    this->close();
}

void frmUpdate::OnUpdateButtonClick(bool v)
{
    (void)v;

    LaunchUpdate();
}

void frmUpdate::OnMessageAvailable(QString text)
{
    ui->textBrowser->append(text);
}

void frmUpdate::OnFinished()
{
    redefineProgressBar();
}

void frmUpdate::connectSignals()
{
    QObject::connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(OnUpdateButtonClick(bool)));
    QObject::connect(updateTask, SIGNAL(messageAvailable(QString)), this, SLOT(OnMessageAvailable(QString)));
    QObject::connect(updateTask, SIGNAL(updateFinished()), this, SLOT(OnFinished()));
}

void frmUpdate::setUndefinedProgressBar()
{
    ui->progressBar->setMaximum(0);
    ui->progressBar->setMinimum(0);
}

void frmUpdate::redefineProgressBar()
{
    ui->progressBar->setMaximum(1);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setValue(0);
}


#include "frmupdate.h"
#include "ui_frmupdate.h"
#include "version.h"
#include <QDir>
#include <QMessageBox>
#include <QScopedPointer>

#ifdef QT_DEBUG
#include <QDebug>
#endif

#ifdef Q_OS_WIN64
#define ARCH "x64"
#else
#define ARCH "x86"
#endif

frmUpdate::frmUpdate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmUpdate)
{
    ui->setupUi(this);
    setFixedSize(width(), height());
    setWindowTitle(tr("Spark - Atualização"));

    m_manager = new QNetworkAccessManager();
    m_reply = nullptr;
    m_download_reply = nullptr;
    connectSignals();

    progressRangeConfigured = false;
}

void frmUpdate::OnRequestFinished(QNetworkReply *reply)
{
    qDebug() << "request finished";
    QString versionTag = reply->header(QNetworkRequest::LocationHeader).toUrl().fileName().toStdString().substr(1).c_str();

    QVersionNumber current(SPARK_MAJOR, SPARK_MINOR, SPARK_PATCH);
    QVersionNumber latest = QVersionNumber::fromString(versionTag);

    if(latest > current) {
        emit updateFound();

        OnMessageAvailable(tr("Atualização disponível. Versão %1").arg(latest.toString()));

        QMessageBox::StandardButton answer = QMessageBox::question(this, tr("Pergunta"),
                              tr("Há uma atualização disponível (Versão %1). Deseja baixar?").arg(latest.toString()));

        if(answer == QMessageBox::StandardButton::Yes) {
            OnMessageAvailable(tr("Baixando atualização ..."));

            // Desconecta os sinais e conecta um novo
            QObject::disconnect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(OnRequestFinished(QNetworkReply*)));
            QObject::connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(OnDownloadRequestFinished(QNetworkReply*)));

            m_localFilename = QString("Spark-v%1-%2.exe").arg(latest.toString(), ARCH);

            QNetworkRequest downloadRequest;
            QString fileUrl = downloadBaseUrl + "/" + QString("v%1").arg(latest.toString()) + "/" +
                    m_localFilename;

            downloadRequest.setUrl(fileUrl);
            OnMessageAvailable(tr("Baixando arquivo: %1 ...").arg(fileUrl));

            m_download_reply = m_manager->get(downloadRequest);
            QObject::connect(m_download_reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(OnDownloadProgressChange(qint64,qint64)));
        } else {
            OnMessageAvailable(tr("Atualização cancelada pelo usuário"));
            ui->pushButton->setEnabled(true);
        }
    } else {
        ui->textBrowser->setMarkdown(tr("Você já está executando a última versão deste programa"));
        ui->textBrowser->append(tr("Versão mais recente: %1").arg(latest.toString()));
        redefineProgressBar();
        ui->pushButton->setEnabled(true);
    }
}

void frmUpdate::OnDownloadProgressChange(qint64 received, qint64 bytesTotal)
{
    if(!progressRangeConfigured) {
        ui->progressBar->setRange(0, bytesTotal);
        progressRangeConfigured = true;
    }

    ui->progressBar->setValue(received);
    OnMessageAvailable(tr("Baixados %1 de %2 KB").arg(received / 1024).arg(bytesTotal / 1024));
}

void frmUpdate::OnDownloadRequestFinished(QNetworkReply *reply)
{
    OnMessageAvailable(tr("Download finalizado com sucesso"));

    QByteArray fileData = reply->readAll();
    QString fullInstallerPath = QDir::cleanPath(QDir::tempPath() + QDir::separator() + m_localFilename);

    QFile file(fullInstallerPath);
    if(!file.open(QIODevice::ReadWrite)) {
        QMessageBox::critical(this, tr("Erro"), tr("Não foi possível executar o instalador"));
        this->redefineProgressBar();
        ui->pushButton->setEnabled(true);
        QObject::disconnect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(OnDownloadRequestFinished(QNetworkReply*)));
        return;
    }

    file.write(fileData);
    file.close();

    // reseta a barra de progresso
    this->redefineProgressBar();

    QMessageBox::StandardButton answer = QMessageBox::question(this, tr("Pergunta"),
                                                               tr("Você deseja executar o instalador agora?"));
    if(answer == QMessageBox::StandardButton::Yes) {
        QScopedPointer<QProcess> installer = QScopedPointer<QProcess>(new QProcess());

        // executa o programa de instalação
        if(!installer->startDetached(fullInstallerPath)) {
            OnMessageAvailable(tr("Erro ao executar programa de instalação"));
        } else {
            OnMessageAvailable(tr("Instalador em execução ..."));
        }
    }

    ui->pushButton->setEnabled(true);

    // Desconecta os objetos
    QObject::disconnect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(OnDownloadRequestFinished(QNetworkReply*)));
}

void frmUpdate::OnErrorOccurred(QNetworkReply::NetworkError e)
{
    ui->pushButton->setEnabled(true);
    qDebug() << "error:" <<e;
}

void frmUpdate::LaunchUpdate()
{
    ui->pushButton->setEnabled(false);

    setUndefinedProgressBar();
    ui->textBrowser->clear();

    QNetworkRequest request(this->baseUrl);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::ManualRedirectPolicy);

    // Conecta o sinal
    QObject::connect(m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(OnRequestFinished(QNetworkReply*)));

    m_reply = m_manager->get(request);
    QObject::connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this, SLOT(OnErrorOccurred(QNetworkReply::NetworkError)));

    qDebug() << "request sent to" <<baseUrl;
}

frmUpdate::~frmUpdate()
{
    qDebug() <<  "destroying form ...";
    delete m_manager;
    delete ui;
    delete m_reply;
    delete m_download_reply;
}

void frmUpdate::OnFailed()
{
    redefineProgressBar();
    QMessageBox::critical(0, tr("Erro"), tr("Algo deu errado com a atualização: clique para fechar a janela"));
    ui->textBrowser->clear();
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
    QObject::connect(this, &frmUpdate::updateFound, &frmUpdate::redefineProgressBar);
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


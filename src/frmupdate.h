#ifndef FRMUPDATE_H
#define FRMUPDATE_H

#include <QWidget>
#include <QThread>
#include <QProcess>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QMutex>
#include <QVersionNumber>

namespace Ui {
class frmUpdate;
}

class Updater;

class frmUpdate : public QWidget
{
    Q_OBJECT

public:
    explicit frmUpdate(QWidget *parent = nullptr);
    ~frmUpdate();
    void LaunchUpdate();

signals:
    void updateFound();
public slots:
    void OnFailed();
    void OnUpdateButtonClick(bool);
    void OnMessageAvailable(QString);
    void OnFinished();

    void OnErrorOccurred(QNetworkReply::NetworkError e);
    void OnRequestFinished(QNetworkReply *reply);
    void OnDownloadProgressChange(qint64 received, qint64 bytesTotal);
    void OnDownloadRequestFinished(QNetworkReply *reply);
private:
    Ui::frmUpdate *ui;
    QNetworkAccessManager *m_manager;
    QNetworkReply *m_reply, *m_download_reply;
    const QString baseUrl = "https://github.com/lucasvmx/Spark/releases/latest";
    const QString downloadBaseUrl = "https://github.com/lucasvmx/Spark/releases/download";
    QString m_localFilename; // nome do arquivo baixado a partir da atualização
    bool progressRangeConfigured;

    void connectSignals();
    void setUndefinedProgressBar();
    void redefineProgressBar();
};



#endif // FRMUPDATE_H

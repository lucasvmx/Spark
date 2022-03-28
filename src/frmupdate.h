#ifndef FRMUPDATE_H
#define FRMUPDATE_H

#include <QWidget>
#include <QThread>
#include <QProcess>

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

public slots:
    void OnFailed();
    void OnUpdateButtonClick(bool);
    void OnMessageAvailable(QString);
    void OnFinished();
private:
    Ui::frmUpdate *ui;
    Updater *updateTask;

    void connectSignals();
    void setUndefinedProgressBar();
    void redefineProgressBar();
};

class Updater : public QThread {

    Q_OBJECT

public:
    Updater();
    ~Updater();

signals:
    void messageAvailable(QString msg);
    void updateFailed();
    void updateFinished();

private:

    void run();

private:
    QProcess *p;
};


#endif // FRMUPDATE_H

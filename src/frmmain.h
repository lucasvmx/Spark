/**
 * @file frmmain.h
 * @author Lucas Vieira de Jesus (lucas.engen.cc@gmail.com)
 * @brief Informações da janela principal
 * @version 0.1
 * @date 2020-08-22
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QMainWindow>
#include <QThread>
#include <QApplication>
#include <QTranslator>

namespace Ui {
class frmMain;
}

class frmMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = nullptr);
    ~frmMain();
    void connectAllSignals();


public slots:
    void OnButtonStartClicked(bool x);
    void OnButtonSettingsClicked(bool x);
    void OnAction_AboutQtTriggered(bool x);
    void OnAction_QuitTriggered(bool x);
    void delegateSetText(QString text);
    void OnAction_AboutTriggered(bool x);

    void showCriticalMsgBox(QString title, QString text);
private:
    Ui::frmMain *ui;
    void setTextColorFromId(int id);
    void printText(int id, QString text);
    void println(int id, QString text);
};

enum textId {
    success, error, panic, log_info, info
};

#endif // FRMMAIN_H

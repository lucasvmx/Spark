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

#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QMainWindow>
#include <QThread>

namespace Ui {
class frmMain;
}

class frmMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();
    void connectAllSignals();

public slots:
    void OnButtonStartClicked(bool x);
    void OnButtonSettingsClicked(bool x);
    void OnAction_AboutQtTriggered(bool x);
    void OnAction_QuitTriggered(bool x);
    void delegateSetText(QString text);
    void OnAction_AboutTriggered(bool x);

private:
    Ui::frmMain *ui;
    void printText(int id, const char *text, ...);
};

enum textId {
    success, error, panic, log_info, info
};

#endif // FRMMAIN_H

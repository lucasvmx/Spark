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

#ifndef FRMSETTINGS_H
#define FRMSETTINGS_H

#include <QWidget>

namespace Ui {
class frmSettings;
}

class frmSettings : public QWidget
{
    Q_OBJECT

public:
    explicit frmSettings(QWidget *parent = 0);
    ~frmSettings();
    void connectAllSignals();

public slots:
    void OnButtonSave_Clicked(bool b);
    void OnSliderMoved(int v);

private:
    Ui::frmSettings *ui;
};

extern int globalId;
extern int globalDelay;
extern bool globalZero;
extern bool globalSupport;
extern bool globalHighEnergy;

#endif // FRMSETTINGS_H

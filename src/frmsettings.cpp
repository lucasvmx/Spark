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

#include "frmsettings.h"
#include "ui_frmsettings.h"
#include <QMessageBox>

int globalId = -1;
int globalDelay = -1;
bool globalZero = false;
bool globalSupport = false;
bool globalHighEnergy = false;

frmSettings::frmSettings(QWidget *parent) : QWidget(parent), ui(new Ui::frmSettings)
{
    ui->setupUi(this);
    this->connectAllSignals();
    this->setFixedHeight(this->height());
    this->setFixedWidth(this->width());
}

frmSettings::~frmSettings()
{
    delete ui;
}

void frmSettings::connectAllSignals()
{
    QObject::connect(ui->buttonSave, SIGNAL(clicked(bool)), this, SLOT(OnButtonSave_Clicked(bool)));
    QObject::connect(ui->horizontalSlider, SIGNAL(sliderMoved(int)), this, SLOT(OnSliderMoved(int)));
}

void frmSettings::OnButtonSave_Clicked(bool b)
{
    int playerId = -1;
    int delay = -1;
    bool zero_energy = false;
    bool support_player = false;
    bool high_energy = false;

    (void)b;

    playerId = ui->comboBox->currentIndex();
    delay = ui->horizontalSlider->value();
    zero_energy = ui->checkBox->isChecked();
    support_player = ui->checkBox_2->isChecked();
    high_energy = ui->checkBox_3->isChecked();

    /*
        Independentemente do slot em que estamos, nosso id sempre será o 0
        Os demais id'serão sempre reservados para a IA
    */

    globalId = playerId;
    globalDelay = delay;
    globalZero = zero_energy;
    globalSupport = support_player;
    globalHighEnergy = high_energy;

    QMessageBox::information(this, "WarHack", "Configurações salvas!");
    this->close();
}

void frmSettings::OnSliderMoved(int v)
{
    ui->label_3->setText(QString::asprintf( "%d segundos", v));
}

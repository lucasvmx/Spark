/**
 * @file frmsettings.cpp
 * @author Lucas Vieira de Jesus (lucas.engen.cc@gmail.com)
 * @brief Janela para configurar o hack
 * @version 0.1
 * @date 2020-08-22
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "frmsettings.h"
#include "ui_frmsettings.h"
#include <QMessageBox>

// id do jogador selecionado (padrão: 0)
int player_id = 0;

// tempo de espera entre uma iteração e a outra do hack (padrão: 5)
int hacking_delay = 5;

// flag para armazenar se a energia do inimigo deverá ser zerada
bool bEraseEnemyEnergy = false;

// flag para armazenar se um jogador em específico deverá ser favorecido
bool bSupportSpecificPlayer = false;

// flag para armazenar se o jogador escolhido deverá ter a energia infinita
bool bInfiniteEnergy = false;

// flag para armzenar se o modo Deus deverá ser habilitado
bool bEnableGodMode = false;

// flag para armazenar se a detecção de anti-cheat deverá ser burlada
bool bPreventAntiCheatDetection = false;

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
    QObject::connect(ui->horizontalSlider_delay, SIGNAL(sliderMoved(int)), this, SLOT(OnSliderMoved(int)));
}

void frmSettings::OnButtonSave_Clicked(bool b)
{
    Q_UNUSED(b);

    // Extrai os dados da configuração
    bInfiniteEnergy = ui->radioButton_infinite_energy->isChecked();
    bSupportSpecificPlayer = ui->checkBox_support_specific_player->isChecked();
    bEraseEnemyEnergy = ui->radioButton_erase_enemy_energy->isChecked();
    bEnableGodMode = ui->checkBox_enable_godmode->isChecked();
    bPreventAntiCheatDetection = ui->checkBox_prevent_anticheat->isChecked();
    hacking_delay = ui->horizontalSlider_delay->value();
    player_id = ui->comboBox_playerId->currentIndex();

    if(!bInfiniteEnergy && !bSupportSpecificPlayer && !bEraseEnemyEnergy && !bEnableGodMode)
    {
        bInfiniteEnergy = true;
        QMessageBox::warning(0, "Atenção",
                             "Você não escolheu nenhum algoritmo de hacking. Por padrão, o de energia infinita será usado");
    } else
    {
        QMessageBox::information(0, "Sucesso", "Configurações salvas!");
        this->close();
    }
}

void frmSettings::OnSliderMoved(int v)
{
    ui->labelWaitTime->setText(QString::asprintf( "%d segundos", v));
}

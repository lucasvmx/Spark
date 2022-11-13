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
#include "version.h"
#include <QMessageBox>
#ifdef QT_DEBUG
#include <QDebug>
#endif

frmSettings::frmSettings(QWidget *parent, QSharedPointer<GameSettings> &config) : QWidget(parent), ui(new Ui::frmSettings)
{
    ui->setupUi(this);
    this->connectAllSignals();
    this->setFixedHeight(this->height());
    this->setFixedWidth(this->width());
    this->setWindowTitle(tr("%1 - configurações").arg(PROGNAME));
    ui->label->setText(tr("Tempo de espera:"));
    configAlias = config;
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
    configAlias->setConfigValue(S_INFINITE_ENERGY, ui->radioButton_infinite_energy->isChecked());
    configAlias->setConfigValue(S_SUPPORT_SPECIFIC_PLAYER, ui->checkBox_support_specific_player->isChecked());
    configAlias->setConfigValue(S_ERASE_ENEMY_POWER, ui->radioButton_erase_enemy_energy->isChecked());
    configAlias->setConfigValue(S_ENABLE_GOD_MODE, ui->checkBox_enable_godmode->isChecked());
    configAlias->setConfigValue(S_PREVENT_ANTICHEAT, ui->checkBox_prevent_anticheat->isChecked());
    configAlias->setConfigValue(S_INCREASE_POWER_GEN_SPEED, ui->radioButton_increasePowerGen->isChecked());
    configAlias->setConfigValue(S_HACK_DELAY, ui->horizontalSlider_delay->value());
    configAlias->setConfigValue(S_PLAYER_ID, ui->comboBox_playerId->currentIndex());
    configAlias->setConfigValue(S_INCREASE_PLAYER_DAMAGE, ui->checkBox_increase_damage->isChecked());

    if(!configAlias->getConfigValue(S_INFINITE_ENERGY).toBool() &&
            !configAlias->getConfigValue(S_SUPPORT_SPECIFIC_PLAYER).toBool() &&
            !configAlias->getConfigValue(S_ERASE_ENEMY_POWER).toBool() &&
            !configAlias->getConfigValue(S_ENABLE_GOD_MODE).toBool() &&
            !configAlias->getConfigValue(S_PREVENT_ANTICHEAT).toBool() &&
            !configAlias->getConfigValue(S_INCREASE_POWER_GEN_SPEED).toBool())
    {
        configAlias->setConfigValue(S_INFINITE_ENERGY, true);
        QMessageBox::warning(0, tr("Atenção"),
                             tr("Você não escolheu nenhum algoritmo de hacking. Por padrão, o de energia infinita será usado"));
    } else
    {
        QMessageBox::information(0, tr("Sucesso"), tr("Configurações salvas!"));
        this->close();
    }
}

void frmSettings::OnSliderMoved(int v)
{
    ui->labelWaitTime->setText(QString::asprintf(tr("%d segundos").toStdString().c_str(), v));
}

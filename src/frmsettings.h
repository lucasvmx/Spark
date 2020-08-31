/**
 * @file frmsettings.h
 * @author Lucas Vieira de Jesus (lucas.engen.cc@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-08-22
 * 
 * @copyright Copyright (c) 2020
 * 
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
    explicit frmSettings(QWidget *parent = nullptr);
    ~frmSettings();
    void connectAllSignals();

public slots:
    void OnButtonSave_Clicked(bool b);
    void OnSliderMoved(int v);

private:
    Ui::frmSettings *ui;
};

// id do jogador selecionado
extern int player_id;

// tempo de espera entre uma iteração e a outra do hack
extern int hacking_delay;

// flag para armazenar se a energia do inimigo deverá ser zerada
extern bool bEraseEnemyEnergy;

// flag para armazenar se um jogador em específico deverá ser favorecido
extern bool bSupportSpecificPlayer;

// flag para armazenar se o jogador escolhido deverá ter a energia infinita
extern bool bInfiniteEnergy;

// flag para armzenar se o modo Deus deverá ser habilitado
extern bool bEnableGodMode;

// flag para armazenar se a detecção de anti-cheat deverá ser burlada
extern bool bPreventAntiCheatDetection;

#endif // FRMSETTINGS_H

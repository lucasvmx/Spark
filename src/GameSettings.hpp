#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMap>
#include <QString>
#include <QVariant>

// Keys used to save settings on map
#define S_PLAYER_ID "player_id"
#define S_HACK_DELAY "hacking_delay"
#define S_ERASE_ENEMY_POWER "erase_enemy_energy"
#define S_SUPPORT_SPECIFIC_PLAYER "support_specific_player"
#define S_INFINITE_ENERGY "infinity_energy"
#define S_ENABLE_GOD_MODE "enable_god_mode"
#define S_PREVENT_ANTICHEAT "prevent_anti_cheat_detection"
#define S_INCREASE_POWER_GEN_SPEED "increase_power_generation_speed"

class GameSettings {

public:
    GameSettings();
    QMap<QString,QVariant> getSettings();
    void updateSettings(QMap<QString,QVariant>& newSettings);
    QVariant getConfigValue(QString keyName) const;
	bool setConfigValue(QString keyName, QVariant newValue);
	void setDefault();
private:
   QMap<QString,QVariant> settings;
};

#endif

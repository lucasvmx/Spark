#include "GameSettings.hpp"

GameSettings::GameSettings()
{
    // Initializes settings with default values
	setDefault();
}

QMap<QString, QVariant> GameSettings::getSettings()
{
    return this->settings;
}

void GameSettings::updateSettings(QMap<QString, QVariant> &newSettings)
{
    this->settings = newSettings;
}

QVariant GameSettings::getConfigValue(QString keyName) const
{
    const auto it = settings.find(keyName);

    if(it == settings.end()) {
        return QVariant();
    }

    return it.value();
}

bool GameSettings::setConfigValue(QString keyName, QVariant newValue)
{
	auto it = settings.find(keyName);
	if(it != settings.end() && !it.value().isNull()) {
		settings[keyName] = newValue;
	}
	
	return false;
}

void GameSettings::setDefault()
{
    this->settings = {
        // id do jogador selecionado (padrão: 0)
        {S_PLAYER_ID, 0U},

        // tempo de espera (em segundos) entre uma iteração e a outra do hack (padrão: 5)
        {S_HACK_DELAY, 5U},

        // flag para armazenar se a energia do inimigo deverá ser zerada
        {S_ERASE_ENEMY_POWER, false},

        // flag para armazenar se um jogador em específico deverá ser favorecido
        {S_SUPPORT_SPECIFIC_PLAYER, false},

        // flag para armazenar se o jogador escolhido deverá ter a energia infinita
        {S_INFINITE_ENERGY, false},

        // flag para armazenar se o modo Deus deverá ser habilitado
        {S_ENABLE_GOD_MODE, false},


        // flag para armazenar se a detecção de anti-cheat deverá ser burlada
        {S_PREVENT_ANTICHEAT, false},

        // flag para armazenar se a velocidade de geração de energia deve ser aumentada
        {S_INCREASE_POWER_GEN_SPEED, false},

        // flag para indiciar se os danos causados pelo jogador devem ser aumentados
        {S_INCREASE_PLAYER_DAMAGE, false}
    };
}

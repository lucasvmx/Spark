/**
 * @file thread.h
 * @author Lucas Vieira de Jesus (lucas.engen.cc@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-08-22
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once

#include "exception.h"
#include "GameSettings.hpp"
#include <QThread>
#include <QSharedPointer>
#include <QMutex>

namespace Threads
{
    /**
     * @brief The MainHackingThread class
     */
    class MainHackingThread : public QThread
    {
        Q_OBJECT

    public:
        /**
         * @brief Inicializa a classe
         */
        explicit MainHackingThread(QSharedPointer<GameSettings> cfg);


    private:
        void run();
        void startLibhack();
		
        /**
         * @brief hack_handle ponteiro para o handle retornado por libhack_init
         */
        QSharedPointer<libhack_handle> hack_handle;
        uint32_t max_power;

        /**
         * @brief player_id
         */
        int player_id;

        /**
         * @brief config armazena as configurações do cheat
         */
        GameSettings *config;

		/**
		 * @brief Determina se a thread do trainer deve continuar em execução
		 * @return TRUE se ela deve continuar em execução, FALSE caso contrário
		 */
		bool canContinue() const;
		
        /**
         * @brief Aguarda até que o warzone 2100 seja aberto
         */
        void waitForWarzone();

        /**
         * @brief wait Aguarda por um tempo, sem bloquear a execução da thread
         * @param seconds Tempo a aguardar
         */
        void wait(uint32_t seconds);

        /**
         * @brief Oferece energia "infinita" ao jogador
         */
        void giveInfiniteEnergy();

        /**
         * @brief Zera a energia dos jogadores inimigos
         */
        void eraseEnemyEnergy();

        /**
         * @brief Verifica se a partida já começou
         * @return true se a partida tiver começado, falso caso contrário
         */
        bool isGamePlayStarted() const;
		
        /**
         * @brief Alias para isGamePlayStarted()
         * @return true se a partida tiver começado, falso caso contrário
         */
		bool isGamePlayRunning() const;
		
        /**
         * @brief Habilita o modo deus
         */
        void enableGodMode();

        /**
         * @brief Aumenta a velocidade de geração de energia
         */
        void increasePowerGenerationSpeed(unsigned new_modifier);

        /**
         * @brief increaseDamageOutput Aumenta os danos causados pelo jogador
         * @param new_damage valor do dano
         */
        void increaseDamageOutput(unsigned new_damage = 500);

        /**
         * @brief performFeaturesChecking Verifica se todas as features solicitadas são suportadas
         * @return true se forem suportadas, false c.c
         */
        bool performFeaturesChecking();
    signals:

        /**
         * @brief Exibe um texto no console da IU
         * @param status_text
         */
        void updateStatus(QString status_text);

        /**
         * @brief Exibe uma caixa de mensagem de alerta
         * @param title Título da mensagem
         * @param text Texto principal
         */
        void showCriticalMsgBox(QString title, QString text);
    };

    /**
     * @brief The AntiCheatDetectionThread class
     */
    class AntiCheatDetectionThread: public QThread
    {
        Q_OBJECT

    public:
        /**
         * @brief AntiCheatDetectionThread
         * @param warzone_version Versão do warzone 2100
         */
        AntiCheatDetectionThread(unsigned short warzone_version, libhack_handle *handle);

    private:

        QSharedPointer<libhack_handle> hack_handle;

        /**
         * @brief wz_version Armazena a versão do warzone
         */
        unsigned short wz_version;

        /**
         * @brief getGameStatusIndex Obtém a posição de inicio na estrutura game_status
         * @return
         */
        int getGameStatusIndex();

        /**
         * @brief wait Aguarda por um tempo determinado, evitando o bloqueio da thread
         * @param seconds Tempo a ser aguardado
         */
        void wait(uint32_t seconds);

        /**
         * @brief run Executa a thread
         */
        void run();
    };
}



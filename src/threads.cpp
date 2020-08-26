/**
 * @file thread.cpp
 * @author Lucas Vieira de Jesus (lucas.engen.cc@gmail.com)
 * @brief Implementação do hack propriamente dito
 * @version 0.1
 * @date 2020-08-22
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "threads.h"
#include "wzhack.h"
#include "frmmain.h"
#include "frmsettings.h"
#include "exception.h"
#include "dynamic_loader.h"
#include <QMessageBox>
#ifdef Q_OS_WIN
#include <windows.h>
#endif
#if defined(_DEBUG) || defined(QT_DEBUG)
#include <QDebug>
#endif

using namespace Threads;

static libhack_handle *hack_handle = nullptr;
static uint32_t MAX_POWER;

/**
 * @brief start Posição inicial na struct, de acordo com a versão do warzone atualmente em execução
 */
unsigned short start;

MainHackingThread::MainHackingThread()
{

}

void MainHackingThread::run()
{
    char warzone_path[BUFLEN];
    DWORD warzone_path_len = arraySize(warzone_path);
    short warzone_version;

    // Inicializa a libhack
    this->startLibhack();

    // Carrega as funções específicas do warzone
    LoadWarzoneFunctions(hack_handle);

    // Verifica se o warzone já está aberto. Se não estiver, aguarda pela sua abertura
    this->waitForWarzone();

    // Obtém o caminho completo do executável do warzone 2100
    if(!fQueryFullProcessImageFileName(hack_handle->hProcess, 0, warzone_path, &warzone_path_len))
        throw Exception(QString("Erro ao obter caminho do warzone: %1").arg(GetLastError()).toStdString().c_str());

    // Verifica qual versão do warzone está em execução
    warzone_version = WzHack_GetWarzoneVersion(warzone_path);

    // Obtém o valor máximo da energia
    MAX_POWER = GetWarzoneMaxPowerValue(warzone_version);

    // Se certifica se que a versão é conhecida
    Q_ASSERT(warzone_version != WZ_UNKNOWN);

    // Obtém a posição de inicio na estrutura
    WzHack_GetStructStartIndex(warzone_version, &::start);

    // Realizar as interferências no jogo
    if(bInfiniteEnergy)
        giveInfiniteEnergy();
    else if(bEraseEnemyEnergy)
        eraseEnemyEnergy();
}

void MainHackingThread::runHack()
{
    // Captura as exceções na thread principal
    try {
        this->run();
    } catch(Exception& error)
    {
        QMessageBox::critical(0, "Algo deu errado", error.what());
    }
}

void MainHackingThread::startLibhack()
{
    hack_handle = libhack_init(WZ_PROCESS_NAME);
    if(!hack_handle)
        throw Exception(QString("Erro ao inicializar a libhack: %1").arg(GetLastError()).toStdString().c_str());
}

void MainHackingThread::waitForWarzone()
{
    Q_ASSERT(hack_handle != nullptr);

#ifdef Q_OS_WIN
    // Tenta abrir o warzone
    while(!libhack_open_process(hack_handle))
    {
        if(GetLastError() == ERROR_ACCESS_DENIED)
            throw Exception(QString("Falha ao abrir processo do warzone: %1").arg(GetLastError()).toStdString().c_str());

        Sleep(5000);
    }
#endif
}

void MainHackingThread::wait(uint32_t seconds)
{
    short milisseconds = 1000;
    short tick = milisseconds / milisseconds;
    uint32_t waited = 0;

    while(!isInterruptionRequested() && !(waited >= seconds) && libhack_process_is_running(::hack_handle))
    {
#ifdef Q_OS_WIN
        Sleep(static_cast<DWORD>(milisseconds));
#endif
        waited += tick;
    }
}

void MainHackingThread::giveInfiniteEnergy()
{
    // Fazer o jogador ter energia infinita
    while(!this->isInterruptionRequested())
    {
        // Aumenta a energia do jogador selecionado, para o valor máximo
        if(bSupportSpecificPlayer)
            libhack_write_int_to_addr(hack_handle, player_info[::start + ::player_id].power_offset, ::MAX_POWER);
        else
            libhack_write_int_to_addr(hack_handle, player_info[::start].power_offset, ::MAX_POWER);

        // Aguarda pelo número de segundos desejado ou até que uma interrupção seja solicitada
        wait(::hacking_delay);
    }
}

void MainHackingThread::eraseEnemyEnergy()
{
    // Certifica-se de que podemos acessar a função
    Q_ASSERT(isHumanPlayer);

    while(!this->isInterruptionRequested())
    {
        // FIXME: encontrar método para não zerar a energia dos jogadores amigos
        for(int player = 0; player < MAX_PLAYERS; player++)
        {
            // Não zerar a energia do jogador favorecido
            if(bSupportSpecificPlayer && (::player_id == player))
                continue;

            if(!isHumanPlayer(player))
                libhack_write_int_to_addr(hack_handle, ::player_info[::start].power_offset, 0);
        }

        // Aguarda pelo número de segundos desejado ou até que uma interrupção seja solicitada
        wait(::hacking_delay);
    }
}

AntiCheatDetectionThread::AntiCheatDetectionThread(unsigned short warzone_version)
{
    Q_ASSERT(hack_handle != nullptr);
    this->wz_version = warzone_version;
}

void AntiCheatDetectionThread::preventAntiCheatDetection()
{
    try {
        this->run();
    } catch(Exception& error)
    {
        fprintf(stderr, "(%s:%d) Error: %s\n", __FILE__, __LINE__, error.what());
    }
}

int AntiCheatDetectionThread::getGameStatusIndex()
{
    int index = 0;

    for(const auto& element : gstatus)
    {
        if(element.warzone_version == this->wz_version)
            return index;

        ++index;
    }

    return -1;
}

void AntiCheatDetectionThread::wait(uint32_t seconds)
{
    uint32_t waited = 0;
    uint32_t tick_ms = 100;

    while((waited / 1000) < seconds)
    {
        if(this->isInterruptionRequested())
            break;

        Sleep(tick_ms);
        waited += tick_ms;
    }
}

void AntiCheatDetectionThread::run()
{
    int index = this->getGameStatusIndex();

    Q_ASSERT(index >= 0);

    while(!this->isInterruptionRequested())
    {
        libhack_write_int_to_addr(::hack_handle, ::gstatus[index].cheated_offset, 0);

        // Atualiza o valor a cada 5 segundos
        this->wait(5);
    }
}

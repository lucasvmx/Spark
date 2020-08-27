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

/**
 * @brief base_addr Endereço base do processo na memória
 */
unsigned long base_addr;

/**
 * @brief warzone_version Versão do warzone
 */
short detected_warzone_version;

MainHackingThread::MainHackingThread()
{
    ::base_addr = 0UL;
}

void MainHackingThread::run()
{
    char warzone_path[BUFLEN];
    DWORD warzone_path_len = arraySize(warzone_path);
    AntiCheatDetectionThread *antiCheat = nullptr;

    try {

        Q_ASSERT(1 == 2);

        // Inicializa a libhack
        this->startLibhack();

        // Carrega as funções específicas do warzone
        LoadWarzoneFunctions(hack_handle);

        // Verifica se o warzone já está aberto. Se não estiver, aguarda pela sua abertura
        this->waitForWarzone();

        // Obtém o caminho completo do executável do warzone 2100
        if(!fQueryFullProcessImageFileName(hack_handle->hProcess, 0, warzone_path, &warzone_path_len))
            throw new Exception(tr("Erro ao obter caminho do warzone: %1").arg(GetLastError()).toStdString().c_str());

        // Verifica qual versão do warzone está em execução
        ::detected_warzone_version = WzHack_GetWarzoneVersion(warzone_path);

        emit updateStatus(tr("Aguardando usuário entrar na partida ...<br>"));

        // Aguarda o usuário entrar na partida
        while(!this->isGamePlayStarted())
            this->wait(1);

        // Verifica se a versão suporta as operações solicitadas
        if(bEnableGodMode) Q_ASSERT(IsFeatureSupported(detected_warzone_version, FEATURE_GOD_MODE));
        if(bInfiniteEnergy) Q_ASSERT(IsFeatureSupported(detected_warzone_version, FEATURE_CHANGE_POWER));
        if(bEraseEnemyEnergy) Q_ASSERT(isHumanPlayer != nullptr);

        emit updateStatus(tr("Partida iniciada<br>"));

        // Obtém o valor máximo da energia
        MAX_POWER = GetWarzoneMaxPowerValue(::detected_warzone_version);

#ifdef QT_DEBUG
        qDebug("Max power for this version: %u\n", MAX_POWER);
#endif

        // Se certifica se que a versão é conhecida
        Q_ASSERT(::detected_warzone_version != WZ_UNKNOWN);

        // Obtém a posição de inicio na estrutura
        WzHack_GetStructStartIndex(::detected_warzone_version, &::start);

        if(bPreventAntiCheatDetection)
        {
            antiCheat = new AntiCheatDetectionThread(::detected_warzone_version);
            antiCheat->preventAntiCheatDetection();
        }

        // Realizar as interferências no jogo
        if(bEnableGodMode)
            this->enableGodMode();

        if(bInfiniteEnergy)
            giveInfiniteEnergy();
        else if(bEraseEnemyEnergy)
            eraseEnemyEnergy();
    } catch(Exception *e)
    {
        fprintf(stderr, "(%s:%d) %s\n", __FILE__, __LINE__, e->what());
        fflush(stderr);

        emit showCriticalMsgBox(tr("Algo deu errado"), e->what());
    }

    // Para a thread do anti-cheat
    if(antiCheat)
        antiCheat->exit(0);

    emit updateStatus(tr("Operações finalizadas"));
}

void MainHackingThread::runHack()
{
    this->run();
}

void MainHackingThread::startLibhack()
{
    hack_handle = libhack_init(WZ_PROCESS_NAME);
    if(!hack_handle)
        throw new Exception(tr("Erro ao inicializar a libhack: %1").arg(GetLastError()).toStdString().c_str());
}

void MainHackingThread::waitForWarzone()
{
    Q_ASSERT(hack_handle != nullptr);

    emit updateStatus(tr("Aguardando pelo warzone ...<br>"));

    // Tenta abrir o warzone
    while(!libhack_open_process(hack_handle))
    {
#ifdef Q_OS_WIN
        if(GetLastError() == ERROR_ACCESS_DENIED)
            throw new Exception(tr("Falha ao abrir processo do warzone: %1").arg(GetLastError()).toStdString().c_str());

        Sleep(5000);
    }
#endif

    // Obtém o endereço-base do processo e guarda na memória
    ::base_addr = libhack_get_base_addr(hack_handle);
    if(!(::base_addr > 0))
        throw new Exception(tr("Falha ao obter endereço-base do processo").toStdString().c_str());

    emit updateStatus("Warzone 2100 em execução ...<br>");
}

void MainHackingThread::wait(uint32_t seconds)
{
    const ushort milisseconds = 100;
    uint32_t time_to_wait = seconds * 1000;
    uint32_t waited = 0;

    while(waited < time_to_wait)
    {
        if(isInterruptionRequested() || !(libhack_process_is_running(::hack_handle)))
            break;

#ifdef Q_OS_WIN
        Sleep(static_cast<DWORD>(milisseconds));
#endif
        waited += milisseconds;
    }
}

void MainHackingThread::giveInfiniteEnergy()
{
    int status;
    DWORD addressToWrite;

    // Calcula o endereço de leitura
    if(bSupportSpecificPlayer)
        addressToWrite = ::base_addr + player_info[::start + ::player_id].power_offset;
    else
        addressToWrite = ::base_addr + player_info[::start].power_offset;

    // Fazer o jogador ter energia infinita
    while(!this->isInterruptionRequested() && libhack_process_is_running(::hack_handle))
    {

        // Aumenta a energia do jogador selecionado para o valor máximo
        status = libhack_write_int_to_addr(hack_handle, addressToWrite, ::MAX_POWER);

        if(status <= 0)
            throw new Exception(tr("Falha ao oferecer energia ao jogador %1").arg(::player_id).toStdString().c_str());

        // Aguarda pelo número de segundos desejado ou até que uma interrupção seja solicitada
        wait(::hacking_delay);
    }
}

void MainHackingThread::eraseEnemyEnergy()
{
    unsigned long addrToWrite;

    // Certifica-se de que podemos acessar a função
    Q_ASSERT(isHumanPlayer);

    // Calcula o endereço de leitura
    addrToWrite = ::base_addr + ::player_info[::start].power_offset;

    while(!this->isInterruptionRequested() && libhack_process_is_running(::hack_handle))
    {
        // FIXME: encontrar método para não zerar a energia dos jogadores amigos
        for(int player = 0; player < MAX_PLAYERS; player++)
        {
            // Não zerar a energia do jogador favorecido
            if(bSupportSpecificPlayer && (::player_id == player))
                continue;

            if(!isHumanPlayer(player))
                libhack_write_int_to_addr(hack_handle, addrToWrite, 0);
        }

        // Aguarda pelo número de segundos desejado ou até que uma interrupção seja solicitada
        wait(::hacking_delay);
    }
}

bool MainHackingThread::isGamePlayStarted() const
{
    unsigned long addressToRead = 0;
    int started;

    if(IsFeatureSupported(::detected_warzone_version, FEATURE_CHECK_GAME_IS_RUNNING))
    {
        for(const auto& status : gstatus)
        {
            if(status.warzone_version == ::detected_warzone_version)
            {
                addressToRead = ::base_addr + status.grpInitialized_offset;
                break;
            }
        }

        if(addressToRead) {
            started = libhack_read_int_from_addr(::hack_handle, addressToRead);
            return started == 1;
        }
    }

    return false;
}

void MainHackingThread::enableGodMode()
{
    unsigned long addrToWrite = 0;
    Q_ASSERT(hack_handle != nullptr);

    // Calcula o endereço para escrita
    for(const auto& player : player_info)
    {
        if(player.warzone_version != ::detected_warzone_version)
            continue;

        addrToWrite = ::base_addr + player.godMode_offset;
        break;
    }

    if(addrToWrite)
        libhack_write_int_to_addr(::hack_handle, addrToWrite, 1);
}

AntiCheatDetectionThread::AntiCheatDetectionThread(unsigned short warzone_version)
{
    Q_ASSERT(hack_handle != nullptr);
    this->wz_version = warzone_version;
}

void AntiCheatDetectionThread::preventAntiCheatDetection()
{
    this->run();
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
    DWORD addrToWrite;

    Q_ASSERT(index >= 0);

    // Calcula o endereço de escrita
    addrToWrite = ::base_addr + ::gstatus[index].cheated_offset;

    try
    {
        while(!this->isInterruptionRequested() && libhack_process_is_running(::hack_handle))
        {
            libhack_write_int_to_addr(::hack_handle, addrToWrite, 0);

            // Atualiza o valor a cada 5 segundos
            this->wait(5);
        }
    } catch(Exception *e)
    {
        fprintf(stderr, "(%s:%d) %s\n", __FILE__, __LINE__, e->what());
        fflush(stderr);
    }
}

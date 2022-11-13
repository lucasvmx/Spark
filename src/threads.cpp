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
#include "exception.h"
#include "dynamic_loader.h"
#include <QMessageBox>
#include <QtGlobal>
#ifdef Q_OS_WIN
#include <windows.h>
#endif
#if defined(_DEBUG) || defined(QT_DEBUG)
#include <QDebug>
#endif

using namespace Threads;

static QMutex mux;

/**
 * @brief start Posição inicial na struct, de acordo com a versão do warzone atualmente em execução
 */
unsigned short start = 0;

#ifndef Q_OS_WIN64
/**
 * @brief base_addr Endereço base do processo na memória
 */
DWORD base_addr;
#else
/**
 * @brief base_addr Endereço base do processo na memória (64 bits)
 */
DWORD64 base_addr;
#endif

/**
 * @brief warzone_version Versão do warzone
 */
short detected_warzone_version;


MainHackingThread::MainHackingThread(QSharedPointer<GameSettings> cfg)
{
    ::base_addr = 0;
    config = cfg.get();

    player_id = config->getConfigValue(S_PLAYER_ID).toInt();
}

void MainHackingThread::run()
{
    char warzone_path[BUFLEN];
    DWORD warzone_path_len = arraySize(warzone_path);
    AntiCheatDetectionThread *antiCheat = nullptr;

#ifdef Q_OS_WIN
    RtlSecureZeroMemory(warzone_path, sizeof(warzone_path));
#endif

    try {

        // Inicializa a libhack
        this->startLibhack();

        // Verifica se o warzone já está aberto. Se não estiver, aguarda pela sua abertura
        this->waitForWarzone();

        // Obtém o caminho completo do executável do warzone 2100
        if(!fQueryFullProcessImageFileName(hack_handle.get()->hProcess, 0, warzone_path, &warzone_path_len))
            throw new Exception(tr("Erro ao obter caminho do warzone: %1").arg(GetLastError()).toStdString().c_str());

        // Verifica qual versão do warzone está em execução
        ::detected_warzone_version = WzHack_GetWarzoneVersion(warzone_path);

        emit updateStatus(tr("Aguardando usuário entrar na partida ..."));

        // Aguarda o usuário entrar na partida
        while(!this->isGamePlayStarted())
            this->wait(1);

        // Verifica se a versão suporta as operações solicitadas
        if(!performFeaturesChecking()) {
            throw new Exception("Alguns recursos estão indisponíveis para essa versão do Warzone 2100");
        }

        emit updateStatus(tr("Partida iniciada"));

        // Obtém o valor máximo da energia
        max_power = GetWarzoneMaxPowerValue(::detected_warzone_version);

        // Se certifica se que a versão é conhecida
        Q_ASSERT(::detected_warzone_version != WZ_UNKNOWN);

        // Obtém a posição de inicio na estrutura
        WzHack_GetStructStartIndex(::detected_warzone_version, &::start);

        if(config->getConfigValue(S_PREVENT_ANTICHEAT).toBool())
        {
            mux.lock();
            antiCheat = new AntiCheatDetectionThread(::detected_warzone_version, hack_handle.get());
            mux.unlock();
            antiCheat->start();
        }

        // Realizar as interferências no jogo
        if(config->getConfigValue(S_ENABLE_GOD_MODE).toBool()) {
            this->enableGodMode();
        }

        auto delay = config->getConfigValue(S_HACK_DELAY).toUInt();

        emit updateStatus(tr("Aplicando algoritmos de hacking ..."));

		// Continua as alterações no jogo até que ele seja fechado
		while(this->canContinue())
		{
			if(this->isGamePlayRunning()) {
                if(config->getConfigValue(S_INFINITE_ENERGY).toBool()) {
                    giveInfiniteEnergy();
                }
                else if(config->getConfigValue(S_ERASE_ENEMY_POWER).toBool()) {
                    eraseEnemyEnergy();
                } else if(config->getConfigValue(S_INCREASE_POWER_GEN_SPEED).toBool()) {
                    increasePowerGenerationSpeed(5000);
                }

                if(config->getConfigValue(S_INCREASE_PLAYER_DAMAGE).toBool()) {
                    increaseDamageOutput();
                }
			}

			// Aguarda pelo número de segundos desejado ou até que uma interrupção seja solicitada
            wait(delay);
		}

    } catch(Exception *e)
    {
        fprintf(stderr, "(%s:%d) %s\n", __FILE__, __LINE__, e->what());
        fflush(stderr);

        emit showCriticalMsgBox(tr("Algo deu errado"), e->what());
    }

    // Para a thread do anti-cheat
    if(antiCheat)
        antiCheat->exit(0);

    emit updateStatus(tr("O Warzone 2100 foi fechado"));
	emit updateStatus(tr("Clique novamente no botão iniciar para rodar o trainer"));
}

void MainHackingThread::startLibhack()
{
    hack_handle = QSharedPointer<libhack_handle>(libhack_init(WZ_PROCESS_NAME));
    if(!hack_handle)
        throw new Exception(tr("Erro ao inicializar a libhack: %1").arg(GetLastError()).toStdString().c_str());
}

bool MainHackingThread::canContinue() const
{
    return ((libhack_process_is_running(hack_handle.get())) && !(this->isInterruptionRequested()));
}

void MainHackingThread::waitForWarzone()
{
    Q_ASSERT(hack_handle.get() != nullptr);

    emit updateStatus(tr("Aguardando pelo warzone ..."));

    // Tenta abrir o warzone
    while(1)
    {
        bool opened;

        mux.lock();
        opened = libhack_open_process(hack_handle.get());
        mux.unlock();

        if(opened) {
            // Aguarda 5 segundos antes de obter o endereço-base. Isso evita que ocorra algum erro na hora
            // de obter o endereço-base do processo
            Sleep(5000);
            break;
        }

#ifdef Q_OS_WIN
        if(GetLastError() == ERROR_ACCESS_DENIED)
            throw new Exception(tr("Falha ao abrir processo do warzone: %1").arg(GetLastError()).toStdString().c_str());

        Sleep(5000);
#endif
    }

    // Obtém o endereço-base do processo e guarda na memória
    mux.lock();
#ifndef Q_OS_WIN64
    ::base_addr = libhack_get_base_addr(hack_handle.get());
#else
    ::base_addr = libhack_get_base_addr64(hack_handle.get());
#endif
    mux.unlock();
    if(!(::base_addr > 0))
        throw new Exception(tr("Falha ao obter endereço-base do processo").toStdString().c_str());

#ifdef QT_DEBUG
#ifdef Q_OS_WIN64
    fprintf(stderr, "Endereço-base: %llx\n", ::base_addr);
#else
    fprintf(stderr, "Endereço-base: %#x\n", ::base_addr);
#endif
#endif

    emit updateStatus(tr("Warzone 2100 em execução ..."));
}

void MainHackingThread::wait(uint32_t seconds)
{
    const ushort milisseconds = 100;
    uint32_t time_to_wait = seconds * 1000;
    uint32_t waited = 0;

    while(waited < time_to_wait)
    {
        if(!(this->canContinue()))
            break;

#ifdef Q_OS_WIN
        Sleep(static_cast<DWORD>(milisseconds));
#else
		usleep(milisseconds * milisseconds);
#endif
        waited += milisseconds;
    }
}

void MainHackingThread::giveInfiniteEnergy()
{
    int status;
#ifndef Q_OS_WIN64
    DWORD addressToWrite;
#else
    DWORD64 addressToWrite;
#endif

    // Calcula o endereço de leitura
    if(config->getConfigValue(S_SUPPORT_SPECIFIC_PLAYER).toBool()) {
        addressToWrite = ::base_addr + player_info[::start + player_id].power_offset;
    } else {
        addressToWrite = ::base_addr + player_info[::start].power_offset;
    }

    // Fazer o jogador ter energia infinita

	// Aumenta a energia do jogador selecionado para o valor máximo
    mux.lock();
#ifndef Q_OS_WIN64
    status = libhack_write_int_to_addr(hack_handle.get(), addressToWrite, max_power);
#else
    status = libhack_write_int_to_addr64(hack_handle.get(), addressToWrite, max_power);
#endif
    mux.unlock();

	if(status <= 0)
        throw new Exception(tr("Falha ao oferecer energia ao jogador %1").arg(player_id).toStdString().c_str());
}

void MainHackingThread::eraseEnemyEnergy()
{
#ifndef Q_OS_WIN64
    unsigned long addrToWrite;
#else
    DWORD64 addrToWrite;
#endif

	// FIXME: encontrar método para não zerar a energia dos jogadores amigos
	for(int player = 0; player < MAX_PLAYERS; player++)
	{
		// Não zerar a energia do jogador favorecido
        if(config->getConfigValue(S_SUPPORT_SPECIFIC_PLAYER).toBool() ||
                (this->player_id == player)) {
            continue;
        }


        // Calcula o endereço de escrita
        addrToWrite = ::base_addr + ::player_info[::start + player].power_offset;

        mux.lock();
#ifndef Q_OS_WIN64
        libhack_write_int_to_addr(hack_handle.get(), addrToWrite, 0);
#else
        libhack_write_int_to_addr64(hack_handle.get(), addrToWrite, 0);
#endif
        mux.unlock();
	}
}

bool MainHackingThread::isGamePlayStarted() const
{
#ifndef Q_OS_WIN64
    unsigned long addressToRead = 0;
#else
    unsigned long long addressToRead = 0;
#endif
    int started;

    mux.lock();
    if(!libhack_process_is_running(hack_handle.get())) {
        mux.unlock();
        throw new Exception(tr("O warzone 2100 foi fechado subitamente").toStdString().c_str());
    }
    mux.unlock();

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
            mux.lock();
#ifndef Q_OS_WIN64
            started = libhack_read_int_from_addr(hack_handle.get(), addressToRead);
#else
            started = libhack_read_int_from_addr64(hack_handle.get(), addressToRead);
#endif
            mux.unlock();
            return started == 1;
        }
    } else {
        throw new Exception(tr("Essa versão do warzone ainda não suporta a detecção de status "
                            "(verificar se a partida foi iniciada). Envie um e-mail para "
                            "lucas.engen.cc@gmail.com contendo a versão do jogo e reporte o ocorrido :)").toStdString().c_str());
    }

    return false;
}

bool MainHackingThread::isGamePlayRunning() const
{
	return this->isGamePlayStarted();
}

void MainHackingThread::enableGodMode()
{
#ifdef Q_OS_WIN64
    DWORD64 addrToWrite = 0;
#else
    DWORD addrToWrite = 0;
#endif
    int god_mode;
    int bytes_written = 0;

    mux.lock();
    Q_ASSERT(hack_handle.get() != nullptr);
    mux.unlock();

    // Calcula o endereço para escrita
    for(const auto& player : player_info)
    {
        if(player.warzone_version != ::detected_warzone_version)
            continue;

        addrToWrite = ::base_addr + player.godMode_offset;
        break;
    }

    if(addrToWrite)
    {
        do
        {
            qDebug("god mode address: %llx", addrToWrite);

#ifndef Q_OS_WIN64
            mux.lock();
            bytes_written = libhack_write_int_to_addr(hack_handle.get(), addrToWrite, 1);
            if(bytes_written <= 0) {
                mux.unlock();
                fprintf(stderr, "(%s:%d) erro ao habilitar o god mode: %lu\n", __FILE__, __LINE__, GetLastError());
                break;
            }

            // Certifica-se de que o modo Deus está habilitado
            mux.lock();
            god_mode = libhack_read_int_from_addr(hack_handle.get(), addrToWrite);
            mux.unlock();
#else

            mux.lock();
            bytes_written = libhack_write_int_to_addr64(hack_handle.get(), addrToWrite, 1);
            mux.unlock();
            if(bytes_written <= 0) {
                fprintf(stderr, "(%s:%d) erro ao habilitar o god mode: %lu\n", __FILE__, __LINE__, GetLastError());
                break;
            }

            // Certifica-se de que o modo Deus está habilitado
            mux.lock();
            god_mode = libhack_read_int_from_addr64(hack_handle.get(), addrToWrite);
            mux.unlock();
#endif

            mux.lock();
            if((god_mode > 0) || (this->isInterruptionRequested()) || !(libhack_process_is_running(hack_handle.get()))) {
                mux.unlock();
                break;
            }

            mux.unlock();

        } while(1);
    }
}

void MainHackingThread::increasePowerGenerationSpeed(unsigned new_modifier)
{
#ifndef Q_OS_WIN64
    DWORD powerModifierAddr = 0;
#else
    DWORD64 powerModifierAddr = 0;
#endif

    for(const auto& p : player_info) {
        if(p.warzone_version == ::detected_warzone_version) {
            if(p.warzone_version >= WZ_431) {
                powerModifierAddr = ::base_addr + p.power_modifier_offset;
            } else {
                powerModifierAddr = ::base_addr + p.power_modifier_offset + 0x20;
            }
            break;
        }
    }


    // bloqueia o mutex global, pois vai acessar o ponteiro hack_handle
    mux.lock();

#ifdef Q_OS_WIN64
    libhack_write_int_to_addr64(hack_handle.get(), static_cast<DWORD64>(powerModifierAddr), new_modifier);
#else
    libhack_write_int_to_addr(hack_handle.get(), static_cast<DWORD>(powerModifierAddr), new_modifier);
#endif

    mux.unlock();
}

void MainHackingThread::increaseDamageOutput(unsigned new_damage)
{

#ifndef Q_OS_WIN64
    DWORD damageModifierAddr = 0;
#else
    DWORD64 damageModifierAddr = 0;
#endif

    // calcula o endereço de escrita (isso só pode ser feito depois que a partida foi iniciada
    damageModifierAddr = ::base_addr + player_info[::start + player_id].damage_modifier_offset;

    qDebug("damage output address: %llx", damageModifierAddr);

    // bloqueia o mutex global antes de acessar o ponteiro
    mux.lock();
#ifdef Q_OS_WIN64
    libhack_write_int_to_addr64(this->hack_handle.get(), damageModifierAddr, new_damage);
#else
    libhack_write_int_to_addr(this->hack_handle.get(), damageModifierAddr, new_damage);
#endif
    mux.unlock();
}

bool MainHackingThread::performFeaturesChecking()
{
    unsigned notSupported = 0;

    // Verifica se a versão suporta as operações solicitadas
    if(config->getConfigValue(S_ENABLE_GOD_MODE).toBool() && !IsFeatureSupported(detected_warzone_version, FEATURE_GOD_MODE)) {
        notSupported++;
    }

    if((config->getConfigValue(S_INFINITE_ENERGY).toBool() || config->getConfigValue(S_ERASE_ENEMY_POWER).toBool()) &&
             !IsFeatureSupported(detected_warzone_version, FEATURE_CHANGE_POWER)) {
        qDebug() << "change power not supported";
        notSupported++;
    }

    // Verifica se a feature de aumentar dano é suportada
    if(config->getConfigValue(S_INCREASE_PLAYER_DAMAGE).toBool() &&
            !IsFeatureSupported(detected_warzone_version, FEATURE_INCREASE_PLAYER_DAMAGE)) {
        notSupported++;
    }

    qDebug() << "features not supported: " << notSupported;

    return notSupported == 0;
}

AntiCheatDetectionThread::AntiCheatDetectionThread(unsigned short warzone_version, libhack_handle *handle)
{
    this->wz_version = warzone_version;
    hack_handle = QSharedPointer<libhack_handle>(handle);
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
#ifdef Q_OS_WIN64
    DWORD64 addrToWrite;
#else
    DWORD addrToWrite;
#endif

    Q_ASSERT(index >= 0);

    // Calcula o endereço de escrita
    addrToWrite = ::base_addr + ::gstatus[index].cheated_offset;

    qDebug("cheated offset: %llx", addrToWrite);

    try
    {
        while(1)
        {
            mux.lock();
            if(this->isInterruptionRequested() || !libhack_process_is_running(hack_handle.get())) {
                mux.unlock();
                break;
            }

#ifdef Q_OS_WIN64
            libhack_write_int_to_addr64(hack_handle.get(), addrToWrite, 0);
#else
            libhack_write_int_to_addr(hack_handle.get(), addrToWrite, 0);
#endif
            mux.unlock();

            // Atualiza o valor a cada 5 segundos
            this->wait(5);
        }
    } catch(Exception *e)
    {
        fprintf(stderr, "(%s:%d) %s\n", __FILE__, __LINE__, e->what());
        fflush(stderr);
    }
}

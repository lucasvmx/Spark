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

#include <QThread>

namespace Threads
{
    class MainHackingThread : public QThread
    {
        Q_OBJECT

    public:
        MainHackingThread();
        void runHack();

    private:
        void run();
        void startLibhack();
        void waitForWarzone();
        void wait(uint32_t seconds);
        void giveInfiniteEnergy();
        void eraseEnemyEnergy();

    signals:
        void updateStatus(QString status_text);
    };

    class AntiCheatDetectionThread: public QThread
    {
        Q_OBJECT

    public:
        AntiCheatDetectionThread(unsigned short warzone_version);
        void preventAntiCheatDetection();

    private:
        unsigned short wz_version;
        int getGameStatusIndex();
        void wait(uint32_t seconds);
        void run();
    };
}



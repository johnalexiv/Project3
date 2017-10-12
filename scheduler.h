//
//  scheduler.h
//  Project3
//
//  Created by John Alexander on 10/5/17.
//  Copyright © 2017 John Alexander. All rights reserved.
//

#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <iostream>
#include <queue>
#include <vector>
#include "process.h"
#include "queuetypes.h"

class Scheduler
{
public:
    Scheduler(std::vector<Process> processes);
    ~Scheduler();

    void runProcesses();

private:
    void initializeScheduler(std::vector<Process>);

    Process startTop();
    void startPop();
    void startPush(Process);
    bool isStartEmpty();

    Process activeTop();
    void activePop();
    void activePush(Process);
    bool isActiveEmpty();

    Process expiredTop();
    void expiredPop();
    void expiredPush(Process);
    bool isExpiredEmpty();

    Process ioTop();
    void ioPop();
    void ioPush(Process);
    bool isIoEmpty();

    Process finishedTop();
    void finishedPop();
    void finishedPush(Process);
    bool isFinishedEmpty();

    Process cpuTop();
    void cpuPop();
    void cpuPush(Process);
    bool isCpuEmpty();

    int getClock();
    void resetClock();
    void incrementClock();

    void checkArrivingProcesses();
    void checkPreemptRequired();
    void assignLowestPriorityProcessToCpu();
    void updateIoBursts();
    void updateCpuTimeSlice();

private:
    StartQueue          * _startQueue;
    ActiveExpiredQueue  * _activeQueue;
    ActiveExpiredQueue  * _expiredQueue;
    IOQueue             * _ioQueue;
    FinishedQueue       * _finishedQueue;
    CPU                 * _cpu;

    int                 _clock;
};

#endif // _SCHEDULER_H







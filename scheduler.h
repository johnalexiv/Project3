//
//  scheduler.h
//  Project3
//
//  Created by John Alexander on 10/5/17.
//  Copyright © 2017 John Alexander. All rights reserved.
//

#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <queue>
#include <vector>
#include "process.h"
#include "queuetypes.h"

class Scheduler
{
public:
    Scheduler(std::vector<Process> processes);
    ~Scheduler();
    
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

    void decrementTimeSlices();

private:
    void initializeScheduler(std::vector<Process>);
    void decrementIoTimeSlices();
    void decrementCpuTimeSlice();

private:
    StartQueue      * _startQueue;
    ActiveQueue     * _activeQueue;
    ExpiredQueue    * _expiredQueue;
    IOQueue         * _ioQueue;
    FinishedQueue   * _finishedQueue;
    CPU             * _cpu;

    int             _clock;
};

#endif // _SCHEDULER_H







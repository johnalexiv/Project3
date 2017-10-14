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
#include <iomanip>
#include <queue>
#include <vector>
#include <string>
#include "process.h"
#include "queuetypes.h"

class Scheduler
{
public:
    Scheduler(std::vector<Process> processes);
    ~Scheduler();

    void runProcesses();
    void printStatistics();

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

    void checkProcessFinishedCpu();
    void checkProcessesFinishedIO();

    void checkArrivingProcesses();
    void checkPreemptRequired();
    void assignLowestPriorityProcessToCpu();
    void decrementIoBursts();
    void updateCpuBurstAndTimeSlice();
    bool allQueuesEmpty();
    void swapQueuesIfRequired();
    void swapActiveAndExpiredQueues();

    void printMessage(int, int);
    void printMessage(int, Process, int);
    void printMessage(int, Process, Process, int);
    void printProcessStatistics(Process);
    void printAverages(std::vector<Process>);

    double round(double);

private:
    StartQueue          * _startQueue;
    ActiveExpiredQueue  * _activeQueue;
    ActiveExpiredQueue  * _expiredQueue;
    IOQueue             * _ioQueue;
    FinishedQueue       * _finishedQueue;
    CPU                 * _cpu;

    int                 _clock;

    enum PrintOption
    {
        TO_ACTIVE,
        TO_CPU,
        PREEMPT,
        FINISHED,
        TO_IO,
        TO_EXPIRED,
        IO_TO_EXPIRED,
        IO_TO_ACTIVE,
        SWAPPED
    };
};

#endif // _SCHEDULER_H







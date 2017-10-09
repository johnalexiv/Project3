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

struct compareStartQueue  
{  
    bool operator()(Process first, Process second)  
    {  
        if ( first.getArrivalTime() == second.getArrivalTime() )
            return first.getPID() > second.getPID();
        else
            return first.getArrivalTime() > second.getArrivalTime();  
    }  
};

struct compareActiveExpiredQueue  
{  
    bool operator()(Process first, Process second)  
    {  
        if ( first.getPriority() == second.getPriority() )
            return first.getPID() > second.getPID();
        else
            return first.getPriority() > second.getPriority();  
    }
};

struct compareIOQueue  
{  
    bool operator()(Process first, Process second)  
    {  
        if ( first.getCurrentIoBurst() == second.getCurrentIoBurst() )
            return first.getPID() > second.getPID();
        else
            return first.getCurrentIoBurst() > second.getCurrentIoBurst();  
    }
};

struct compareFinishedQueue  
{  
    bool operator()(Process first, Process second)  
    {
        return first.getEndTime() > second.getEndTime();  
    }
};

typedef std::priority_queue< Process, std::vector<Process>, compareStartQueue >             StartQueue;
typedef std::priority_queue< Process, std::vector<Process>, compareActiveExpiredQueue >     ActiveQueue;
typedef std::priority_queue< Process, std::vector<Process>, compareActiveExpiredQueue >     ExpiredQueue;
typedef std::priority_queue< Process, std::vector<Process>, compareIOQueue >                IOQueue;
typedef std::priority_queue< Process, std::vector<Process>, compareFinishedQueue >          FinishedQueue;
typedef std::queue<Process>                                                                 CPU;

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

private:
    void initializeScheduler(std::vector<Process>);

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







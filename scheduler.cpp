//
//  scheduler.cpp
//  Project3
//
//  Created by John Alexander on 10/5/17.
//  Copyright © 2017 John Alexander. All rights reserved.
//

#include "scheduler.h"

Scheduler::Scheduler(std::vector<Process> processes)
{
    initializeScheduler(processes);
}

Scheduler::~Scheduler()
{
    delete _startQueue;
    delete _activeQueue;
    delete _expiredQueue;
    delete _ioQueue;
    delete _finishedQueue;
    delete _cpu;
}

void Scheduler::initializeScheduler(std::vector<Process> processes)
{
    _startQueue = new StartQueue;
    _activeQueue = new ActiveQueue;
    _expiredQueue = new ExpiredQueue;
    _ioQueue = new IOQueue;
    _finishedQueue = new FinishedQueue;
    _cpu = new CPU;
    _clock = 0;

    int i = 0;
    while( i < processes.size() )
        startPush(processes[i++]);
}

// Start
Process Scheduler::startTop()
{
    if ( !isStartEmpty() )
        return _startQueue->top();
}

void Scheduler::startPop()
{
    _startQueue->pop();
}

void Scheduler::startPush(Process process)
{
    _startQueue->push(process);
}

bool Scheduler::isStartEmpty()
{
    return _startQueue->empty();
}

// Active
Process Scheduler::activeTop()
{
    if ( !isActiveEmpty() )
        return _activeQueue->top();
}

void Scheduler::activePop()
{
    _activeQueue->pop();
}

void Scheduler::activePush(Process process)
{
    _activeQueue->push(process);
}

bool Scheduler::isActiveEmpty()
{
    return _activeQueue->empty();
}

// Expired
Process Scheduler::expiredTop()
{
    if ( !isExpiredEmpty() )
        return _expiredQueue->top();
}

void Scheduler::expiredPop()
{
    _expiredQueue->pop();
}

void Scheduler::expiredPush(Process process)
{
    _expiredQueue->push(process);
}

bool Scheduler::isExpiredEmpty()
{
    return _expiredQueue->empty();
}

// IO
Process Scheduler::ioTop()
{
    if ( !isIoEmpty() )
        return _ioQueue->top();
}

void Scheduler::ioPop()
{
    _ioQueue->pop();
}

void Scheduler::ioPush(Process process)
{
    _ioQueue->push(process);
}

bool Scheduler::isIoEmpty()
{
    return _ioQueue->empty();
}

// Finished
Process Scheduler::finishedTop()
{
    if ( !isFinishedEmpty() )
        return _finishedQueue->top();
}

void Scheduler::finishedPop()
{
    _finishedQueue->pop();
}

void Scheduler::finishedPush(Process process)
{
    _finishedQueue->push(process);
}

bool Scheduler::isFinishedEmpty()
{
    return _finishedQueue->empty();
}

// CPU
Process Scheduler::cpuTop()
{
    if ( !isCpuEmpty() )
        return _cpu->front();
}

void Scheduler::cpuPop()
{
    _cpu->pop();
}

void Scheduler::cpuPush(Process process)
{
    _cpu->push(process);
}

bool Scheduler::isCpuEmpty()
{
    return _cpu->empty();
}

// Clock
int Scheduler::getClock()
{
    return _clock;
}

void Scheduler::resetClock()
{
    _clock = 0;
}

void Scheduler::incrementClock()
{
    _clock++;
}

void Scheduler::decrementTimeSlices()
{
    decrementIoTimeSlices();
    decrementCpuTimeSlice();
}

void Scheduler::decrementIoTimeSlices()
{
    int i = 0;
    std::vector<Process> processes;
    while ( !isIoEmpty() )
    {
        processes[i++] = ioTop();
        ioPop(); 
    }
    for( i = 0; i < processes.size(); i++ )
    {
        processes[i].decrementTimeSlice();
        ioPush(processes[i]);
    }
}

void Scheduler::decrementCpuTimeSlice()
{
    if ( !isCpuEmpty() )
    {
        Process process = cpuTop();
        cpuPop();
        process.decrementTimeSlice();
        cpuPush(process);
    }
}







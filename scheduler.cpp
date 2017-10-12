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
    _activeQueue = new ActiveExpiredQueue;
    _expiredQueue = new ActiveExpiredQueue;
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

void Scheduler::runProcesses()
{   
    while ( getClock() < 1000 )
    {
        // insert process into active queue if arriveTime == clock
        checkArrivingProcesses();

        // check if CPU is empty and put lowest priority process 
        // from active queue in CPU. use FIFO if two or more processes
        // have the same priority
        assignLowestPriorityProcessToCpu();

        // check if active queue has lower priority process,
        // if so, prempt process and put it back in active queue
        // and save its time slice
        checkPreemptRequired();

        // decrement the timeslice of the process in the CPU
        updateCpuTimeSlice();

        // decrement the timeslice of IO burst for all IO processes
        updateIoBursts();

        // if there is a process in cpu
        if ( !isCpuEmpty() )
        {
            Process cpuProcess = cpuTop();
            //std::cout << "Clock: " << getClock() << " , Timeslice: " << cpuProcess.getTimeSlice() << std::endl;
        }
            // if process in cpu is done with cpu burst
                
                // if process is done with all cpu bursts send to finished
                
                // if process is not done with all cpu bursts, send to IO
            
            // if process has used all timeslice send to expired queue
            // and recalculate priority and timeslice
        
        // if there are any process in IO queue that finished with IO
           
            // if IO process timeslice is done, move process to expired and recalc.

            // if process still has timeslice then more to active queue

        // if all queues are empty then break, simulation is done

        // if ready/cpu are empty and expired is not empty
        // then swap ready and expired queues

        // increment clock

        // if ( !isActiveEmpty() )
        // {
        //     std::cout << "PID: " << activeTop().getPID() << std::endl; 
        //     std::cout << "Arrival Time: " << activeTop().getArrivalTime() << std::endl;
        //     std::cout << "Priority: " << activeTop().getPriority() << std::endl; 
        //     std::cout << "Time Slice: " << activeTop().getTimeSlice() << std::endl; 
        //     activePop();
        // }

        incrementClock();
    }
}

void Scheduler::checkArrivingProcesses()
{
    if ( isStartEmpty() )
        return;

    Process currentProcess = startTop();
    while ( currentProcess.getArrivalTime() == getClock() )
    {
        activePush(currentProcess);
        startPop();
        std::cout << "[" << getClock() << "] ";
        std::cout << "<" << currentProcess.getPID() << "> ";
        std::cout << "Enters ready queue (Priority: " << currentProcess.getPriority();
        std::cout << ", Timeslice: " << currentProcess.getTimeSlice() << ")" << std::endl;
        if ( isStartEmpty() )
            break;

        currentProcess = startTop();
    }
}

void Scheduler::assignLowestPriorityProcessToCpu()
{
    if ( isCpuEmpty() && !isActiveEmpty() )
    {
        Process process = activeTop();
        cpuPush(process);
        activePop();
    }
}

void Scheduler::checkPreemptRequired()
{
    if ( !isActiveEmpty() && !isCpuEmpty() )
    {
        Process activeProcess = activeTop();
        Process cpuProcess = cpuTop();
        if ( activeProcess.getPriority() < cpuProcess.getPriority() )
        {
            cpuPop();
            activePop();
            activePush(cpuProcess);
            cpuPush(activeProcess);
        }
    }
}

void Scheduler::updateCpuTimeSlice()
{
    if ( !isCpuEmpty() )
    {
        Process process = cpuTop();
        cpuPop();
        process.decrementTimeSlice();
        cpuPush(process);
    }
}

void Scheduler::updateIoBursts()
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








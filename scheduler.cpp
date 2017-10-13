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
    while ( true )
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
        updateCpuBurstAndTimeSlice();

        // decrement the timeslice of IO burst for all IO processes
        updateIoBursts();

        // if there is a process in cpu
        checkProcessFinishedCpu();

            // if process in cpu is done with cpu burst
                
                // if process is done with all cpu bursts send to finished
                
                // if process is not done with all cpu bursts, send to IO
            
            // if process has used all timeslice send to expired queue
            // and recalculate priority and timeslice
        
        // if there are any process in IO queue that finished with IO
        checkProcessesFinishedIO();

            // if IO process timeslice is done, move process to expired and recalc.

            // if process still has timeslice then more to active queue

        // if all queues are empty then break, simulation is done
        if ( allQueuesEmpty() )
            break;

        // if ready/cpu are empty and expired is not empty
        // then swap ready and expired queues
        swapQueuesIfRequired();

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
        printMessage(TO_ACTIVE, currentProcess);

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

        printMessage(TO_CPU, process);
    }
}

void Scheduler::checkPreemptRequired()
{
    if ( !isActiveEmpty() && !isCpuEmpty() )
    {
        Process activeProcess = activeTop();
        Process cpuProcess = cpuTop();
        if ( activeProcess.getDynamicPriority() < cpuProcess.getDynamicPriority() )
        {
            cpuPop();
            activePop();
            activePush(cpuProcess);
            cpuPush(activeProcess);

            printMessage(PREEMPT, activeProcess, cpuProcess);
        }
    }
}

void Scheduler::updateCpuBurstAndTimeSlice()
{
    if ( !isCpuEmpty() )
    {
        Process process = cpuTop();
        cpuPop();
        process.updateCpuBurstAndTimeslice();
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
        processes[i].updateIoBurst();
        ioPush(processes[i]);
    }
}

void Scheduler::checkProcessFinishedCpu()
{
    if ( !isCpuEmpty() )
    {
        Process cpuProcess = cpuTop();
        cpuPop();

        if ( cpuProcess.cpuFlagSet() )
        {
            cpuProcess.setCpuFlag(false);

            if ( cpuProcess.isFinished() )
            {
                finishedPush(cpuProcess);
                printMessage(FINISHED, cpuProcess);
            }
            else  
            {
                ioPush(cpuProcess);
                printMessage(TO_IO, cpuProcess);
            }
        }
        else if ( cpuProcess.getTimeSlice() == 0 )
        {
            cpuProcess.newTimeSlice();
            expiredPush(cpuProcess);
            printMessage(TO_EXPIRED, cpuProcess);
        }
        else
        {
            cpuPush(cpuProcess);
        }
    }  
}

void Scheduler::checkProcessesFinishedIO()
{
    int i = 0;
    std::vector<Process> ioProcesses;
    while ( !isIoEmpty() )
    {
        Process process = ioTop();
        ioPop(); 

        if ( process.ioFlagSet() )
        {
            process.setIoFlag(false);

            if ( process.getTimeSlice() == 0 )
            {
                process.newTimeSlice();
                expiredPush(process);
                printMessage(IO_TO_EXPIRED, process);
            }
            else
            {
                activePush(process);
                printMessage(IO_TO_ACTIVE, process);
            }
        }
        else
            ioProcesses[i++] = process;
    }

    for( i = 0; i < ioProcesses.size(); i++ )
        ioPush(ioProcesses[i]);
}

bool Scheduler::allQueuesEmpty()
{
    bool allEmpty = true;
    if ( !isCpuEmpty() || 
         !isActiveEmpty() || 
         !isExpiredEmpty() || 
         !isIoEmpty() ||
         !isStartEmpty() )
        allEmpty = false;
    
    return allEmpty;
}

void Scheduler::swapQueuesIfRequired()
{
    if ( isCpuEmpty() && isActiveEmpty() )
        if ( !isExpiredEmpty() )
        {
            swapActiveAndExpiredQueues();
            printMessage(SWAPPED);
        }
}

void Scheduler::swapActiveAndExpiredQueues()
{
    ActiveExpiredQueue temp = *_activeQueue;
    *_activeQueue = *_expiredQueue;
    *_expiredQueue = temp;
}

void Scheduler::printMessage(int messageType)
{
    std::string message;
    switch( messageType )
    {
        case SWAPPED:
            message = "[" + std::to_string(getClock()) + "] *** Queue Swap";
            break;
        default:
            message = "something went wrong.";
            break;
    }
    std::cout << message << std::endl;
}

void Scheduler::printMessage(int messageType, Process p1, Process p2)
{
    std::string message;
    message = "[" + std::to_string(getClock()) + "] ";
    message += "<" + std::to_string(p1.getPID()) + "> ";
    switch( messageType )
    {
        case PREEMPT:
            message += "Preempts Process " + std::to_string(p2.getPID());
            break;
        default:
            message = "something went wrong.";
            break;
    }
    std::cout << message << std::endl;
}

void Scheduler::printMessage(int messageType, Process p1)
{
    std::string message;
    message = "[" + std::to_string(getClock()) + "] ";
    message += "<" + std::to_string(p1.getPID()) + "> ";
    switch( messageType )
    {
        case TO_ACTIVE:
            message += "Enters ready queue (Priority: " + std::to_string(p1.getDynamicPriority());
            message += ", Timeslice: " + std::to_string(p1.getTimeSlice()) + ")";
            break;
        case TO_CPU:
            message += "Enters the CPU";
            break;
        case FINISHED:
            message += "Finishes and moves to the Finished Queue";
            break;
        case TO_IO:
            message += "Moves to the IO Queue";
            break;
        case TO_EXPIRED:
            message += "Finishes its time slice and moves to Expired Queue ";
            message += "(Priority: " + std::to_string(p1.getDynamicPriority());
            message += ", Timeslice: " + std::to_string(p1.getTimeSlice()) + ")";
            break;
        case IO_TO_EXPIRED:
            message += "Finishes IO and moves to the Expired Queue ";
            message += "(Priority: " + std::to_string(p1.getDynamicPriority());
            message += ", Timeslice: " + std::to_string(p1.getTimeSlice()) + ")";
            break;
        case IO_TO_ACTIVE:
            message += "Finishes IO and moves to the Ready Queue";
            break;
        default:
            message = "something went wrong.";
            break;
    }
    std::cout << message << std::endl;
}










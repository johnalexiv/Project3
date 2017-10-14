//
//  scheduler.cpp
//  Project3
//
//  Created by John Alexander on 10/5/17.
//  Copyright © 2017 John Alexander. All rights reserved.
//

#define ALLOWPRINTING

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
    process.setEndTime(getClock());
    process.calculateStatistics();
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
        checkArrivingProcesses();

        assignLowestPriorityProcessToCpu();

        checkPreemptRequired();

        updateCpuBurstAndTimeSlice();

        decrementIoBursts();

        checkProcessFinishedCpu();

        checkProcessesFinishedIO();

        if ( allQueuesEmpty() )
            break;

        swapQueuesIfRequired();

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
        printMessage(TO_ACTIVE, currentProcess, __LINE__);

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

        printMessage(TO_CPU, process, __LINE__);
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

            printMessage(PREEMPT, activeProcess, cpuProcess, __LINE__);
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

void Scheduler::decrementIoBursts()
{
    int i = 0;
    std::vector<Process> processes;
    while ( !isIoEmpty() )
    {
        Process process = ioTop();
        ioPop(); 
        processes.push_back(process);
    }
    for ( i = 0; i < processes.size(); i++ )
    {
        processes[i].decrementIoBurst();
        ioPush(processes[i]);
    }
}

void Scheduler::checkProcessFinishedCpu()
{
    if ( !isCpuEmpty() )
    {
        Process cpuProcess = cpuTop();
        cpuPop();

        if ( cpuProcess.getCurrentCpuBurst() == 0 )
        {
            cpuProcess.updateCpuBurst();

            if ( cpuProcess.isFinished() )
            {
                finishedPush(cpuProcess);
                printMessage(FINISHED, cpuProcess, __LINE__);
            }
            else  
            {
                ioPush(cpuProcess);
                printMessage(TO_IO, cpuProcess, __LINE__);
            }
        }
        else if ( cpuProcess.getTimeSlice() == 0 )
        {
            cpuProcess.newTimeSlice();
            expiredPush(cpuProcess);
            printMessage(TO_EXPIRED, cpuProcess, __LINE__);
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

        if ( process.getCurrentIoBurst() == 0 )
        {
            process.updateIoBurst();

            if ( process.getTimeSlice() == 0 )
            {
                process.newTimeSlice();
                expiredPush(process);
                printMessage(IO_TO_EXPIRED, process, __LINE__);
            }
            else
            {
                activePush(process);
                printMessage(IO_TO_ACTIVE, process, __LINE__);
            }
        }
        else
        {
            ioProcesses.push_back(process);
        }
    }

    for ( i = 0; i < ioProcesses.size(); i++ )
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
            printMessage(SWAPPED, __LINE__);
        }
}

void Scheduler::swapActiveAndExpiredQueues()
{
    ActiveExpiredQueue temp = *_activeQueue;
    *_activeQueue = *_expiredQueue;
    *_expiredQueue = temp;
}

void Scheduler::printMessage(int messageType, int line)
{
#ifdef ALLOWPRINTING
    std::string message;
    switch( messageType )
    {
        case SWAPPED:
            message = "[" + std::to_string(getClock()) + "] *** Queue Swap";
            break;
        default:
            message = "something went wrong. <Line: " + std::to_string(line) + ">";
            break;
    }
    std::cout << message << std::endl;
#endif
}

void Scheduler::printMessage(int messageType, Process p1, Process p2, int line)
{
#ifdef ALLOWPRINTING
    std::string message;
    message = "[" + std::to_string(getClock()) + "] ";
    message += "<" + std::to_string(p1.getPID()) + "> ";
    switch ( messageType )
    {
        case PREEMPT:
            message += "Preempts Process " + std::to_string(p2.getPID());
            break;
        default:
            message = "something went wrong. <Line: " + std::to_string(line) + ">";
            break;
    }
    std::cout << message << std::endl;
#endif
}

void Scheduler::printMessage(int messageType, Process p1, int line)
{
#ifdef ALLOWPRINTING
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
            message = "something went wrong. <Line: " + std::to_string(line) + ">";
            break;
    }
    std::cout << message << std::endl;
#endif
}

void Scheduler::printProcessStatistics(Process process)
{
    std::string message;
    message = "PID: <" + std::to_string(process.getPID()) + "> ";
    message += "Statistics:\n";
    message += "1. Turn around time: " + std::to_string(process.getTurnAroundTime()) + "\n";
    message += "2. Total CPU time: " + std::to_string(process.getTotalCpuBurst()) + "\n";
    message += "3. Waiting Time: " + std::to_string(process.getWaitingTime()) + "\n";
    message += "4. Percentage of CPU utilization time: ";
    std::cout << std::fixed << std::setprecision(1);
    std::cout << message << round( process.getUtilizationTime() * 10.0) / 10.0 << std::endl;
}

double Scheduler::round(double value)
{
    if ( value < 0 )
        return ceil(value - 0.5);
    return floor(value + 0.5);
}

void Scheduler::printAverages(std::vector<Process> processes)
{
    double averageWaitingTime = 0.0;
    double averageTurnAroundTime = 0.0;
    double averageCpuUtilization = 0.0;

    for ( int i = 0; i < processes.size(); i++ )
    {
        Process process = processes[i];
        averageWaitingTime += process.getWaitingTime();
        averageTurnAroundTime += process.getTurnAroundTime();
        averageCpuUtilization += process.getUtilizationTime();
    }

    averageWaitingTime = round( averageWaitingTime / processes.size() * 1000.0 ) / 1000.0;
    averageTurnAroundTime = round( averageTurnAroundTime / processes.size() * 1000.0 ) / 1000.0;
    averageCpuUtilization = round( averageCpuUtilization / processes.size() * 1000.0 ) / 1000.0;

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "OVERALL Statistics:" << std::endl;
    std::cout << "Average Waiting Time: " << averageWaitingTime << std::endl;
    std::cout << "Average Turnaround Time: " << averageTurnAroundTime << std::endl;
    std::cout << "Average CPU Utilization Time: " << averageCpuUtilization << std::endl;
}

void Scheduler::printStatistics()
{
#ifdef ALLOWPRINTING
    std::cout << std::endl;
    std::cout << "========================== Overall Performance Output" << std::endl;
    std::cout << "==========================" << std::endl;

    std::vector<Process> processes;
    while ( !isFinishedEmpty() )
    {
        processes.push_back(finishedTop());
        finishedPop();
    }

    for ( int i = 0; i < processes.size(); i++ )
    {
        printProcessStatistics(processes[i]);
        std::cout << " -------------------------------------------------- " << std::endl;
    }

    printAverages(processes);
#endif
}



















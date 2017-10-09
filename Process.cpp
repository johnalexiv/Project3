//
//  process.cpp
//  Project3
//
//  Created by John Alexander on 10/5/17.
//  Copyright © 2017 John Alexander. All rights reserved.
//

#include "process.h"

Process::Process(int pid,
                int nice, 
                int arrivalTime, 
                std::vector<int> cpuBursts,
                std::vector<int> ioBursts)
{
    setPID(pid);
    setPriority(calculateOriginalPriority(nice));
    setTimeSlice(calculateTimeSlice(getPriority()));
    setArrivalTime(arrivalTime);
    setCpuBursts(cpuBursts);
    setIoBursts(ioBursts);
    setStartTime(0);
    setEndTime(0);
    setCurrentCpuBurst(0);
    setCurrentIoBurst(0);
}

int Process::getPID() 
{ 
    return _pid; 
}

void Process::setPID(int pid) 
{ 
    _pid = pid; 
}

int Process::getArrivalTime() 
{ 
    return _arrivalTime; 
}

void Process::setArrivalTime(int arrivalTime) 
{ 
    _arrivalTime = arrivalTime; 
}

int Process::getStartTime() 
{ 
    return _startTime; 
}

void Process::setStartTime(int startTime) 
{ 
    _startTime = startTime; 
}

int Process::getEndTime() 
{ 
    return _endTime; 
}

void Process::setEndTime(int endTime) 
{ 
    _endTime = endTime; 
}

int Process::getPriority() 
{ 
    return _priority; 
}

void Process::setPriority(int priority) 
{ 
    _priority = priority; 
}

int Process::getTimeSlice() 
{ 
    return _timeSlice; 
}

void Process::setTimeSlice(int timeSlice) 
{ 
    _timeSlice = timeSlice; 
}

std::vector<int> Process::getCpuBursts() 
{ 
    return _cpuBursts; 
}

void Process::setCpuBursts(std::vector<int> cpuBursts) 
{ 
    _cpuBursts = cpuBursts; 
}

std::vector<int> Process::getIoBursts() 
{ 
    return _ioBursts; 
}

void Process::setIoBursts(std::vector<int> ioBursts) 
{ 
    _ioBursts = ioBursts; 
}

int Process::getCurrentCpuBurst() 
{ 
    return _currentCpuBurst; 
}

void Process::setCurrentCpuBurst(int currentCpuBurst) 
{ 
    _currentCpuBurst = currentCpuBurst; 
}

int Process::getCurrentIoBurst() 
{ 
    return _currentIoBurst; 
}

void Process::setCurrentIoBurst(int currentIoBurst) 
{ 
    _currentIoBurst = currentIoBurst; 
}

int Process::calculateOriginalPriority(int nice)
{
    return (int)( ( ( ( nice + 20 ) / 39.0 ) * 30 + 0.5 ) + 105 );
}

// fix this
int Process::calculatePriority(int priority, int bonus)
{
    return (int)( priority + bonus );
}

int Process::calculateBonus(int totalCpuBurst, int totalIoBurst)
{
    
}

int Process::calculateTimeSlice(int priority)
{
    return (int)( ( ( 1 - priority / 150.0 ) * 395 + 0.5 ) + 5 );
}




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
    setStaticPriority(calculateStaticPriority(nice));
    setTimeSlice(calculateInitialTimeSlice(getStaticPriority()));
    setArrivalTime(arrivalTime);
    setCpuBursts(cpuBursts);
    setIoBursts(ioBursts);
    setEndTime(0);
    _dynamicPriority = _staticPriority;
    _cpuMaxIndex = cpuBursts.size() - 1;
    _cpuIndex = 0;
    _ioMaxIndex = ioBursts.size() - 1;
    _ioIndex = 0;
    _isFinished = false;
    _totalCpuBurst = 0;
    _totalIoBurst = 0;
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

int Process::getEndTime() 
{ 
    return _endTime; 
}

void Process::setEndTime(int endTime) 
{ 
    _endTime = endTime; 
}

int Process::getStaticPriority() 
{ 
    return _staticPriority; 
}

void Process::setStaticPriority(int priority) 
{ 
    _staticPriority = priority; 
}

int Process::getDynamicPriority() 
{ 
    return _dynamicPriority; 
}

void Process::setDynamicPriority(int priority) 
{ 
    _dynamicPriority = priority; 
}

int Process::getTimeSlice() 
{ 
    return _timeSlice; 
}

void Process::newTimeSlice()
{
    calculateTimeSlice();
}

void Process::setTimeSlice(int timeSlice) 
{ 
    _timeSlice = timeSlice; 
}

void Process::decrementTimeSlice()
{
    if ( getTimeSlice() > 0 )
        _timeSlice--;
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

void Process::updateCpuBurstAndTimeslice()
{
    decrementCpuBurst();
    decrementTimeSlice();
}

int Process::getCurrentCpuBurst()
{
    return _cpuBursts[_cpuIndex];
}

int Process::getTotalCpuBurst()
{
    return _totalCpuBurst;
}

void Process::decrementCpuBurst()
{
    if ( getCurrentCpuBurst() > 0 )
    {
        _cpuBursts[_cpuIndex]--;
        _totalCpuBurst++;
    }
}

void Process::updateCpuBurst()
{
    if ( getCurrentCpuBurst() == 0 && _cpuIndex < _cpuMaxIndex )
        _cpuIndex++;
    else
        _isFinished = true;
}

void Process::updateIoBurst()
{
    if ( getCurrentIoBurst() == 0 && _ioIndex < _ioMaxIndex )
        _ioIndex++;
}

bool Process::isFinished()
{
    return _isFinished;
}

int Process::getCurrentIoBurst()
{
    return _ioBursts[_ioIndex];
}

int Process::getTotalIoBurst()
{
    return _totalIoBurst;
}

void Process::decrementIoBurst()
{
    if ( getCurrentIoBurst() > 0 )
    {
        _ioBursts[_ioIndex]--;
        _totalIoBurst++;
    }
}

int Process::calculateStaticPriority(int nice)
{
    _staticPriority = (int)( ( ( ( nice + 20 ) / 39.0 ) * 30 + 0.5 ) + 105 );
    return _staticPriority;
}

int Process::calculatePriority()
{
    _dynamicPriority = (int)( getStaticPriority() + calculateBonus() );
    return _dynamicPriority;
}

int Process::calculateBonus()
{
    int totalCpuBurst = getTotalCpuBurst();
    int totalIoBurst = getTotalIoBurst();

    if ( totalCpuBurst < totalIoBurst )
        return (int)( ( ( 1 - ( totalCpuBurst / (double)(totalIoBurst) ) ) * -5 ) - 0.5);
    else
        return (int)( ( ( 1 - ( totalIoBurst / (double)(totalCpuBurst) ) ) * 5 ) + 0.5);
}

int Process::calculateTimeSlice()
{
    int priority = calculatePriority();
    _timeSlice = (int)( ( ( 1 - ( priority / 150.0 ) ) * 395 + 0.5 ) + 5 );
    return _timeSlice;
}

int Process::calculateInitialTimeSlice(int priority)
{
    return (int)( ( ( 1 - ( priority / 150.0 ) ) * 395 + 0.5 ) + 5 );
}

int Process::getTurnAroundTime()
{
    return _turnAroundTime;
}

int Process::getWaitingTime()
{
    return _waitingTime;
}

double Process::getUtilizationTime()
{
    return _cpuUtilizationTime;
}

void Process::calculateStatistics()
{
    calculateTurnAroundTime();
    calculateWaitingTime();
    calculateUtilizationTime();
}

void Process::calculateTurnAroundTime()
{
    _turnAroundTime = getEndTime() - getArrivalTime();
}

void Process::calculateWaitingTime()
{
    _waitingTime = getTurnAroundTime() - getTotalCpuBurst() - getTotalIoBurst();
}

void Process::calculateUtilizationTime()
{
    _cpuUtilizationTime = ( (double)getTotalCpuBurst() / (double)getTurnAroundTime() ) * 100;
}   





















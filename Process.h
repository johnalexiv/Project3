//
//  process.h
//  Project3
//
//  Created by John Alexander on 10/5/17.
//  Copyright © 2017 John Alexander. All rights reserved.
//

#ifndef _PROCESS_H
#define _PROCESS_H

#include <vector>
#include <algorithm>

class Process
{
public:
    Process(int pid,
            int nice, 
            int arrivalTime, 
            std::vector<int> cpuBursts,
            std::vector<int> ioBursts);

    int getPID();

    int getArrivalTime();

    int getEndTime();
    void setEndTime(int);

    int getStaticPriority();
    int getDynamicPriority();

    int getTimeSlice();
    void newTimeSlice();

    int getTotalCpuBurst();
    int getTotalIoBurst();

    void updateCpuBurstAndTimeslice();
    int getCurrentCpuBurst();
    void updateCpuBurst();

    int getCurrentIoBurst();
    void updateIoBurst();
    void decrementIoBurst();

    int getTurnAroundTime();
    int getWaitingTime();
    double getUtilizationTime();

    void calculateStatistics();

    bool isFinished();

    std::vector<int> getCpuBursts();
    std::vector<int> getIoBursts();

private:
    void setPID(int);
    void setArrivalTime(int);
    void setStaticPriority(int);
    void setDynamicPriority(int); 
    void setTimeSlice(int);
    void setCpuBursts(std::vector<int>);
    void setIoBursts(std::vector<int>);

    void decrementTimeSlice();
    void decrementCpuBurst();
    
    int calculateStaticPriority(int);
    int calculatePriority();
    int calculateBonus();
    int calculateTimeSlice();
    int calculateInitialTimeSlice(int);
    void calculateTurnAroundTime();
    void calculateWaitingTime();
    void calculateUtilizationTime();

private:
    int     _pid;
    int     _arrivalTime;
    int     _endTime;
    int     _staticPriority;
    int     _dynamicPriority;
    int     _timeSlice;
    bool    _isFinished;

    int     _cpuMaxIndex;
    int     _cpuIndex;
    int     _ioMaxIndex;
    int     _ioIndex;

    int     _totalCpuBurst;
    int     _totalIoBurst;

    int     _turnAroundTime;
    int     _waitingTime;
    double  _cpuUtilizationTime;
    
    std::vector<int> _cpuBursts;
    std::vector<int> _ioBursts;
};


#endif // _PROCESS_H


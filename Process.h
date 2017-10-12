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

class Process
{
public:
    Process(int pid,
            int nice, 
            int arrivalTime, 
            std::vector<int> cpuBursts,
            std::vector<int> ioBursts);

    int getPID();
    void setPID(int);

    int getArrivalTime();
    void setArrivalTime(int);

    int getStartTime();
    void setStartTime(int);

    int getEndTime();
    void setEndTime(int);

    int getPriority();
    void setPriority(int);

    int getTimeSlice();
    void setTimeSlice(int);
    void decrementTimeSlice();

    std::vector<int> getCpuBursts();
    void setCpuBursts(std::vector<int>);

    std::vector<int> getIoBursts();
    void setIoBursts(std::vector<int>);

    int getCurrentCpuBurst();
    void setCurrentCpuBurst(int);

    int getCurrentIoBurst();
    void setCurrentIoBurst(int);

    int getCpuBurstCount();
    void setCpuBurstCount(int);

    void updateProcess();

private:
    int calculateOriginalPriority(int);
    int calculatePriority(int, int);
    int calculateBonus(int, int);
    int calculateTimeSlice(int);

private:
    int _pid;
    int _arrivalTime;
    int _startTime;
    int _endTime;
    int _priority;
    int _timeSlice;

    int _cpuBurstCount;
    int _currentCpuBurst;
    int _currentIoBurst;

    int _totalCpuBurst;
    int _totalIoBurst;
    
    std::vector<int> _cpuBursts;
    std::vector<int> _ioBursts;
};


#endif // _PROCESS_H


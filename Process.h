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

    int getArrivalTime();

    int getStartTime();
    void setStartTime(int);

    int getEndTime();
    void setEndTime(int);

    int getStaticPriority();
    int getDynamicPriority();

    int getTimeSlice();
    void newTimeSlice();

    void updateCpuBurstAndTimeslice();
    int getCurrentCpuBurst();
    int getTotalCpuBurst();

    int getCurrentIoBurst();
    void updateIoBurst();
    int getTotalIoBurst();

    bool isFinished();

    bool cpuFlagSet();
    bool ioFlagSet();

    void setCpuFlag(bool);
    void setIoFlag(bool);

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
    void decrementIoBurst();

    int calculateStaticPriority(int);
    int calculatePriority();
    int calculateBonus();
    int calculateTimeSlice();
    int calculateInitialTimeSlice(int);

private:
    int     _pid;
    int     _arrivalTime;
    int     _startTime;
    int     _endTime;
    int     _staticPriority;
    int     _dynamicPriority;
    int     _timeSlice;
    bool    _isFinished;

    int     _cpuMaxIndex;
    int     _cpuIndex;
    int     _ioMaxIndex;
    int     _ioIndex;
    bool    _cpuBurstFinishedFlag;
    bool    _ioBurstFinishedFlag;

    int     _totalCpuBurst;
    int     _totalIoBurst;
    
    std::vector<int> _cpuBursts;
    std::vector<int> _ioBursts;
};


#endif // _PROCESS_H


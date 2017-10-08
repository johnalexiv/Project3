//
//  project3a.cpp
//  Project3
//
//  Created by John Alexander on 10/5/17.
//  Copyright © 2017 John Alexander. All rights reserved.
//

#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <queue>

class Process
{
public:
    Process(int pid,
            int nice, 
            int arrivalTime, 
            std::vector<int> cpuBursts,
            std::vector<int> ioBursts);

    int getPID() { return _pid; }
    void setPID(int pid) { _pid = pid; }

    int getArrivalTime() { return _arrivalTime; }
    void setArrivalTime(int arrivalTime) { _arrivalTime = arrivalTime; }

    int getStartTime() { return _startTime; }
    void setStartTime(int startTime) { _startTime = startTime; }

    int getEndTime() { return _endTime; }
    void setEndTime(int endTime) { _endTime = endTime; }

    int getPriority() { return _priority; }
    void setPriority(int priority) { _priority = priority; }

    int getTimeSlice() { return _timeSlice; }
    void setTimeSlice(int timeSlice) { _timeSlice = timeSlice; }

    int getCurrentState() { return _currentState; }
    void setCurrentState(int currentState) { _currentState = currentState; }

    std::vector<int> _cpuBursts;
    std::vector<int> _ioBursts;

private:
    int calculateOriginalPriority(int);
    int caluclatePriority(int);
    int calculateTimeSlice(int);
 
private:
    int _pid;
    int _arrivalTime;
    int _startTime;
    int _endTime;
    int _priority;
    int _timeSlice;
    int _currentState;

    int _currentCpuBurst;
    int _currentIoBurst;
    

    enum State
    {
        ARRIVED,
        START,
        ACTIVE,
        EXPIRED,
        IO,
        FINISHED,
        CPU,
        NUMOFSTATES
    };
};

Process::Process(int pid,
                int nice, 
                int arrivalTime, 
                std::vector<int> cpuBursts,
                std::vector<int> ioBursts)
{
    _pid = pid;
    _priority = calculateOriginalPriority(nice);
    _arrivalTime = arrivalTime;
    _cpuBursts = cpuBursts;
    _ioBursts = ioBursts;
    _arrivalTime = 0;
    _startTime = 0;
    _endTime = 0;
    _timeSlice = 0;
    _currentCpuBurst = 0;
    _currentIoBurst = 0;
    _currentState = ARRIVED;
}

int Process::calculateOriginalPriority(int nice)
{
    return (int)((((nice + 20)/39.0)*30 + 0.5) + 105);
}

int Process::caluclatePriority(int priority)
{
    int bonus;

    return (int)(priority + bonus);
}

int Process::calculateTimeSlice(int priority)
{
    return (int)(((1 - priority/150.0)*395 + 0.5) + 5);
}

void readInput();
void debugPrint();
std::vector<int> getProcessDetails(std::string);
void seperateProcesses(std::vector<std::string>);
void createProcess(std::vector<int>);

std::vector<Process> processes;


int main()
{
    readInput();

    debugPrint();

    return 0;
}

void readInput()
{
    std::vector<std::string> processInfo;
    std::string str;
    while( getline(std::cin, str) )
    {
        if ( str.find("***") != std::string::npos )
            break;
        processInfo.push_back(str);
    }

    seperateProcesses(processInfo);
}

void debugPrint()
{
    int i = 0;
    while( i < processes.size() )
    {
        std::cout << "PID: " << processes[i].getPID() << std::endl;
        std::cout << "Priority: " << processes[i].getPriority() << std::endl;

        int j = 0;
        std::cout << "CPU Bursts: ";
        while( j < processes[i]._cpuBursts.size() )
        {
            std::cout << processes[i]._cpuBursts[j++] << " ";
        }
        std::cout << std::endl;

        j = 0;
        std::cout << "IO Bursts: ";
        while( j < processes[i]._ioBursts.size() )
        {
            std::cout << processes[i]._ioBursts[j++] << " ";
        }
        std::cout << std::endl << std::endl;
        i++;
    }
}

void seperateProcesses(std::vector<std::string> processInfo)
{
    int i = 0;
    std::vector<int> processDetails;
    while( i < processInfo.size() )
    {
        processDetails = getProcessDetails(processInfo[i++]);

        createProcess(processDetails);
    }
}

std::vector<int> getProcessDetails(std::string details)
{
    std::string str;
    std::istringstream f(details);
    std::vector<int> values;
    while( getline(f, str, ' ') )
    {
        values.push_back(std::stoi(str));
    }
    return values;
}

void createProcess(std::vector<int> processDetails)
{
    int pid = processes.size();
    int nice = processDetails[0];
    int arrivalTime = processDetails[1];
    int numCpuBursts = processDetails[2];
    std::vector<int> cpuBursts;
    std::vector<int> ioBursts;

    int i = 3;
    while( i < processDetails.size() )
    {
        if ( i % 2 == 1 )
            cpuBursts.push_back(processDetails[i++]);
        else 
            ioBursts.push_back(processDetails[i++]);
    }
    Process process = Process(pid,
                            nice,
                            arrivalTime,
                            cpuBursts,
                            ioBursts);
    processes.push_back(process);
}

//  read input 
// clock = 0 
// While ( true ) {
//  insert processes to the active queue if clock tick . ( calculate priority and if the cpu is empty the lowest priority
// they are to start at this timeslice).
// process in the active queue
//    is put into the cpu. If more than two processes have the same
// priority then FIFO strategy can be used.
// Check if the lowest priority process in the active queue has a lower
// priority then the process in the cpu. If so then preempt.
// Preempted process goes back to the active queue.
// Perform cpu (decrement the timeslice of the process in the cpu) Perform IO(decrement the IO burst for all processes in the IO queue) if there is a process in the cpu (call this process p)
// if p's current cpu burst is exhausted
// if p is done with all cpu bursts send to the finished queue
// if p is not done with all cpu bursts (which means there is an IO
// burst ) send to the IO queue .
// if p's timeslice is exhausted send to the expired queue and
// recalculate priority and timeslice .
// If there is any process in the IO queue that is finished with its IO
// burst (there can be more than one, call this process p) If p's timeslice is exhausted move to the expired queue and
// recalculate the priority and timeslice .
// If p still has timeslice left insert p into the active queue.
// If the startup queue , ready queue , expired queue , IO queue and the cpu are all empty then break out of the while loop (the simulation is complete)
// if the ready queue and cpu are empty and the expired queue is not empty then switch the expired and active queues, this can be done
// by just swapping the pointers of both the queues. Increment the clock (clock++)
// }
// print ending report
// Input Speci cation:
// The input  le will be read using Unix redirection.
// i.e. ./a.out < InputFile.txt
// The input of the program will be a text  le that contains a  nite number of jobs (or processes), one per line. The order of the process can be arbitrary (i.e. not in the order of arrival time). You should read each number until the end of the line. All numbers are integers.
// All input will be assumed to be correct.
// • First column is a process's nice value.
// • Second column is a process's arrival time.
// • Third column is the number of CPU bursts. (remember number of IO bursts is CPU bursts -1)
// • Fourth column is the CPU burst time. 4
         






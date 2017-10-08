//
//  project3a.cpp
//  Project3
//
//  Created by John Alexander on 10/5/17.
//  Copyright © 2017 John Alexander. All rights reserved.
//

#include "project3a.h"

int main()
{
    std::vector<std::string> processesInfo;
    processesInfo = readInput();

    std::vector<Process> processes;
    processes = parseAndCreateProcesses(processesInfo);

    StartQueue startQueue;
    putProcessessIntoStartQueue(startQueue, processes);

    debugPrint(startQueue, processes);

    runProcesses(startQueue, processes);

    return 0;
}

std::vector<std::string> readInput()
{
    std::vector<std::string> processesInfo;
    std::string line;
    while( getline(std::cin, line) )
    {
        if ( line.find("***") != std::string::npos )
            break;
        processesInfo.push_back(line);
    }

    return processesInfo;
}

std::vector<Process> parseAndCreateProcesses(std::vector<std::string> processInfo)
{
    int pid = 0;
    std::vector<int> processDetails;
    std::vector<Process> processes;
    while( pid < processInfo.size() )
    {
        processDetails = getProcessDetails(processInfo[pid]);
        processes.push_back(createProcess(pid, processDetails));
        pid++;
    }

    return processes;
}

std::vector<int> getProcessDetails(std::string details)
{
    std::string token;
    std::istringstream buffer(details);
    std::vector<int> values;
    while( getline(buffer, token, ' ') )
    {
        values.push_back(std::stoi(token));
    }
    return values;
}

Process createProcess(int pid, std::vector<int> processDetails)
{
    int nice = processDetails[0];
    int arrivalTime = processDetails[1];
    int numCpuBursts = processDetails[2];
    std::vector<int> cpuBursts;
    std::vector<int> ioBursts;

    int burst = 3;  // first CPU burst
    while( burst < processDetails.size() )
    {
        if ( burst % 2 == 1 )
            cpuBursts.push_back(processDetails[burst++]);
        else 
            ioBursts.push_back(processDetails[burst++]);
    }
    Process process = Process(pid,
                            nice,
                            arrivalTime,
                            cpuBursts,
                            ioBursts);
    return process;
}

void putProcessessIntoStartQueue(StartQueue &startQueue, std::vector<Process> processes)
{
    int i = 0;
    while( i < processes.size() )
    {
        startQueue.push(processes[i++]);
    }
}


void runProcesses(StartQueue startQueue, std::vector<Process> processes)
{
    int clock = 0;
    while(true)
    {
        break;
    }
}

void debugPrint(StartQueue startQueue, std::vector<Process> processes)
{
    while( !startQueue.empty() )
    {
        Process temp = startQueue.top();
        std::cout << "PID: " << temp.getPID() << ", Arrival Time: " << temp.getArrivalTime() << std::endl;
        startQueue.pop();
    }

    int i = 0;
    while( i < processes.size() )
    {
        std::cout << "PID: " << processes[i].getPID() << std::endl;
        std::cout << "Priority: " << processes[i].getPriority() << std::endl;
        std::cout << "Arrival Time: " << processes[i].getArrivalTime() << std::endl;
        std::cout << "Time Slice: " << processes[i].getTimeSlice() << std::endl;
        int j = 0;
        std::cout << "CPU Bursts: ";
        std::vector<int> cpuBursts = processes[i].getCpuBursts();
        while( j < processes[i].getCpuBursts().size() )
        {
            std::cout << cpuBursts[j++] << " ";
        }
        std::cout << std::endl;

        j = 0;
        std::cout << "IO Bursts: ";
        std::vector<int> ioBursts = processes[i].getIoBursts();
        while( j < processes[i].getIoBursts().size() )
        {
            std::cout << ioBursts[j++] << " ";
        }
        std::cout << std::endl << std::endl;
        i++;
    }
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
         






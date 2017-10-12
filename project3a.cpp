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

    Scheduler scheduler = Scheduler(processes);

    //debugPrint(startQueue, processes);

    scheduler.runProcesses();

    return 0;
}

std::vector<std::string> readInput()
{
    std::vector<std::string> processesInfo;
    std::string line;
    while ( getline(std::cin, line) )
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
    while ( pid < processInfo.size() )
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
    while ( getline(buffer, token, ' ') )
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
    while ( burst < processDetails.size() )
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

void debugPrint(StartQueue startQueue, std::vector<Process> processes)
{
    while ( !startQueue.empty() )
    {
        Process temp = startQueue.top();
        std::cout << "PID: " << temp.getPID() << ", Arrival Time: " << temp.getArrivalTime() << std::endl;
        startQueue.pop();
    }

    int i = 0;
    while ( i < processes.size() )
    {
        std::cout << "PID: " << processes[i].getPID() << std::endl;
        std::cout << "Priority: " << processes[i].getPriority() << std::endl;
        std::cout << "Arrival Time: " << processes[i].getArrivalTime() << std::endl;
        std::cout << "Time Slice: " << processes[i].getTimeSlice() << std::endl;
        int j = 0;
        std::cout << "CPU Bursts: ";
        std::vector<int> cpuBursts = processes[i].getCpuBursts();
        while ( j < processes[i].getCpuBursts().size() )
        {
            std::cout << cpuBursts[j++] << " ";
        }
        std::cout << std::endl;

        j = 0;
        std::cout << "IO Bursts: ";
        std::vector<int> ioBursts = processes[i].getIoBursts();
        while ( j < processes[i].getIoBursts().size() )
        {
            std::cout << ioBursts[j++] << " ";
        }
        std::cout << std::endl << std::endl;
        i++;
    }
}












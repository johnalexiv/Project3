//
//  project3a.h
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
#include "process.h"

std::vector<std::string> readInput();
void debugPrint(std::vector<Process>);
void runProcesses();
std::vector<int> getProcessDetails(std::string);
std::vector<Process> parseAndCreateProcesses(std::vector<std::string>);
Process createProcess(int, std::vector<int>);
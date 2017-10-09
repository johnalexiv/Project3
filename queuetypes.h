//
//  queuetypes.h
//  Project3
//
//  Created by John Alexander on 10/5/17.
//  Copyright © 2017 John Alexander. All rights reserved.
//

#ifndef _QUEUETYPES_H
#define _QUEUETYPES_H

struct compareStartQueue  
{  
    bool operator()(Process first, Process second)  
    {  
        if ( first.getArrivalTime() == second.getArrivalTime() )
            return first.getPID() > second.getPID();
        else
            return first.getArrivalTime() > second.getArrivalTime();  
    }  
};

struct compareActiveExpiredQueue  
{  
    bool operator()(Process first, Process second)  
    {  
        if ( first.getPriority() == second.getPriority() )
            return first.getPID() > second.getPID();
        else
            return first.getPriority() > second.getPriority();  
    }
};

struct compareIOQueue  
{  
    bool operator()(Process first, Process second)  
    {  
        if ( first.getCurrentIoBurst() == second.getCurrentIoBurst() )
            return first.getPID() > second.getPID();
        else
            return first.getCurrentIoBurst() > second.getCurrentIoBurst();  
    }
};

struct compareFinishedQueue  
{  
    bool operator()(Process first, Process second)  
    {
        return first.getEndTime() > second.getEndTime();  
    }
};

typedef std::priority_queue< Process, std::vector<Process>, compareStartQueue >             StartQueue;
typedef std::priority_queue< Process, std::vector<Process>, compareActiveExpiredQueue >     ActiveQueue;
typedef std::priority_queue< Process, std::vector<Process>, compareActiveExpiredQueue >     ExpiredQueue;
typedef std::priority_queue< Process, std::vector<Process>, compareIOQueue >                IOQueue;
typedef std::priority_queue< Process, std::vector<Process>, compareFinishedQueue >          FinishedQueue;
typedef std::queue<Process>                                                                 CPU;


#endif // _QUEUETYPES_H




#ifndef TRYSTAN_KAEST_LAB_H
#define TRYSTAN_KAEST_LAB_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "exceptionStatus.h"


struct process_container{
    int pid;
    int arrival_time;
    int burst_time;
    int finish_time;
    int waiting_time;
    int turn_around;
    int context_switchs;
    process_container(): pid(0), arrival_time(0), burst_time(0), finish_time(0),
                         waiting_time(0), turn_around(0), context_switchs(0)
    {   }
    process_container(int p, int a, int b): pid(p), arrival_time(a), burst_time(b),
                                            finish_time(0), waiting_time(0),
                                            turn_around(0), context_switchs(0)
    {   }
};

exception_status read(std::vector<process_container>&, std::string);


#endif

#ifndef TRYSTAN_KAEST_LAB_H
#define TRYSTAN_KAEST_LAB_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>
#include "exceptionStatus.h"

////////////BEGIN PROCESS_CONTAINER DECLARATIONS///////////////////////////
class process_container{
    public:
        int pid;
        int arrival_time;
        int burst_time;
        int finish_time;
        int waiting_time;
        int turn_around;
        int context_switchs;

        //SORT METHOD FOR QUICKSORT ON VECTOR
        bool operator<(const process_container& other) const
        {
            if( this->burst_time != other.burst_time){ //if no conflict
                return this->burst_time < other.burst_time;
            }
            return this->arrival_time < other.arrival_time; //resolve conflict with FCFS
        }

        bool operator>(const process_container& other) const
        {
            if( this->burst_time != other.burst_time){ //if no conflict
                return this->burst_time > other.burst_time;
            }
            return this->arrival_time > other.arrival_time; //resolve conflict with FCFS
        }


        //CONSTRUCTORS
        process_container(): pid(0), arrival_time(0), burst_time(0), finish_time(0),
                             waiting_time(0), turn_around(0), context_switchs(0)
        {   }
        process_container(int p, int a, int b): pid(p), arrival_time(a), burst_time(b),
                                                finish_time(0), waiting_time(0),
                                                turn_around(0), context_switchs(0)
        {   }

};


struct process_data{
    double burst;
    double wait;
    double turnaround;
    int context_switchs;
};
////////////END PROCESS_CONTAINER DECLARATIONS///////////////////////////

/**Comparison function for vector sorting*/
inline bool compareSRTF(process_container& a, process_container& b)
{
    a.context_switchs += 1;
    b.context_switchs += 1;
    if( a.burst_time != b.burst_time){ //if no conflict
        return a.burst_time < b.burst_time;
    }
    return a.arrival_time < b.arrival_time; //resolve conflict with FCFS
}


exception_status read(std::vector<process_container>&, std::string);
void print(std::vector<process_container>&);
void printRR(std::vector<process_container>&,int);
process_data averages(std::vector<process_container>&);
void fcfs(std::vector<process_container>&);
void srtf(std::vector<process_container>&);
void rr(std::vector<process_container>&, int);
void testPrint(std::vector<process_container>& process_list);

#endif

#ifndef LAB3_HEADER_H
#define LAB3_HEADER_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <fstream>
#include <queue>
#include "exceptionStatus.h"


////////////BEGIN UTILITIES///////////////////////////

//Temporary data holding for averages function
struct process_data{
    double burst;
    double wait;
    double turnaround;
    int context_switchs;
};

////////////END UTILITIES///////////////////////////

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

        //Comparison overloads
        bool operator<(const process_container& other) const;
        bool operator>(const process_container& other) const;
        bool operator==(const process_container& other) const;


        //CONSTRUCTORS
        process_container(): pid(0), arrival_time(0), burst_time(0), finish_time(0),
                             waiting_time(0), turn_around(0), context_switchs(0)
        {   }
        process_container(int p, int a, int b): pid(p), arrival_time(a), burst_time(b),
                                                finish_time(0), waiting_time(0),
                                                turn_around(0), context_switchs(0)
        {   }

};

/** This function is used to implement priority queues for the process_container
  * class. These data structure adaptors are used to implement SRTF in this
  * program. */
template <class process_container> struct burstComp : std::binary_function <process_container,process_container,bool> {
  bool operator() (const process_container& x, const process_container& y) const {
      if( x->burst_time != y->burst_time){ //if no conflict
          return x->burst_time < y->burst_time;
      }
      return x->arrival_time < y->arrival_time; //resolve conflict with FCFS
      }
};

////////////END PROCESS_CONTAINER DECLARATIONS///////////////////////////



////////////BEGIN FUNCTION DECLARATIONS///////////////////////////

/////////////////////////////ALGORITHM EXECUTION FUNCTIONS/////////
void fcfs(std::vector<process_container>&);
void srtf(std::vector<process_container>&);
void rr(std::vector<process_container>&, int);

/////////////////////////////FINAL FORMATTING AND OUTPUT FUNCTIONS/////////
void print(std::vector<process_container>&);
void printGantt(process_container,int,int &,bool);
process_data averages(std::vector<process_container>&);

/////////////////////////////UTILITY FUNCTIONS/////////
exception_status read(std::vector<process_container>&,std::string);

////////////END FUNCTION DECLARATIONS///////////////////////////

#endif

#include "lab3_header.h"

/////////////////////////////ALGORITHM EXECUTION FUNCTIONS/////////

/** This function simulates scheduling using the First Come First Serve algorithm
  * And prints out the gantt chart.
  * @param process_list the list of processes to be scheduled and processed
  */
void fcfs(std::vector<process_container>&process_list){

///////////////Wait Times///////////////
    for (std::vector<process_container>::iterator i = process_list.begin(); i != process_list.end(); ++i){
        if(i == process_list.begin()){ //catches the base case
            i->waiting_time = 0;
            continue;
        }

        //Waiting time of A[i] | A[i].wait_time = A[i-1].CPU_burst + A[i-1].wait_time
        i->waiting_time =  (i-1)->burst_time + (i-1)->waiting_time;
    }


///////////////Turn Around Times///////////////
    for (std::vector<process_container>::iterator i = process_list.begin(); i != process_list.end(); ++i){
        //Turn around time of A[i] | A[i].turnaround = A[i].CPU_burst + A[i].wait_time
        i->turn_around =  i->burst_time + i->waiting_time;
    }

///////////////Finish Times///////////////
    for (std::vector<process_container>::iterator i = process_list.begin(); i != process_list.end(); ++i){
        if(i == process_list.begin()){ //catches the base case
            i->finish_time = i->burst_time;
            continue;
        }

        // Finish time of A[i] | A[i].finish_time = A[i-1].finish_time + A[i].CPU_burst
        i->finish_time =  (i-1)->finish_time + i->burst_time;
    }

    int reset = 0;
    int i;
    for (i = 0; i < process_list.size() - 1; ++i){ //print all but last standard
        printGantt(process_list[i], process_list[i].burst_time, reset, false);
    }
    //print the last one as the last
    printGantt((process_list[i]), process_list[i].burst_time, reset, true);

    return;
}

/** This function simulates scheduling using the Shortest Remaining Time First algorithm
  * @param process_list the list of processes to be scheduled and processed
  */
void srtf(std::vector<process_container>& process_list){
    std::priority_queue<process_container*, std::vector<process_container*>, burstComp<process_container*>> backlog;
    std::vector<process_container> running_process = process_list; //copy so we can butcher burst_times
    process_container* current_process; //what the CPU is actively processing
    int processRun = 0; //how much time on current_process
    int runtime = 0;
    int queue_index = 1;
    int reset = 0;
    backlog.push(&running_process[0]); //get first process
    current_process = backlog.top(); //load first process

    /////////LOAD THE BACKLOG
    while(queue_index < running_process.size()) {

        if(current_process->pid != backlog.top()->pid){ //if there is a smaller process
            if(processRun == 0){
                --current_process->burst_time;
            }
            ++current_process->context_switchs; //we needed to switch (this gets us context switches + 1)
            printGantt((*current_process), processRun, reset, false); //print the current one
            processRun = 0; //reset the time calculation
            current_process = backlog.top(); //set the current process to the smallest one
        }

        if(runtime < running_process[queue_index].arrival_time){ //if there hasn't been a new arrival
            --current_process->burst_time; //process
            ++runtime; //time passed
            ++processRun; //time passed

            if(current_process->burst_time == 0){ //if completly processed
                backlog.pop(); //we done
                current_process->burst_time = 99999999; //want to skip forward
                process_container* it;
                for(int i = 0; i < process_list.size(); ++i){
                    if(*current_process == process_list[i]){
                        it = &process_list[i];
                    }
                }
                it->waiting_time = runtime - it->burst_time; //calculate waiting time
            }

        }else{
            backlog.push(&running_process[queue_index]); //new arrival
            ++queue_index; //move to next process in queue
        }

    }
    // printGantt((*current_process), processRun, reset); //print the current one

    ///////////FINISH PROCESSING THE BACKLOG
    while(!backlog.empty()) { //while we still have processes in the backlog

        if(current_process->pid != backlog.top()->pid){ //if there is a smaller process
            ++current_process->context_switchs; //we needed to switch (this gets us context switches + 1)
            printGantt((*current_process), processRun, reset, false); //print the current one
            processRun = 0; //reset the time calculation
            current_process = backlog.top(); //set the current process to the smallest one
        }

        while(current_process->burst_time > 0){ //completely process this. Smallest isn't changing
            --current_process->burst_time;
            ++runtime;
            ++processRun;
        }

            //if we finished processing the current process AND the top is the current process (paranoia)
        if(current_process->burst_time == 0){
            backlog.pop(); //we done
            current_process->burst_time = 99999999; //want to skip forward
            process_container* it;
            for(int i = 0; i < process_list.size(); ++i){
                if(*current_process == process_list[i]){
                    it = &process_list[i];
                }
            }
            it->waiting_time = runtime - it->burst_time; //calculate waiting time
        }

    }
    if(processRun > 0){
        printGantt((*current_process), processRun, reset,true); //print the current one
    }

    // printf("\n*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   "); //20 ms per line

        //Copy all of the context switches
    for(int i = 0; i < process_list.size(); ++i){
        if(running_process[i].context_switchs == 0){
            process_list[i].context_switchs = running_process[i].context_switchs; //adjusted for extra initial switch
        }else{
            process_list[i].context_switchs = running_process[i].context_switchs - 1; //adjusted for extra initial switch
        }
    }

    ///////////////Turn Around Times///////////////
        for (std::vector<process_container>::iterator i = process_list.begin(); i != process_list.end(); ++i){
            //Turn around time of A[i] | A[i].turnaround = A[i].CPU_burst + A[i].wait_time
            i->turn_around =  i->burst_time + i->waiting_time;
        }

    ///////////////Finish Times///////////////
        for (std::vector<process_container>::iterator i = process_list.begin(); i != process_list.end(); ++i){
            if(i == process_list.begin()){ //catches the base case
                i->finish_time = i->burst_time;
                continue;
            }

            // Finish time of A[i] | A[i].finish_time = A[i-1].finish_time + A[i].CPU_burst
            i->finish_time =  (i-1)->finish_time + i->burst_time;
        }

    return;
}

/** This function simulates scheduling using the Round Robin algorithm.
  * It also prints the Gantt chart
  * @param process_list the list of processes to be scheduled and processed
  */
void rr(std::vector<process_container>& process_list, int quantum){
////////////WAIT_TIMES////////////////
    int r_burst[process_list.size()];
    int runtime = 0;
    int j = 0;
    int reset = 0;
    int processRun = 0;
    for (std::vector<process_container>::iterator i = process_list.begin(); i != process_list.end(); ++i, ++j){
        r_burst[j] = i->burst_time;
    }

    bool m_exit = false;
    while(!m_exit){
        //iterate through all processes
        m_exit = true;
        j = 0;
        process_container lastLineP;
        for (int i = 0; i < process_list.size();  ++i, ++j){
            //if this hasn't been processed
            if(r_burst[j] > 0){
                    //since we are back here... we ain't done yet
                m_exit = false;

                process_list[(i-1)].context_switchs += 1; //one switch here since we changed from last thread

                //if we can't finish it in this run
                if(r_burst[j] > quantum){
                    r_burst[j] -= quantum; //processed one set
                    runtime += quantum; //we move forward in time
                    processRun = quantum; //we ran for the quantum time on current process
                }else{ //if we CAN finish it in this run
                        //the time that it had left
                    runtime += r_burst[j];
                    processRun = r_burst[j]; //we ran the remainder on this process
                        //the whole time it took minus when it was actually processing
                    process_list[i].waiting_time = runtime - process_list[i].burst_time; //is waiting time
                        //Since we finished... We don't need no more
                    r_burst[j] = 0;
                }
                printGantt(process_list[i], processRun, reset, false); //print
            }
        }
    }
        //handle last line print
        printf("\n");
        int lastLine = reset + processRun;
        for(int j = 0; j < lastLine - 1; ++j){ //business as usual
            printf("*   "); //skip past 1 millisen
        }

    // printf("\n*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   "); //20 ms per line
///////////////Turn Around Times///////////////
    for (std::vector<process_container>::iterator i = process_list.begin(); i != process_list.end(); ++i){
        //Turn around time of A[i] | A[i].turnaround = A[i].CPU_burst + A[i].wait_time
        i->turn_around =  i->burst_time + i->waiting_time;
    }

///////////////Finish Times///////////////
    for (std::vector<process_container>::iterator i = process_list.begin(); i != process_list.end(); ++i){
            i->finish_time = i->burst_time + i->waiting_time;
    }


    return;
}



/////////////////////////////FINAL FORMATTING AND OUTPUT FUNCTIONS/////////

/** This function prints the results of the algorithm used in a table
  * @param process_list the algorithm-traversed list filled with data
  */
void print(std::vector<process_container> &process_list){

    ////////////////////TABLE TIME////////////////////

    printf("________________________________________________________________________\n");
    printf("| pid  | arrival  | CPU-  | finish  | waiting | turn around |  No. of  |\n");
    printf("|      |          | burst |         | time    |             |  Context |\n");
    printf("|______|__________|_______|_________|_________|_____________|__________|\n");



    for (std::vector<process_container>::iterator i = process_list.begin(); i != process_list.end(); ++i){
        //FORMAT PID OUTPUT
        printf("| %-4i ", i->pid);

        //FORMAT ARRIVAL_TIME OUTPUT
        printf("| %-8i ", i->arrival_time);

        //FORMAT CPU BURST_TIME
        printf("| %-5i ", i->burst_time);

        //FORMAT FINISH TIME
        printf("| %-7i ", i->finish_time);

        //FORMAT WAITING TIME
        printf("| %-7i ", i->waiting_time);

        //FORMAT TURNAROUND OUTPUT
        printf("| %-11i ", i->turn_around);

        //FORMAT NO. CONTEXT SWITCHES OUTPUT
        printf("| %-8i ", i->context_switchs);


        printf("|\n");
        printf("|______|__________|_______|_________|_________|_____________|__________|\n");
    }

    process_data data = averages(process_list);

    printf("Average CPU burst time = %g ms\n", data.burst);
    printf("Average waiting time = %g ms\n", data.wait);
    printf("Average turn around time = %g ms\n", data.turnaround);
    printf("Total No. of Context Switching Performed = %i\n", data.context_switchs);

}

/** This function prints the gantt chart of the algorithm used in real time.
  * @param current_process The process we are printing out.
  * @param processRun How long current_process just ran for.
  * @param reset The amount of ms we have already written out with no newline.
  * @param lastPrint Flag to know whether we reached the last print.
  */
void printGantt(process_container current_process, int processRun, int &reset, bool lastPrint){
    int lastLine = 0;
    int ms_limit = 20;
    printf("P%-3i",current_process.pid); //print out the process and fill spaces
    reset+=1; //we've used up one millisecond

    if((reset+processRun) <= ms_limit){ //if we have no line wraps
        for(int j = 0; j < processRun-1; ++j){ //business as usual
            printf("    "); //skip past 1 millisen
            reset+=1; //for every print we've used up a millisecond
        }
        lastLine = reset + 1;

    }else{ //if there is a line wrap

            //how much space is left on this line
        int leftover = ms_limit - reset;

            //fill the rest of leftovers
        for(int j = 0; j < leftover; ++j){
            printf("    ");
            reset+=1; //for every print we've used up a millisecond
        }

        //print ms line
        printf("\n*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   \n");
        reset = 0;


             //how much burst remains on next line
        int nextLine = lastLine = processRun - leftover;

            //fill the remainder on the next line
        for(int j = 1; j < nextLine; ++j){
            printf("    ");
            reset+=1; //for every print we've used up a millisecond
        }

    }
    // if(reset > ms_limit){
    //     reset = 0;
    //     printf("\n*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   \n"); //20 ms per line
    // }
    if(lastPrint == true){
        printf("\n");
        for(int j = 0; j < lastLine-1; ++j){ //business as usual
            printf("*   "); //skip past 1 millisen
        }
        reset = 0;
    }
}

/** This function processed the averages and totals of processes
  * @param process_list the algorithm-traversed list filled with data
  */
process_data averages(std::vector<process_container>&process_list){
    process_data data;
    int temp;

    //AVERAGE BURST_TIME
    temp = 0;
    for (std::vector<process_container>::iterator i = process_list.begin(); i != process_list.end(); ++i){
        temp += i->burst_time;
    }
    data.burst = (double)temp / (double)process_list.size();

    //AVERAGE WAIT_TIME
    temp = 0;
    for (std::vector<process_container>::iterator i = process_list.begin(); i != process_list.end(); ++i){
        temp += i->waiting_time;
    }
    data.wait = (double)temp / (double)process_list.size();

    //AVERAGE TURN_AROUND TIME
    temp = 0;
    for (std::vector<process_container>::iterator i = process_list.begin(); i != process_list.end(); ++i){
        temp += i->turn_around;
    }
    data.turnaround = (double)temp / (double)process_list.size();

    //TOTAL SWITCHS
    temp = 0;
    for (std::vector<process_container>::iterator i = process_list.begin(); i != process_list.end(); ++i){
        temp += i->context_switchs;
    }
    data.context_switchs = temp;

    return data;
}



/////////////////////////////UTILITY FUNCTIONS/////////

/** This function reads in a series of processes into a vector
  * @param input The vector that will be filled
  * @param filename The file we are going to read from.
  */
 exception_status read(std::vector<process_container> &input, std::string filename){
    exception_status error;

      std::ifstream infile;

          infile.open(filename);

          ///////////check file/////////
        if (!infile.is_open() || infile.fail()) {
            // set error
            return error.badCall("file_invalid"); // if the file didn't read, no need to continue.
        }

      int count = 0;
      char buf[128];
      input.reserve(100);

      while(!infile.eof()){
        int pid, arrival_time, burst_time;

        count++;

        //---------PID--------
        infile.getline(buf, sizeof(buf), ' '); //read the current word up to space

        pid = std::atoi(buf);

        //---------ARRIVAL_TIME--------
        infile.getline(buf, sizeof(buf), ' '); //read the current word up to space

        arrival_time = std::atoi(buf);

        //---------BURST_TIME--------
        infile.getline(buf, sizeof(buf)); //read the current word up to endline

        burst_time = std::atoi(buf);

        //---------SAVE------
        input.emplace_back(pid, arrival_time, burst_time);
        // printf("pid: %i, at: %i, bt: %i", pid, arrival_time, burst_time);

        infile >> std::ws;

      if (infile.fail()) {
          // set error
          error.which = count;
          return error.badCall("input_error_line_"); // if the file didn't read correctly, no need to continue.
      }
    }

    return error;
}



/////////////////////////////process_container IMPLEMENTATIONS/////////

/** operator overload for comparison functions
  * \@param other The process 'this' is being compared to.
  * \@return Result of comparisons */
bool process_container::operator<(const process_container& other) const
{
    if( this->burst_time != other.burst_time){ //if no conflict
        return this->burst_time < other.burst_time;
    }
    return this->arrival_time < other.arrival_time; //resolve conflict with FCFS
}

/** operator overload for comparison functions
  * \@param other The process 'this' is being compared to.
  * \@return Result of comparisons */
bool process_container::operator==(const process_container& other) const
{
    return this->pid == other.pid;
}

/** operator overload for find functions
  * \@param other The process 'this' is being compared to.
  * \@return Result of comparisons */
bool process_container::operator>(const process_container& other) const
{
    if( this->burst_time != other.burst_time){ //if no conflict
        return this->burst_time > other.burst_time;
    }
    return this->arrival_time > other.arrival_time; //resolve conflict with FCFS
}

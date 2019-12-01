#include "trystan_kaes_lab3.h"


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

////////////////SORT IMPLEMENTATION////////////

/** This sort emulates the CPU context switching
  *\@param process_list The list of processes.
  *\@param n The amount of processes in the process list.
  */
void insertionSort(std::vector<process_container>& process_list, int n)
{
    process_container key;
    for (int i = 1; i < n; i++)
    {
        key = process_list[i];
        int j = i - 1;

        while(j >= 0 && process_list[j] > key)
        {
            key.context_switchs += 1;
            std::swap(process_list[j],process_list[j+1]);
            j = j - 1;
        }
        process_list[j + 1] = key;
    }
}

/** This function prints the results of the algorithm used in formatted fashion
  * @param process_list the algorithm-traversed list filled with data
  */
void print(std::vector<process_container> &process_list){
    int ms_limit = 20;
    // int character_limit = 77;
    int reset = 0;
    for (std::vector<process_container>::iterator i = process_list.begin(); i != process_list.end(); ++i){
        printf("P%-3i",i->pid); //print out the process and fill spaces
        reset+=1; //we've used up one millisecond

        if((reset+i->burst_time) <= ms_limit){ //if we have no line wraps
            for(int j = 0; j < i->burst_time-1; ++j){ //business as usual
                printf("    "); //skip past 1 millisen
                reset+=1; //for every print we've used up a millisecond
            }
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
            int nextLine = i->burst_time - leftover;

                //fill the remainder on the next line
            for(int j = 1; j < nextLine; ++j){
                printf("    ");
                reset+=1; //for every print we've used up a millisecond
            }

        }
        if(reset > ms_limit && i != process_list.end()){
            reset = 0;
            printf("\n*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   \n"); //20 ms per line
        }
    }
    printf("\n*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   "); //20 ms per line
    printf("\n(each star represents one ms)\n");

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
    printf("Average turn around time = %g ms,\n", data.turnaround);
    printf("Total No. of Context Switching Performed = %i\n", data.context_switchs);

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

/** This function prints the round robin Algorithm.
  * @param process_list the list of processes to be processed and printed
  */
void printRR(std::vector<process_container>& process_list, int quantum){
////////////WAIT_TIMES////////////////
    int r_burst[process_list.size()];
    int runtime = 0;
    int j = 0;
    int ms_limit = 20;
    int reset = 0;
    int processRun = 0;
    for (std::vector<process_container>::iterator i = process_list.begin(); i != process_list.end(); ++i, ++j){
        r_burst[j] = i->burst_time;
    }

    bool m_exit = false;
    while(!m_exit){
        //iterate through all processes to find print times
        m_exit = true;
        j = 0;
        for (std::vector<process_container>::iterator i = process_list.begin(); i != process_list.end(); ++i, ++j){
            //if this hasn't been processed
            if(r_burst[j] > 0){
                    //since we are back here... we ain't done yet
                m_exit = false;
                //if we can't finish it in this run
                if(r_burst[j] > quantum){
                    r_burst[j] -= quantum;
                    runtime += quantum;
                    processRun = quantum; //we ran for the quantum time on current process
                }else{ //if we CAN finish it in this run
                        //the time that it had left
                    runtime += r_burst[j];
                    processRun = r_burst[j]; //we ran the remainder on this process
                        //the whole time it took minus when it was actually processing
                    i->waiting_time = runtime - i->burst_time;
                        //Since we finished... We don't need no more
                    r_burst[j] = 0;

                }
            }

            //NOW WE HANDLE THE PRINT OF THAT PROCESS

            printf("P%-3i",i->pid); //print out the process and fill spaces
            reset+=1; //we've used up one millisecond

            if((reset+processRun) <= ms_limit){ //if we have no line wraps
                for(int j = 0; j < processRun-1; ++j){ //business as usual
                    printf("    "); //skip past 1 millisen
                    reset+=1; //for every print we've used up a millisecond
                }
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
                int nextLine = processRun - leftover;

                    //fill the remainder on the next line
                for(int j = 1; j < nextLine; ++j){
                    printf("    ");
                    reset+=1; //for every print we've used up a millisecond
                }

            }
            if(reset > ms_limit && i != process_list.end()){
                reset = 0;
                printf("\n*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   \n"); //20 ms per line
            }
        }
    }

    printf("\n*   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   "); //20 ms per line
    printf("\n(each star represents one ms)\n");

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
    printf("Average turn around time = %g ms,\n", data.turnaround);
    printf("Total No. of Context Switching Performed = %i\n", data.context_switchs);

}


/** This function simulates scheduling using the First Come First Serve algorithm
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



    return;
}

/** This function simulates scheduling using the Shortest Remaining Time First algorithm
  * @param process_list the list of processes to be scheduled and processed
  */
void srtf(std::vector<process_container>& process_list){
    // std::priority_queue<process_container*, std::vector<process_container*>, std::greater<process_container*>> backlog;
    // std::vector<process_container> running_process = process_list;
    // process_container current_process;
    // int processRun;
    // int runtime = 0;
    //
    //
    // //iterate through all processes
    // for (int i = 0; i < running_process.size(); ++i){
    //
    //
    //     //run the current process until a new arrival
    //     processRun = 0;
    //     while(runtime < running_process[i + 1].arrival_time || running_process[i].burst_time > 0){
    //         --running_process[i].burst_time;
    //         ++runtime;
    //         ++processRun;
    //     }
    //
    //     if(runtime == running_process[i + 1].arrival_time){
    //         backlog.push(&running_process[i + 1]);
    //     }
    //
    //     if(running_process[i] < (*backlog.top())){
    //         --i;
    //     }else{
    //         ++running_process[i].context_switchs;
    //     }
    //
    //
    // }
    //
    // bool m_exit = false;
    // while(!m_exit){
    //     m_exit = true;
    //
    //     for (int i = 0; i < running_process.size(); ++i){
    //
    //         if(running_process[i].burst_time > 0){
    //             m_exit = false;
    //
    //         }
    //
    //
    //     }
    // }

    std::sort(process_list.begin(), process_list.end());
    fcfs(process_list);
    return;
}


/** This function simulates scheduling using the Round Robin algorithm.
  * @param process_list the list of processes to be scheduled and processed
  */
void rr(std::vector<process_container>& process_list, int quantum){
////////////WAIT_TIMES////////////////
    int r_burst[process_list.size()];
    int runtime = 0;
    int j = 0;
    for (std::vector<process_container>::iterator i = process_list.begin(); i != process_list.end(); ++i, ++j){
        r_burst[j] = i->burst_time;
    }

    bool m_exit = false;
    while(!m_exit){
        //iterate through all processes
        m_exit = true;
        j = 0;
        for (std::vector<process_container>::iterator i = process_list.begin(); i != process_list.end(); ++i, ++j){
            //if this hasn't been processed
            if(r_burst[j] > 0){
                    //since we are back here... we ain't done yet
                m_exit = false;

                i->context_switchs += 1; //one switch here since we changed from last thread

                //if we can't finish it in this run
                if(r_burst[j] > quantum){
                    r_burst[j] -= quantum; //processed one set
                    runtime += quantum; //we move forward in time
                }else{ //if we CAN finish it in this run
                        //the time that it had left
                    runtime += r_burst[j];
                        //the whole time it took minus when it was actually processing
                    i->waiting_time = runtime - i->burst_time;
                        //Since we finished... We don't need no more
                    r_burst[j] = 0;

                }
            }
        }
    }

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


void testPrint(std::vector<process_container>& process_list){
    for (std::vector<process_container>::const_iterator i = process_list.begin(); i != process_list.end(); ++i){
		std::cout << i->pid << " "
				  << i->arrival_time << " "
				  << i->burst_time << " "
				  << std::endl;
	}
}

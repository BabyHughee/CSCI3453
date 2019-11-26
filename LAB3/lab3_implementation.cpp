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

void print(std::vector<process_container> &process_list){
    int reset = 0;
    for (std::vector<process_container>::iterator i = process_list.begin(); i != process_list.end(); ++i){
        if((i-1)->burst_time == 1 && i->pid > 9){
            if((i-1)->pid > 9){
                reset-=2;
            }else{
                reset-=1;
            }

        }
        printf("P%i",i->pid);
        reset+=1;
        if((i->burst_time+reset) > 31){ //if there is a line split
            int line1 = 30 - reset; //whats remains on line 1
            int line2 = i->burst_time - line1 - 1; //whats left for line 2
            for(int j = 0; j < line1-1; ++j){
                printf("  ");
            }

            printf("\n");
            reset = 0;
            printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *");
            printf("\n");

            reset += line2;
            for(int j = 0; j < line2-1; ++j){
                printf("  ");
            }
        }else{
            reset += i->burst_time - 1;

            if((i-1)->burst_time == 1 && i->pid > 9){
                if((i-1)->pid > 9){
                    for(int j = 2; j < (i->burst_time-1)*2; ++j){
                        printf(" ");
                    }
                }else{
                    for(int j = 1; j < (i->burst_time-1)*2; ++j){
                        printf(" ");
                    }
                }


            }else if((i-1)->pid > 9){
                for(int j = 1; j < (i->burst_time-1)*2; ++j){
                    printf(" ");
                }
            }else{
                for(int j = 0; j < i->burst_time-1; ++j){
                    printf("  ");
                }
            }



        }

        if(reset > 30){
            printf("\n");
            reset = 0;
            printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *");
            printf("\n");
        }
    }
    printf("\n");
    printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *");
    printf("\n");
}


void fcfs(std::vector<process_container>&process_list){

    //Wait Times
    for (std::vector<process_container>::iterator i = process_list.begin(); i != process_list.end(); ++i){
        if(i == process_list.begin()){
            i->waiting_time = 0;
            continue;
        }
        i->waiting_time =  (i-1)->burst_time + (i-1)->waiting_time;
    }


    //Turn Around Times
    for (std::vector<process_container>::iterator i = process_list.begin(); i != process_list.end(); ++i){
        i->turn_around =  i->burst_time + i->waiting_time;
    }

    //Finish Times
    for (std::vector<process_container>::iterator i = process_list.begin(); i != process_list.end(); ++i){
        if(i == process_list.begin()){
            i->finish_time = i->burst_time;
            continue;
        }

        i->finish_time =  (i-1)->finish_time + i->burst_time;
    }



    return;
}

void srtf(std::vector<process_container>&){


    return;
}

void rr(std::vector<process_container>&){


    return;
}

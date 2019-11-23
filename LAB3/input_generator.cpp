#include <stdio.h>
#include <cstdlib>
#include <time.h>

////CHANGE THESE VALUES TO ADJUST TEST FILE GENERATION///
#define ARRIVAL_FREQUENCY 5
#define BURST_RANGE 10
#define PROCESS_AMNT 100 //Maximum for input files as specified in Lab3

int main(int argv, char *argc[]){

    // /* Test input conditions */
    if(argv != 2){
        printf("WARNING: Insufficient arguments. %i invalid\n", argv);
        printf("	Usage: [executable] <filename>\n");
        return 0;
    }

    FILE *file = fopen(argc[1], "w");

    srand(time(NULL));

    // int arrival_time[PROCESS_AMNT];
    int pid, burst, arrival = 0;

    for(pid = 0; pid < PROCESS_AMNT + 1; ++pid){
        /////////////////Arrival Time//////////////
        arrival = arrival + rand() % ARRIVAL_FREQUENCY + 1;
        /////////////////Burst Time//////////////
        burst = rand() % BURST_RANGE + 1;
        ////////////////File Print///////////////
        fprintf(file, "%i %i %i\n", pid, arrival, burst);


    }
    fclose(file);
    return 0;
}

#include "lab3_header.h"


int main(int argv, char *argc[]){
	std::vector<process_container> process_list;
	printf("CPU Simulator: Scheduling Algorithms\n");

	/* Test input conditions */
	if(argv < 3){
		printf("WARNING: Insufficient arguments.\n");
		printf("	Usage: myscheduler <filename> <algorithm>\n");
		return 0;
	}
	/* Test algorithm input */
	if(!(strcmp(argc[2],"FCFS") == 0 || strcmp(argc[2],"SRTF") == 0 || strcmp(argc[2],"RR") == 0)){
			printf("WARNING: Selected Algorithm unavailable.\n");
			printf("\nALGORITHMS:                  - FCFS\n");
			printf("                             - SRTF\n");
			printf("                             - RR\n");
		return 0;
	}

	exception_status error = read(process_list, argc[1]); //read in the process'
	if(error.error == true){ std::cout << error.status << std::endl; } //error ? output : move on

	printf("************************************************************\n");
	printf("*********** Scheduling algorithm : %-6s ******************\n", argc[2]);
	printf("************************************************************\n");

	if(strcmp(argc[2],"FCFS") == 0){
		fcfs(process_list);
	}else if(strcmp(argc[2],"SRTF") == 0){
		srtf(process_list);
	}else if(strcmp(argc[2],"RR") == 0 && argv == 4){
		rr(process_list, atoi(argc[3]));
	}else{
		printf("WARNING: Selected Algorithm unavailable without quantum time.\n");
		return 0;
	}


	std::printf("\n(each star represents one ms)\n");
	print(process_list);

	return 0;

}

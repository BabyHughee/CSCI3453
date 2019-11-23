#include "trystan_kaes_lab3.h"


int main(int argv, char *argc[]){
	std::vector<process_container> process_list;
	std::cout << "CPU Simulator: Scheduling Algorithms\n" << std::endl;

	/* Test input conditions */
	if(argv != 3){
		std::cout << "WARNING: Insufficient arguments." << std::endl;
		std::cout << "	Usage: myscheduler <filename> <algorithm>" << std::endl;
		return 0;
	}
	/* Test algorithm input */
	if(!(strcmp(argc[2],"FCFS") == 0 || strcmp(argc[2],"SRTF") == 0 || strcmp(argc[2],"RR") == 0)){
	        std::cout << "WARNING: Selected Algorithm unavailable." << std::endl;
		return 0;
	}


	std::cout << "************************************************************" << std::endl;
	std::cout << "************ Scheduling algorithm : " << argc[2] << " *******************" << std::endl;
	std::cout << "************************************************************" << std::endl;
	std::cout << "	On process list \"" << argc[1] << "\"" << std::endl;


	//Prepare for finding the minimal spanning tree
	exception_status error = read(process_list, argc[1]); //read in the process'
	if(error.error == true){ std::cout << error.status << std::endl; } //error ? output : move on

	for (std::vector<process_container>::const_iterator i = process_list.begin(); i != process_list.end(); ++i){
		std::cout << i->pid << " "
				  << i->arrival_time << " "
				  << i->burst_time << " "
				  << std::endl;
	}

	std::cout << process_list.size() << std::endl;

	return 0;

}

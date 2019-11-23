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

/*Trystan Kaes
 * Dr. Salim Lakhani
 * CSCI 3453
 * September 11, 2019*/
#include <fstream>
#include <iostream>
#include <sys/utsname.h>
#include <string>
#include <sstream>
#include <ctime>


/*read specified line from file */
std::string getInfo(std::string, std::string);
/* Read specified line from file up to a string delimiter */
std::string getInfoWDelimiter(std::string, std::string, char);
int main(){

  ////////PART A//////////////////////
  std::cout << "\n//////////////////////////////////////////////////";
  std::cout << "\n//////////////////////PART A//////////////////////";
  std::cout << "\n//////////////////////////////////////////////////\n";
  struct utsname systemInformation; //struct holds system information

  if (uname(&systemInformation) < 0) //get information and check if fail
    printf("\n******USNAME_FAILURE*****\n"); //if fail say so.
  else { //if success print everything
    printf("Sysname  -  %s\n", systemInformation.sysname);
    printf("Release  -  %s\n", systemInformation.release);
    printf("Version  -  %s\n", systemInformation.version);
    printf("Machine  -  %s\n", systemInformation.machine);
    printf("Nodename -  %s\n", systemInformation.nodename);
  }

  ////////PART B//////////////////////
  ////////////////////////////////////////////time since system was last booted

  std::cout << "\n//////////////////////////////////////////////////";
  std::cout << "\n//////////////////////PART B//////////////////////";
  std::cout << "\n//////////////////////////////////////////////////\n";
  std::string info; //holds all returns from read in functions
  info = getInfo("/proc/stat", "btime"); //get specified line
  std::istringstream catalyst(info); //format into stringstrem
  int epoch; //holds time integers
  catalyst >> info; //strip leading string
  catalyst >> epoch; // extract integer
  time_t timestamp = epoch; //convert integer to timestamp
  struct tm * timeinfo = localtime(&timestamp); //convert time_t into struct tm*
  char formatted[80]; //holds the output for each time print
  /*convert struct tm* into required format*/
  size_t confirm = strftime(formatted, 80, "Time of last boot         - %F %X", timeinfo);

  std::cout << formatted << std::endl; //output

  ///////////////////////////////The amount of time since system was last booted

  info = getInfoWDelimiter("/proc/uptime", "btime", ' '); //get specified line
                                                          //ignore after ' '
  std::istringstream catalyst1(info); //convert to stringstream
  catalyst1 >> epoch; //extract integer
  timestamp = epoch; //convert to time_t
  timeinfo = localtime(&timestamp); //convert time_t to struct tm*
  /*convert struct tm* into required format*/
  confirm = strftime(formatted, 80, "Time since last boot      - %j:%X", timeinfo);

  std::cout << formatted << std::endl;

  ////////////////////////////////////CPU has spent in user mode and system mode

  info = getInfo("/proc/stat", "cpu"); //read in specified line
  std::istringstream catalyst2(info); //convert to string stream
  catalyst2 >> info; //strip leading string
  catalyst2 >> epoch; //extract first integer
  // timestamp = epoch;
  // timeinfo = localtime(&timestamp);
  // confirm = strftime(formatted, 80, "Time spent in user mode   - %d:%X", timeinfo);
  // std::cout << formatted << std::endl;
  std::cout << "Time spent in user mode   - " << (epoch/100) << " sec" << std::endl; //output first integer
  catalyst2 >> epoch; //strip second integer
  catalyst2 >> epoch; //extrct third integer
  // timestamp = epoch;
  // timeinfo = localtime(&timestamp);
  // confirm = strftime(formatted, 80, "Time spent in system mode - %d:%X", timeinfo);
  // std::cout << formatted << std::endl;
  std::cout << "Time spent in system mode - " << (epoch/100) << " sec" << std::endl; //output third integer

///////The total amount of memory in the system and the amount of memory currently available.

  int memory;
  info = getInfo("/proc/meminfo", "MemTotal"); //extract specified line
  std::stringstream catalyst3(info);
  catalyst3 >> info;
  catalyst3 >> memory;
  std::cout << "Total Memory              - " << memory << " kB" << std::endl; //output

  info = getInfo("/proc/meminfo", "MemAvailable"); //extract specified line
  std::stringstream catalyst4(info);
  catalyst4 >> info;
  catalyst4 >> memory;
  std::cout << "Available Memory          - " << memory << " kB" << std::endl; //output


  return 0;

}


/*read specified line from file */
std::string getInfo(std::string fileName, std::string line){
  std::string info; //initialize input/return string
  std::ifstream in; //initialize file stream
  size_t hello; //initalize check variable

  in.open(fileName); //open

  if(in.is_open() == false){ //if not open
    return "acquisition fail"; //fail
  }
while(!in.eof()){ //while still in the file
  getline(in, info); //get a line
  hello = info.find(line); //find the specified search term

  if(hello != std::string::npos){ //if found
    return info; //return
  }
}

  return "acquisition fail"; //if fail return fail
}




/* Read specified line from file up to a string delimiter */
std::string getInfoWDelimiter(std::string fileName, std::string line, char d){
  std::string info; //initialize return/input string
  std::ifstream in; //initalize file stream

  in.open(fileName); //open file

  if(in.is_open() == false){ //if file isn't open
    return "acquisition fail"; //return fail message
  }

  getline(in, info, d); //otherwise read up to the passed delimiter

  return info; //return the string.
}

#include <fstream>
#include <iostream>
#include <sys/utsname.h>
#include <string>
#include <sstream>


std::string getInfo(std::string, std::string);
std::string getInfoWDelimiter(std::string, std::string, char);
int main(){

  ////////PART A//////////////////////
  std::cout << "\n//////////////////////////////////////////////////";
  std::cout << "\n//////////////////////PART A//////////////////////";
  std::cout << "\n//////////////////////////////////////////////////\n";
  struct utsname systemInformation;

  if (uname(&systemInformation) < 0)
    printf("\n******USNAME_FAILURE*****\n");
  else {
    printf("Sysname:  %s\n", systemInformation.sysname);
    printf("Release:  %s\n", systemInformation.release);
    printf("Version:  %s\n", systemInformation.version);
    printf("Machine:  %s\n", systemInformation.machine);
    printf("Nodename: %s\n", systemInformation.nodename);
  }

  ////////PART B//////////////////////
  //time since system was last booted
  std::cout << "\n//////////////////////////////////////////////////";
  std::cout << "\n//////////////////////PART B//////////////////////";
  std::cout << "\n//////////////////////////////////////////////////\n";
  std::string info;
  info = getInfo("/proc/stat", "btime");
  std::istringstream catalyst(info);
  int epoch;
  catalyst >> info;
  catalyst >> epoch;
  time_t timestamp = epoch;
  struct tm * timeinfo = localtime(&timestamp);
  char formatted[80];
  size_t confirm = strftime(formatted, 80, "Time since last boot - %F %X", timeinfo);

  std::cout << formatted << std::endl;

  //The amount of time since system was last booted
  info = getInfoWDelimiter("/proc/uptime", "btime", ' ');
  std::istringstream catalyst1(info);
  catalyst1 >> epoch;
  timestamp = epoch;
  timeinfo = localtime(&timestamp);
  confirm = strftime(formatted, 80, "Amount of uptime - %j:%X", timeinfo);
  std::cout << formatted << std::endl;

  //CPU has spent in user mode and system mode
  info = getInfo("/proc/stat", "cpu");
  // std::cout << info << std::endl;
  std::istringstream catalyst2(info);
  catalyst2 >> info;
  catalyst2 >> epoch;
  timestamp = epoch;
  timeinfo = localtime(&timestamp);
  confirm = strftime(formatted, 80, "Time spent in user mode - %d:%X", timeinfo);
  std::cout << formatted << std::endl;
  catalyst2 >> epoch;
  catalyst2 >> epoch;
  timestamp = epoch;
  timeinfo = localtime(&timestamp);
  confirm = strftime(formatted, 80, "Time spent in system mode - %d:%X", timeinfo);
  std::cout << formatted << std::endl;

  info = getInfo("/proc/meminfo", "MemTotal");
  std::cout << info << std::endl;

  info = getInfo("/proc/meminfo", "MemAvailable");
  std::cout << info << std::endl;

  return 0;

}

std::string getInfo(std::string fileName, std::string line){
  std::string info;
  std::ifstream in;
  size_t hello;

  in.open(fileName);

  if(in.is_open() == false){
    return "acquisition fail";
  }
while(!in.eof()){
  getline(in, info);
  hello = info.find(line);

  if(hello != std::string::npos){
    return info;
  }
}

  return "acquisition fail";
}

std::string getInfoWDelimiter(std::string fileName, std::string line, char d){
  std::string info;
  std::ifstream in;
  size_t hello;

  in.open(fileName);

  if(in.is_open() == false){
    return "acquisition fail";
  }

  getline(in, info, d);

  return info;
}

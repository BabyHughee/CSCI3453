#include <fstream>
#include <iostream>
#include <sys/utsname.h>
#include <string>
#include <sstream>


std::string getInfo(std::string, std::string);

int main(){

  ////////PART A//////////////////////
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
  std::string info;
  info = getInfo("/proc/stat", "btime");
  std::istringstream catalyst(info);
  int extracted_line;
  catalyst >> info;
  catalyst >> extracted_line;
  time_t t = extracted_line;
  cout << localtime(&t) << std::endl;

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

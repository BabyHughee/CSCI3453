#include <fstream>
#include <iostream>
#include <sys/utsname.h>
#include <string>


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


  return 0;

}

std::string getInfo(std::string fileName, std::string line){
  std::string info;
  std::ifstream in;

  in.open(fileName);

  if(in.is_open() == false){
    return "acquisition fail";
  }
while(!in.eof()){
  getline(in, info);
  if(info.compare(0,line.size() - 1,line) == 0){
    return info;
  }
}

  return "acquisition fail";
}


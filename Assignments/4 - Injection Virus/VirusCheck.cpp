#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <ctime>

using namespace std;

int main() {
  // Virus identification will be accomplished by a grep that searches for malicious code.
  // Excludes binary files and false positive from the virus checker.
  // Will check the entire directory recursively
  string str = "grep -I -R 'rm –rf *' --exclude 'VirusCheck.cpp'";
  
  //store string in command char array with size +1 for null space terminator
  char command[str.size() + 1];
  
  //strcpy takes a string and converts it into a char array
  strcpy(command,str.c_str());
  
  //runs command through system call
  system(command);

  cout << "\nVirus Checking Complete!\n";
  return 0;
}
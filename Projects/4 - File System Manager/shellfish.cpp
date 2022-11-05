//Evan Lee and Ryan Scott's Work

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <ctime>

using namespace std;

// Help Command (doesn't count as a command)
void help(){
  cout << endl << "       List of Commands" << endl;
  cout << "ls   mkdir   echo   clear   date  exit" << endl;
  cout << "ps   cal   ifconfig   whoami   free -m  top " << endl;
  cout << "mkdir  mv  rm  rm -r  rmdir  touch  cat >  du" << endl;
  cout << "find   tree    stat   cp -R  ls -l   cp" << endl;

}


void exit() {
  //exits shell
  exit(0);
}


void doItAll(string str){

  //store string in command char array with size +1 for null space terminator
  char command[str.size() + 1];

  //strcpy takes a string and converts it into a char array
  strcpy(command,str.c_str());
  
  //runs command through system call
  system(command);
  
}


//Will analyze the string that the user has given
void lineInterpreter(string result){
  //will run help command
  if(result.substr(0,4) == "help"){
      help();
  }

  //will run exit command
  else if(result.substr(0,4) == "exit"){
      exit();
  }
  
  //will run system() command on result string
  else{
      doItAll(result);
  }
  
}


int main() {
  //need to flush out first getline()
  cout.flush();
  
  while(true){
    cout << "shellfish---->";  
    string result;
    getline(cin, result); //takes in user input
    
    lineInterpreter(result); //function to analyze string
  }

}
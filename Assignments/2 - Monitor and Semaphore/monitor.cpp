// Evan and Ryan's Work
// Ricardo Citro
// CST-315
// 2/13/22

#include <iostream>
#include <unistd.h>
#include <ctime>

using namespace std;

//Monitor Class that will be used to solve the producer and consumer problem
class Monitor{
  private:
    static const int N = 3; //buffer size

    int count = 0; //min buffer
    int arr[N] = {0}; //Instanciate array with 0's
    

  public:
    Monitor(); //constructor
    ~Monitor(); //destructor

    //checks if buffer is not full so it can produce
    bool canProduce() {
      return (count < N);
    }

    //checks if buffer is not empty so it can consume
    bool canConsume() {
      return (count > 0);
    }
    
    //inserts an item in the array if it can produce
    void insert(int item){
      
      if(canProduce()){
        //add item to array
        arr[count] = item;
        count++;
        
      }
      else{
        return;
      }
    }

    //removes an item in the array if it can consume
    void remove(){
      if(canConsume()){
        arr[count - 1] = (int)NULL;
        count--;
      }
      else{
        return;
      }
    }

    //prints out the array of values
    void print(){
      for(auto& it: arr){
        cout << it << ",\t";
      }
      cout << endl;
    }


};

//producer function will always want to produce if there is space in the buffer
void produce(Monitor* m){ 
  while(true) {
    if(m->canProduce()) {  //checks if produce is possible
      m->insert(rand()%100+1);  //inserts item
      m->print();  //prints out buffer
      sleep(1);
    }
    else {
      break;
    }
  }
}

//consumer function will always want to consume if there is something in the buffer
void consume(Monitor* m){
  while(true) {
    if(m->canConsume()){   //checks if consume is possible
      m->remove();   //removes item
      m->print();   //prints out buffer
      sleep(1);
    }
    else {
      break;
    }
  }
}

int main() {
  //srand is used for random numbers
  srand(time(NULL));

  //create a monitor object using the constructor
  Monitor monitor;

  //will run produce and consume on loop
  while(true){
    produce(&monitor);
    consume(&monitor);
  }

  return 0;
}
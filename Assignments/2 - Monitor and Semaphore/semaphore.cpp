// Evan and Ryan's Work
// Ricardo Citro
// CST-315
// 2/13/22

#include <iostream>
#include <unistd.h>
#include <ctime>
#include <mutex>
using namespace std;

static const int N = 3; // Buffer size
int arr[N] = {0};  // Buffer


// A semaphore is an integer variable that can be accessed only through two standard operations: wait() and signal().
int full = 0; int empty = N;  // 2 Counting Semaphores
std::mutex m;  // Mutex lock


// The wait operation reduces the value of semaphore by 1
void wait(int &counter) {
  if(counter > 0 && counter <= N)
    counter--;
}


// The signal operation increases its value by 1
void signal(int &counter) {
  if(counter >= 0 && counter < N)
    counter++;
}


// Helper function to print entire buffer
void print(){
  for(auto& it: arr){
    cout << it << ",\t";
  }
  cout << endl;
}


void Producer() {

  // Produce an item
  int item = rand()%100+1;
  
  // Reduces value of semaphore by 1 and locks mutex
  wait(empty);  //makes empty go down one
  m.lock();  //locks mutex

  // Too full, can't produce.
  if(full == N)
  {
    m.unlock(); //unlocks mutex
    return;
  }
  
  cout << "Producer Produced: " << item << " \n";
  
  // Add to buffer
  arr[full] = item;
  print();
    
  sleep(1);
  
  m.unlock(); //unlocks mutex
  signal(full);  //makes full go up one
  
}


void Consumer() {
  // Reduces value of semaphore by 1 and locks mutex
  wait(full);  //makes full go down one
  m.lock();  //locks mutex
  
  // Too empty, can't consume.
  if(empty == N)
    {
      m.unlock();   //unlocks mutex
      return;
    }

  cout << "Consumer Consumed: " << arr[full] << "\n";
    
  // Remove from buffer 
  arr[full] = 0;
  print();
    
  sleep(1);
  
  m.unlock();   //unlocks mutex
  signal(empty); //makes empty go up one
}


int main() {
  //srand for random numbers
  srand(time(NULL));

  while(true) {
    Producer();
    Producer();
    Consumer();
    Producer();
    Consumer();
    Consumer();
  }

  return 0;
}

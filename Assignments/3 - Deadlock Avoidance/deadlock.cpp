// Evan and Ryan's Work
// Ricardo Citro
// CST-315
// 2/27/22

#include <iostream>
#include <stdio.h>
#include <unistd.h>
using namespace std;

static const int N = 5;  // Buffer size
int buffer[N] = {0};  // Initialize buffer with 0's
int counter;  // Indexing buffer
bool produce, consume;  // Produce and Consume locks

// Prints out the array of values
void print(){
  for(auto& it: buffer){
    cout << it << ",\t";
  }
  cout << endl;
}

// Determine locks
void DetermineBools() {
  // Consumer
  if(counter >= 0) consume = true;
  else consume = false;
  // Producer
  if(counter < N) produce = true;
  else produce = false;
}

void Producer() {
  time_t timeout;
  
  while(true) {
    DetermineBools();  // Determine if you can produce
    if(produce) {
      cout << "Produced 1\n";
      buffer[counter] = 1;  // Produce
      counter++;  // Increment indexing counter
      timeout = clock () + 2 * CLOCKS_PER_SEC / 10000; // Reset deadlock timeout
    }
    else {
      cout << "Full buffer, no Produce. Timing out " << (timeout - clock()) << "...\n";
      // Let deadlock timout run
    }
    print();  // Print buffer
    sleep(1);

    if(clock() > timeout) {  // If current clock time > timeout, break the loop.
      cout << "\nDeadlock detected! Breaking out!\n\n";
      sleep(1);
      counter--;
      break;
    }
  }
}

void Consumer() {
  time_t timeout;
  
  while(true) {
    DetermineBools();  // Determine if you can consume
    if(consume) {
      cout << "Consumed 1\n";
      buffer[counter] = 0;  // Consume
      counter--;  // Decrement indexing counter
      timeout = clock() + 2 * CLOCKS_PER_SEC / 10000;  // Reset deadlock timeout
    }
    else {
      cout << "Empty buffer, no Consume. Timing out " << (timeout - clock()) << "...\n";
      // Let deadlock timout run
    }
    print();  // Print buffer
    sleep(1);
    
    if(clock() > timeout) {  // If current clock time > timeout, break the loop.
      cout << "\nDeadlock detected! Breaking out!\n\n";
      sleep(1);
      counter++;
      break;
    }
  }
}

int main() {

  while(true) {
    Producer();
    Consumer();
  }
  
}
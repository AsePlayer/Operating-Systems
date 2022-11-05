// Ryan Scott's Work
// CST-315
// Ricardo Citro
// 1/23/2022

/*
   I took the approach of breaking this problem down into smaller and smaller pieces. First, I did some research on pthreads and their methods, bouncing between a few resources and tutorials. After that, I started with implementing one thread, and after successfully making that work, implemented the second thread (using colored cout for tests). From there, I created the "Producing" and "Consuming" aspect, opting to use random numbers with ctime as the production.
*/

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <ctime>

using namespace std;

#define SIZE 1000
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"

static pthread_mutex_t mutex;

int producedArray[SIZE];
int producedCount;
int consumedCount;

void* Producer(void *id) {
  int randomNumber;

  // Lock thread until our logic is complete
  pthread_mutex_lock(&mutex);

  // Don't exceed array size
  if(producedCount >= SIZE) {
      pthread_mutex_unlock(&mutex);
      return NULL;
  }

  // "Produces" random number
  randomNumber = rand() % 1000 + 1;
  cout << GREEN << "Produced random number: " << randomNumber << endl;
  producedArray[producedCount] = randomNumber;
  producedCount++;
  sleep(1);
  
  // Logic is complete, safe to unlock thread
  pthread_mutex_unlock(&mutex);

  // Exit thread
  pthread_exit(NULL);
}

void* Consumer(void *id) {
  int numberToConsume;

  // Lock thread until our logic is complete
  pthread_mutex_lock(&mutex);

  // Don't exceed array size and don't consume beyond production
  if(consumedCount >= SIZE || consumedCount >= producedCount) {
      pthread_mutex_unlock(&mutex);
      return NULL;
  }

  // "Consumes" random number
  numberToConsume = producedArray[consumedCount];
  cout << RED << "Consumed random number: " << numberToConsume << endl;
  producedArray[consumedCount] = (int)NULL;
  consumedCount++;
  sleep(1);

  // Logic is complete, safe to unlock thread
  pthread_mutex_unlock(&mutex);

  // Exit thread
  pthread_exit(NULL);
}


int main() {
  int producerThreads;
  int consumerThreads;

  // User input for producerThreads and consumerThreads
  cout << "Enter amount of producer threads: ";
  cin >> producerThreads;
  cout << "\nEnter amount of consumer threads: ";
  cin >> consumerThreads;
  cout << endl;

  // Initialize random seed for Producer random numbers
  srand(time(NULL));

  // Initialize producer and consumer arrays with size from user input
  pthread_t producers[producerThreads];
  pthread_t consumers[consumerThreads];

  // Don't stop til you're numb
  while (true){

    for(int i = 0; i < producerThreads; i++) {
      // Creates producer thread with the following parameters:
      // (thread, attributes (NULL = default), start function, arg passed to start function)
      pthread_create(&producers[i], NULL, Producer, (void*)i);
    }

    for(int i = 0; i < consumerThreads; i++) {
      // Creates consumer thread with the following parameters:
      // (thread, attributes (NULL = default), start function, arg passed to start function)
      pthread_create(&consumers[i], NULL, Consumer, (void*)i);
    }

    for(int i = 0; i < producerThreads; i++) {
      // Waiting for producer thread to terminate
      void* status;
      pthread_join(producers[i], &status);
    }

    for(int i = 0; i < consumerThreads; i++) {
      // Waiting for consumer thread to terminate
      void* status;
      pthread_join(consumers[i], &status);
    }

  }

  return 0;

}

#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <ctime>
using namespace std;

enum State { BLOCKED, READY, RUNNING };
#define BURST 5
#define BUFFER 10

int burstCounter = BURST;

class Node {
public:
    State currentState = BLOCKED;
    string name;
    int burst;
    bool isIO;
    Node* next;
};

Node* HeadReadyList = NULL; //Ready state linked list
Node* HeadBlockList = NULL; //Blocked state linked list
Node* RunningNode = NULL;   //Running state linked list

static pthread_mutex_t mutex;


// Counts number of Nodes in linked list
int getSize(Node* head)
{
    int count = 0; // Initialize count
    Node* current = head; // Initialize current
    while (current != NULL)
    {
        count++;
        current = current->next;
    }
    return count;
}

void* createProcess(void* thread_arg) {

    pthread_mutex_lock(&mutex); //Lock thread until our logic is complete

    Node** head = (Node**)thread_arg;

    // Waiting for I/O for process burst time.
    string name;
    int burst;
    bool IO;
    // Generate random process
    char c;
    int r;

    for (int i = 0; i < 3; i++) {
        r = rand() % 26;   // generate a random number
        c = 'A' + r;       // Convert to a character from a-z
        name += c;
    }
    burst = rand() % 10 + 1;

    // is IO process
    if (rand() % 9 < 3) {
        IO = true;
        name += " I/O";
    }

    // Randomly wait to remove from blocked queue.
    while (true) {
        if (rand() % 10 < 4) //random event timer
            break;
        //cout << "Process " << name << " with burst " << burst << " waiting for I/O...\n";
        //sleep(1);
    }

    Node* newNode = new Node();
    newNode->name = name;
    newNode->burst = burst;
    newNode->isIO = IO;
    newNode->currentState = BLOCKED;

    if (getSize((Node*)head) > BUFFER)
    {
        pthread_mutex_unlock(&mutex);
        pthread_exit(NULL); //return;
    }

    // special case for the head end
    if (*head == NULL || (*head)->burst >= newNode->burst)
    {
        newNode->next = *head;
        *head = newNode;
        // Logic is complete, safe to unlock thread
        pthread_mutex_unlock(&mutex);
        pthread_exit(NULL); //return;
    }

    // locate the node before the point of insertion
    struct Node* current = *head;
    while (current->next != NULL && current->next->burst < newNode->burst) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
    // Logic is complete, safe to unlock thread
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}


// Function to insert a given node at its correct sorted position into
// a given list sorted in increasing order
// https://www.techiedelight.com/sorted-insert-in-linked-list/
void insert(struct Node** head, string name, int burst, bool isIO)
{
    Node* newNode = new Node();
    newNode->name = name;
    newNode->burst = burst;
    newNode->isIO = isIO;

    // special case for the head end
    if (*head == NULL || (*head)->burst >= newNode->burst)
    {
        newNode->next = *head;
        *head = newNode;
        return;
    }

    // locate the node before the point of insertion
    struct Node* current = *head;

    while (current->next != NULL && current->next->burst < newNode->burst) {
        if (newNode->isIO)
            break;
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
}

void* runProcess(void* thread_id) {
    // If there is no RunningNode and the Ready Queue isn't empty
    Node* temp = HeadReadyList;

    if (RunningNode == NULL && temp != NULL) {
        // Take from Ready Queue and add to RunningNode
        insert(&RunningNode, HeadReadyList->name, HeadReadyList->burst, HeadReadyList->isIO);
        HeadReadyList = HeadReadyList->next;
        //delete temp;
        RunningNode->currentState = RUNNING;
        cout << "\nProcess " << RunningNode->name << " with burst " << RunningNode->burst << " added to Running.\n\n";
    }

    // If there is a process to run, run it and reduce burst by 1.
    if (RunningNode != NULL) {
        cout << "Running Process " << RunningNode->name << " with time " << RunningNode->burst << ". ";
        RunningNode->burst = RunningNode->burst - 1;
        burstCounter--;
        cout << "Reduced to " << RunningNode->burst << endl;
        sleep(1);

        if (RunningNode->burst == 0) {
            cout << endl << "Process " << RunningNode->name << " finished. Removing." << endl << endl;
            sleep(1);
            RunningNode->currentState = BLOCKED;
            RunningNode = NULL;
            burstCounter = BURST;

        }
        else if (burstCounter == 0) {
            cout << endl << "Process " << RunningNode->name << " exceeded time quantum. Moving to Ready Queue." << endl << endl;
            RunningNode->currentState = READY;
            insert(&HeadReadyList, RunningNode->name, RunningNode->burst, RunningNode->isIO);
            RunningNode = NULL;
            burstCounter = BURST;
        }
    }
    else {
        cout << "Running Node is empty. Waiting for process..." << endl;
        sleep(1);
    }
    pthread_exit(NULL);
}

void* blockedToReady(void* thread_id) {
    Node* temp2 = HeadBlockList;
    pthread_mutex_lock(&mutex); //Lock thread until our logic is complete
    while (HeadBlockList != NULL && getSize((Node*)HeadReadyList) <= BUFFER) {
        HeadBlockList->currentState = READY;
        insert(&HeadReadyList, HeadBlockList->name, HeadBlockList->burst, HeadBlockList->isIO);
        cout << "\nTransferring " << HeadBlockList->name << " with time " << HeadBlockList->burst << " to Ready Queue.\n ";
        HeadBlockList = HeadBlockList->next;

    }
    pthread_mutex_unlock(&mutex); //Logic complete. Unlock thread.
    //delete temp2;
    return NULL;
    pthread_exit(NULL);
}




int main() {
    srand(time(NULL));
    pthread_t threads[3];

    int r;
    int s;
    int t;

    while (true) {
        r = pthread_create(&threads[0], NULL, blockedToReady, (void*)0);
        s = pthread_create(&threads[1], NULL, &createProcess, (void*)&HeadBlockList);
        t = pthread_create(&threads[2], NULL, runProcess, (void*)2);
        pthread_join(threads[0], NULL);
        pthread_join(threads[1], NULL);
        pthread_join(threads[2], NULL);
    }
    pthread_exit(NULL);
}

#include <bits/stdc++.h>
#include <atomic>
using namespace std;

#define NUM_THREADS 5

//choosing stores whether a thread is in the process of getting a ticket
atomic<bool> choosing[NUM_THREADS];
//number stores the ticket number for each thread
atomic<int> number [NUM_THREADS];

void lock(int thread_id){
    choosing[thread_id] = true;
    int max_number = 0;

    for (auto & i : number){
        int thread_number = i;
        if (thread_number>max_number) {
            max_number = thread_number;
        }
    }

    //get the next ticket number = largest ticket assigned so far + 1
    number[thread_id] = max_number+1;
    choosing[thread_id] = false;

    //Wait for turn to acquire lock
    //for every other thread wait till it is done choosing and till the current thread's number arrives
    for (int other_id=0;other_id<NUM_THREADS;other_id++){
        while (choosing[other_id]){
            //Busy Wait
        }
        //Wait while the number of the other thread is lesser than the current thread
        //if the number is equal wait if the id of the other thread is lesser than the current thread
        while (number[other_id]!=0 && ((number[other_id]<number[thread_id])||(number[other_id]==number[thread_id] && other_id<thread_id))){
            //Busy Wait
        }
    }
}

//number = 0 indicates that the thread has released the lock
void unlock (int thread_id) {
    number[thread_id] = 0;
}

void criticalSection (int thread_id) {
    cout<<"Thread "<<thread_id<<" is inside the critical section"<<endl;
}

void threadFunction(int thread_id) {
    //Each thread tries to enter the critical section and exit it multiple times
    for (int i=0;i<5;i++) {
        lock(thread_id);
        criticalSection(thread_id);
        unlock(thread_id);

        //Non-critical section
        this_thread::sleep_for(chrono::seconds(1));
    }
}

int main(){
    thread threads[NUM_THREADS];

    for (int i=0;i<NUM_THREADS;i++) {
        choosing[i]=false;
        number[i] = 0;
    }

    //Create threads for each

    for (int i=0;i<NUM_THREADS;i++) {
        threads[i] = thread(threadFunction, i);
        //threadFunction is executed on the newly created thread with id i
    }

    //Joining threads
    for (auto & thread : threads) {
        thread.join();
    }
    return 0;
}


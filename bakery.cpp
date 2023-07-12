#include <bits/stdc++.h>
#include <atomic>
using namespace std;

#define NUM_THREADS 4

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

    number[thread_id] = max_number+1;
    choosing[thread_id] = false;

    //Wait for turn to acquire lock
    for (int other_id=0;other_id<NUM_THREADS;other_id++){
        while (choosing[other_id]){
            //Busy Wait
        }

        while (number[other_id]!=0 && ((number[other_id]<number[thread_id])||(number[other_id]==number[thread_id] && other_id<thread_id))){
            //Busy Wait
        }
    }
}

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
        this_thread::sleep_for(chrono::milliseconds(500));
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
        //thread function is executed on the newly created thread with id i
    }

    //Joining threads
    for (auto & thread : threads) {
        thread.join();
    }
    //We wait for each thread to complete execution before moving on to the next thread
    //That is, we wait for the threadFunction to execute for a particular thread_id completely before going to the next thread_id
    //Thus, at any point only one thread_id is trying to acquire the lock and enter its critical section

    return 0;
}

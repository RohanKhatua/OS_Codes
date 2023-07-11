// Peterson's solution for the reader's writer's problem
// Two processes try to modify and read the value of a shared resource

#include <bits/stdc++.h>
#include <thread>
#include <atomic>
#include <pthread.h>
using namespace std;

// Global Variables
atomic<int> turn(0);
atomic<bool> flag0(false);
atomic<bool> flag1(false);

int shared_resource = 0;

// Process 0
void process0()
{
    flag0 = true;
    turn = 1;
    while (flag1 && turn == 1)
    {
        // Waiting
    }
    // Critical Section
    shared_resource++;
    cout << "Process 0: Shared Resource = " << shared_resource << endl;
    flag0 = false;
}

void process1()
{
    flag1 = true;
    turn = 0;
    while (flag0 && turn == 0)
    {
        // Waiting
    }
    // Critical Section
    shared_resource--;
    cout << "Process 1: Shared Resource = " << shared_resource << endl;

    flag1 = false;
}

int main()
{
    thread t0(process0);
    thread t1(process1);

    t0.join();
    t1.join();

    return 0;
}
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

const int numPhilosophers = 5;

class DiningPhilosophersMonitor {
public:
    DiningPhilosophersMonitor() {
        for (int i = 0; i < numPhilosophers; ++i) {
            forks[i] = true; // true means the fork is available
        }
    }

    void pickUpForks(int philosopherID) {
        std::unique_lock<std::mutex> lock(mutex);

        while (!forks[philosopherID] || !forks[(philosopherID + 1) % numPhilosophers]) {
            condition.wait(lock);
        }

        forks[philosopherID] = false;
        forks[(philosopherID + 1) % numPhilosophers] = false;
    }

    void putDownForks(int philosopherID) {
        std::unique_lock<std::mutex> lock(mutex);

        forks[philosopherID] = true;
        forks[(philosopherID + 1) % numPhilosophers] = true;

        condition.notify_all();
    }

private:
    bool forks[numPhilosophers];
    std::mutex mutex;
    std::condition_variable condition;
};

void philosopher(int philosopherID, DiningPhilosophersMonitor& monitor) {
    for (int i = 0; i < 5; ++i) {
        // Thinking
        std::cout << "Philosopher " << philosopherID << " is thinking." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Hungry, trying to pick up forks
        monitor.pickUpForks(philosopherID);
        std::cout << "Philosopher " << philosopherID << " picked up the forks and is eating." << std::endl;

        // Eating
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // Done eating, putting down forks
        monitor.putDownForks(philosopherID);
        std::cout << "Philosopher " << philosopherID << " put down the forks." << std::endl;
    }
}

int main() {
    DiningPhilosophersMonitor monitor;
    std::thread philosophers[numPhilosophers];

    for (int i = 0; i < numPhilosophers; ++i) {
        philosophers[i] = std::thread(philosopher, i, std::ref(monitor));
    }

    for (int i = 0; i < numPhilosophers; ++i) {
        philosophers[i].join();
    }

    return 0;
}

#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore.h>

const int numPhilosophers = 5;

class DiningPhilosophers {
public:
    DiningPhilosophers() {
        for (int i = 0; i < numPhilosophers; ++i) {
            forks[i] = new std::mutex();
        }
    }

    void pickUpForks(int philosopherID) {
        sem_wait(&mutex);
        sem_wait(&forksSem[philosopherID]);
        sem_wait(&forksSem[(philosopherID + 1) % numPhilosophers]);
        sem_post(&mutex);
    }

    void putDownForks(int philosopherID) {
        sem_post(&forksSem[philosopherID]);
        sem_post(&forksSem[(philosopherID + 1) % numPhilosophers]);
    }

    std::mutex* forks[numPhilosophers];
    sem_t mutex;
    sem_t forksSem[numPhilosophers];
};

void philosopher(int philosopherID, DiningPhilosophers& dining) {
    for (int i = 0; i < 5; ++i) {
        // Thinking
        std::cout << "Philosopher " << philosopherID << " is thinking." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Hungry, trying to pick up forks
        dining.pickUpForks(philosopherID);
        std::cout << "Philosopher " << philosopherID << " picked up the forks and is eating." << std::endl;

        // Eating
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // Done eating, putting down forks
        dining.putDownForks(philosopherID);
        std::cout << "Philosopher " << philosopherID << " put down the forks." << std::endl;
    }
}

int main() {
    DiningPhilosophers dining;
    std::thread philosophers[numPhilosophers];

    for (int i = 0; i < numPhilosophers; ++i) {
        sem_init(&dining.forksSem[i], 0, 1);
    }
    sem_init(&dining.mutex, 0, numPhilosophers - 1);

    for (int i = 0; i < numPhilosophers; ++i) {
        philosophers[i] = std::thread(philosopher, i, std::ref(dining));
    }

    for (int i = 0; i < numPhilosophers; ++i) {
        philosophers[i].join();
    }

    return 0;
}

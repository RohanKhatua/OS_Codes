#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>

const int numPhilosophers = 5;

class DiningPhilosophers {
public:
    DiningPhilosophers() : forks(numPhilosophers), philosophers(numPhilosophers), graph(numPhilosophers) {
        for (int i = 0; i < numPhilosophers; ++i) {
            philosophers[i] = std::thread(&DiningPhilosophers::philosopher, this, i);
        }
    }

    ~DiningPhilosophers() {
        for (int i = 0; i < numPhilosophers; ++i) {
            philosophers[i].join();
        }
    }

private:
    std::vector<std::mutex> forks;
    std::vector<std::thread> philosophers;
    std::mutex graphMutex;
    std::vector<std::vector<int>> graph;

    void printWaitForGraph() {
        std::unique_lock<std::mutex> lock(graphMutex);
        std::cout << "Wait-for graph:" << std::endl;
        for (int i = 0; i < numPhilosophers; ++i) {
            std::cout << "Fork " << i << " is being waited by philosophers: ";
            for (int p : graph[i]) {
                std::cout << p << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    void philosopher(int philosopherID) {
        int leftFork = philosopherID;
        int rightFork = (philosopherID + 1) % numPhilosophers;

        while (true) {
            // Thinking
            std::cout << "Philosopher " << philosopherID << " is thinking." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            // Picking up forks
            std::unique_lock<std::mutex> leftLock(forks[leftFork]);
            std::unique_lock<std::mutex> rightLock(forks[rightFork]);

            // Update wait-for graph
            {
                std::unique_lock<std::mutex> lock(graphMutex);
                graph[leftFork].push_back(philosopherID);
                graph[rightFork].push_back(philosopherID);
            }

            // Print the wait-for graph
            printWaitForGraph();

            // Eating
            std::cout << "Philosopher " << philosopherID << " is eating." << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            // Putting down forks
            leftLock.unlock();
            rightLock.unlock();

            // Update wait-for graph
            {
                std::unique_lock<std::mutex> lock(graphMutex);
                graph[leftFork].clear();
                graph[rightFork].clear();
            }

            // Print the wait-for graph
            printWaitForGraph();
        }
    }
};

int main() {
    DiningPhilosophers dining;
    std::this_thread::sleep_for(std::chrono::seconds(10)); // Let the philosophers eat for 10 seconds
    return 0;
}

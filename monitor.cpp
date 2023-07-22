#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

class Monitor {
public:
    Monitor() : turn(1) {}

    void increment(int threadID) {
        std::unique_lock<std::mutex> lock(mutex);
        while (turn != threadID) {
            condition.wait(lock);
        }

        // Perform the increment
        sharedVariable++;
        std::cout << "Thread " << threadID << " incremented the variable. New value: " << sharedVariable << std::endl;

        // Change turn for the next thread
        turn = (threadID == 1) ? 2 : 1;

        // Notify the waiting thread
        condition.notify_one();
    }

private:
    int sharedVariable = 0;
    int turn;
    std::mutex mutex;
    std::condition_variable condition;
};

void threadFunction(Monitor& monitor, int threadID) {
    for (int i = 0; i < 5; ++i) {
        monitor.increment(threadID);
        // Simulate some work being done
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main() {
    Monitor monitor;
    std::thread thread1(threadFunction, std::ref(monitor), 1);
    std::thread thread2(threadFunction, std::ref(monitor), 2);

    thread1.join();
    thread2.join();

    return 0;
}

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

class Semaphore {
public:
    Semaphore(int count = 1) : count_(count) {}

    void acquire() {
        std::unique_lock<std::mutex> lock(mutex_);
        condition_.wait(lock, [this] { return count_ > 0; });
        count_--;
    }

    void release() {
        std::lock_guard<std::mutex> lock(mutex_);
        count_++;
        condition_.notify_one();
    }

private:
    int count_;
    std::mutex mutex_;
    std::condition_variable condition_;
};

// Shared resource
int shared_data = 0;

void producer(Semaphore& sem) {
    for (int i = 0; i < 5; i++) {
        sem.acquire();
        shared_data++;
        std::cout << "Producer: Produced data = " << shared_data << std::endl;
        sem.release();
        // Simulate some work being done
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void consumer(Semaphore& sem) {
    for (int i = 0; i < 5; i++) {
        sem.acquire();
        std::cout << "Consumer: Consumed data = " << shared_data << std::endl;
        sem.release();
        // Simulate some work being done
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}

int main() {
    Semaphore semaphore;

    std::thread producerThread(producer, std::ref(semaphore));
    std::thread consumerThread(consumer, std::ref(semaphore));

    producerThread.join();
    consumerThread.join();

    return 0;
}

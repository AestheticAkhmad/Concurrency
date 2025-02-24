#include <atomic>
#include <thread>
#include <iostream>
#include <vector>

// Simple spin-lock class
class SpinLock {
    // atomic_flag provides a simple atomic boolean flag.
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
public:
    // The lock method continuously attempts to set the flag.
    // test_and_set returns the previous value, so if it was already set,
    // the loop continues to "spin" until the flag becomes clear.
    void lock() {
        while (flag.test_and_set(std::memory_order_acquire)) {
            // Optionally, you can insert a pause or yield instruction here
            // to reduce CPU usage during the busy wait.
        }
    }

    // The unlock method simply clears the flag, allowing other threads to acquire the lock.
    void unlock() {
        flag.clear(std::memory_order_release);
    }
};

int main() {
    SpinLock spinlock;
    int counter = 0;
    const int iterations = 100000;
    const int num_threads = 4;
    std::vector<std::thread> threads;

    // Each thread will increment the shared counter while using the spin-lock
    auto increment = [&]() {
        for (int i = 0; i < iterations; ++i) {
            // Each thread will lock the spin-lock before incrementing the counter
            spinlock.lock();
            ++counter;
            // After incrementing the counter, the thread will unlock the spin-lock
            spinlock.unlock();
        }
    };

    // Launch multiple threads
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(increment);
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Final counter value: " << counter << std::endl;
    return 0;
}

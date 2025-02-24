#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main() {
    int counter = 0;
    std::mutex mtx; // Mutex to protect the critical section.
    const int iterations = 100000;
    const int num_threads = 4;
    std::vector<std::thread> threads;

    // Each thread increments the counter within a critical section protected by the mutex.
    auto increment = [&]() {
        for (int i = 0; i < iterations; ++i) {
            // std::lock_guard locks the mutex and automatically unlocks it when the guard goes out of scope.
            std::lock_guard<std::mutex> lock(mtx);
            ++counter;
        }
    };

    // Create and start threads.
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(increment);
    }

    // Wait for all threads to finish.
    for (auto& th : threads) {
        th.join();
    }

    std::cout << "Final counter value: " << counter << std::endl;
    return 0;
}

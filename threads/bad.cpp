
#include <atomic>
#include <iostream>

// high contention 1
std::atomic<unsigned long> counter(0);
void processing_loop() {
    while (counter.fetch_add(1, std::memory_order_relaxed) < 100000000) {
        std::cout << "hi\n";
    }
}

// high contention 2
std::mutex m;
void processing_loop_with_mutex() {
    while (true) {
        std::lock_guard<std::mutex> lk(m);
    }
}

// false sharing

int main() {
}

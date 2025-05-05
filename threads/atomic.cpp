#include <atomic>
#include <iostream>
#include <thread>
#include <unordered_set>

std::mutex mut;
std::unordered_set<int> seen;

void countUp(std::atomic<int> &i, int n) {
    while (true) {
        int local_i = i.fetch_add(1);
        if (local_i >= n)
            break;
        std::lock_guard<std::mutex> guard(mut);
        seen.insert(local_i);
    }
}

int main() {
    int n;
    std::cin >> n;
    std::atomic<int> i = 0;
    std::thread t1(countUp, ref(i), n);
    std::thread t2(countUp, ref(i), n);
    std::thread t3(countUp, ref(i), n);

    t1.join();
    t2.join();
    t3.join();

    std::cout << seen.size() << '\n';
}

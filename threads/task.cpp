#include <climits>
#include <future>
#include <iostream>

int do_thing() {
    for (int i = 0; i < INT_MAX; i++) {
    }
    std::cout << "ran thing\n";
    return 1;
}

void accesser(std::shared_future<int> future) {
    std::cout << "accesser: " << future.get() << "\n";
}

int main() {
    std::packaged_task<int()> task(do_thing);
    std::shared_future<int> future = task.get_future().share();
    std::thread t1(std::move(task));

    std::thread t2(accesser, future);
    t2.join();

    std::cout << "main: " << future.get() << '\n';

    t1.join();
}

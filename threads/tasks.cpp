#include <future>
#include <iostream>
#include <thread>

using namespace std;

int comp2() {
    for (int i = 0; i < INT_MAX; i++) {
    }
    return 30;
}

promise<int> prom;
future<int> fut = prom.get_future();

packaged_task<int()> task{comp2};
future<int> fut2 = task.get_future();

void compute() {
    for (int i = 0; i < INT_MAX; i++) {
    }
    prom.set_value(30);
}

void reader() {
    int res = fut.get();
    cout << res << "\n";
}

void reader2() {
    int res = fut2.get();
    cout << res << "\n";
}

int main() {
    thread t1(compute);
    thread t2(reader);
    t1.join();
    t2.join();

    thread t3{reader2};
    task();
    t3.join();
}

#include <atomic>
#include <iostream>
#include <thread>
using namespace std;

atomic<int> x, y, z;

void write_x() {
    x.store(10);
}
void write_y() {
    y.store(100);
}
void read_y() {
    while (!x.load()) {
    }
    if (y.load()) {
        z.store(100);
    }
}
void read_x() {
    while (!y.load()) {
    }
    if (x.load()) {
        z.store(10);
    }
}

int main() {
    thread t1(write_x);
    thread t2(write_y);
    thread t3(read_y);
    thread t4(read_x);

    // we know write before read
    // unsure if x or y first though

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    cout << z.load() << "\n";
}

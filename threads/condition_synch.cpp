#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

using namespace std;

queue<int> q;
condition_variable q_ready;
mutex mut;
bool done = false;

void adder(int n) {
    for (int i = 0; i < n; i++) {
        lock_guard<mutex> g(mut);
        q.push(i);
        q_ready.notify_one();
    }
    lock_guard<mutex> g(mut);
    done = true;
    q_ready.notify_one();
}

void processer(int n) {
    while (true) {
        unique_lock<mutex> g(mut);
        q_ready.wait(g, []() { return !q.empty() || done; });
        if (q.empty())
            break;
        int val = q.front();
        cout << val << " ";
        q.pop();
        g.unlock();
    }
    cout << "\n";
}

int main() {
    int n;
    cin >> n;
    thread t1(adder, n);
    thread t2(processer, n);

    t1.join();
    t2.join();
}

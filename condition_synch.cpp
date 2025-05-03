#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

using namespace std;

queue<int> q;
condition_variable q_ready;
mutex mut;

void adder(int &i, int n) {
    for (; i < n; i++) {
        lock_guard<mutex> g(mut);
        q.push(i);
    }
}

void processer(int &i, int n) {
    while (i < n || !q.empty()) {
        unique_lock<mutex> g(mut);
        q_ready.wait(g, []() { return !q.empty(); });
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
    int i = 0;
    thread t1(adder, ref(i), n);
    thread t2(processer, ref(i), n);

    t1.join();
    t2.join();
}

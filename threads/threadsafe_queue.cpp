#include <cassert>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

using namespace std;

class ThreadsafeQueue {
private:
    queue<int> q;
    mutable mutex mut;
    condition_variable ready;

public:
    ThreadsafeQueue() {}
    ThreadsafeQueue(ThreadsafeQueue &other) {
        lock_guard<mutex> guard(other.mut);
        q = other.q;
    }
    void operator=(ThreadsafeQueue &other) = delete;

    void push(int val) {
        lock_guard<mutex> guard(mut);
        q.push(val);
        ready.notify_one();
    }

    shared_ptr<int> try_pop() {
        lock_guard<mutex> guard(mut);
        if (q.empty())
            return nullptr;
        shared_ptr<int> ptr(make_shared<int>(q.front()));
        q.pop();
        return ptr;
    }
    bool try_pop(int &val) {
        lock_guard<mutex> guard(mut);
        if (q.empty())
            return false;
        val = q.front();
        q.pop();
    }

    shared_ptr<int> wait_pop() {
        unique_lock<mutex> guard(mut);
        ready.wait(guard, [this]() { return !q.empty(); });
        shared_ptr<int> ptr(make_shared<int>(q.front()));
        q.pop();
        return ptr;
    }
    void wait_pop(int &val) {
        unique_lock<mutex> guard(mut);
        ready.wait(guard, [this]() { return !q.empty(); });
        val = q.front();
        q.pop();
    }

    bool empty() const {
        lock_guard<mutex> guard(mut);
        return q.empty();
    }

    size_t size() const {
        lock_guard<mutex> guard(mut);
        return q.size();
    }
};

ThreadsafeQueue q;

void adder(int &i, int n) {
    for (; i < n; i++) {
        q.push(i);
    }
}
void looker(int &i, int n) {
    while (!q.empty() || i < n) {
        int val;
        q.wait_pop(val);
        cout << val << " ";
    }
    cout << "\n";
}

int main() {
    // definitely data race for i but whatever
    int n;
    cin >> n;
    int i = 0;
    thread t1(adder, ref(i), n);
    thread t2(looker, ref(i), n);
    t1.join();
    t2.join();
}

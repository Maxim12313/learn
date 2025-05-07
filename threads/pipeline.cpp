#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>

using namespace std;

class ThreadsafeQueue {
private:
    queue<int> q;
    mutex mtx;
    condition_variable cv;

public:
    ThreadsafeQueue(const ThreadsafeQueue &other) = delete;
    ThreadsafeQueue &operator=(const ThreadsafeQueue &other) = delete;
    ThreadsafeQueue() {}

    void push(int val) {
        lock_guard<mutex> lck(mtx);
        q.push(val);
        cv.notify_one();
    }

    bool wait_pop(int &val) {
        unique_lock<mutex> lck(mtx);
        cv.wait_for(lck, chrono::milliseconds(100),
                    [this] { return !q.empty(); });
        if (q.empty())
            return false;
        val = q.front();
        q.pop();
        return true;
    }
};

class Pipeline {
private:
    vector<thread> pipes;
    vector<unique_ptr<ThreadsafeQueue>> queues;
    atomic<bool> done;

    // pop from i, push to i+1
    void run_pipe(int i, function<int(int)> func) {
        ThreadsafeQueue *receiver = queues[i].get();
        ThreadsafeQueue *sender = queues[i + 1].get();
        while (!done) {
            int input;
            if (!receiver->wait_pop(input))
                continue;
            int res = func(input);
            sender->push(res);
        }
    }

public:
    Pipeline(vector<function<int(int)>> &funcs) : done(false) {
        int n = funcs.size();
        queues.resize(n + 1);
        for (int i = 0; i <= n; i++)
            queues[i] = make_unique<ThreadsafeQueue>();

        for (int i = 0; i < n; i++) {
            pipes.emplace_back([funcs, i, this] { run_pipe(i, funcs[i]); });
        }
    }

    Pipeline(const Pipeline &other) = delete;
    Pipeline &operator=(const Pipeline &other) = delete;

    void push(int val) { queues[0]->push(val); }
    bool wait_pop(int &val) { return queues.back()->wait_pop(val); }

    ~Pipeline() {
        done.store(true);
        for (thread &t : pipes) {
            t.join();
        }
    }
};

int double_val(int val) {
    return val * 2;
}

int add_val(int val) {
    return val + 1;
}

int main() {

    // y = 2x + 3
    vector<function<int(int)>> pipes = {add_val, add_val, add_val, add_val};
    Pipeline pipeline(pipes);
    for (int i = 0; i < 1024; i++) {
        pipeline.push(i);
        int res;
        if (pipeline.wait_pop(res)) {
            cout << res << "\n";
        }
    }
}

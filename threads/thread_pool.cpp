#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

using namespace std;

class ThreadPool {
private:
    vector<thread> threads;
    queue<function<void()>> tasks;
    mutex mtx;
    condition_variable cv;
    atomic<bool> done;

    void thread_task() {
        while (true) {
            function<void()> task;
            {
                unique_lock<mutex> guard(mtx);
                cv.wait(guard, [this] { return done || !tasks.empty(); });
                if (done)
                    return;
                task = std::move(tasks.front());
                tasks.pop();
            }
            task(); // nothing if empty
        }
    }

public:
    ThreadPool(size_t n = thread::hardware_concurrency()) {
        for (int i = 0; i < n; i++) {
            threads.emplace_back([this]() { thread_task(); });
        }
    }

    // cannot be copied
    ThreadPool(const ThreadPool &other) = delete;
    ThreadPool &operator=(const ThreadPool &other) = delete;

    void push(function<void()> func) {
        lock_guard<mutex> guard(mtx);
        tasks.push(std::move(func));
        cv.notify_one();
    }

    void stop() { done.store(true); }

    ~ThreadPool() {
        cout << "ran\n";
        stop();
        cv.notify_all();
        for (thread &t : threads) {
            t.join();
            cout << "joined\n";
        }
    }
};

void sayHi() {
    cout << "hello world\n";
}

int main() {
    ThreadPool pool;
    for (int i = 0; i < 5; i++) {
        pool.push(sayHi);
    }
}

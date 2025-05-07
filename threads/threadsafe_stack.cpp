#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <exception>
#include <future>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <numeric>
#include <stack>
#include <thread>
#include <vector>

using namespace std;

class ThreadsafeStack {
private:
    stack<int> st;
    mutable mutex mut;
    condition_variable cond;

public:
    ThreadsafeStack() {}
    ThreadsafeStack(const ThreadsafeStack &other) {
        lock_guard<mutex> guard(mut);
        st = other.st;
    }
    ThreadsafeStack &operator=(const ThreadsafeStack &other) = delete;

    bool empty() const {
        lock_guard<mutex> guard(mut);
        return st.empty();
    }

    void push(int val) {
        lock_guard<mutex> guard(mut);
        st.push(val);
        cond.notify_one();
    }

    void wait_pop(int &val) {
        unique_lock<mutex> guard(mut);
        cond.wait(guard, [this]() { return !st.empty(); });
        val = std::move(st.top());
        st.pop();
    }

    shared_ptr<int> wait_pop() {
        unique_lock<mutex> guard(mut);
        cond.wait(guard, [this]() { return !st.empty(); });
        shared_ptr<int> res(make_shared<int>(st.top()));
        st.pop();
        return res;
    }

    void try_pop(int &val) {
        lock_guard<mutex> guard(mut);
        if (st.empty())
            return;
        val = st.top();
        st.pop();
    }

    shared_ptr<int> try_pop() {
        lock_guard<mutex> guard(mut);
        if (st.empty())
            return shared_ptr<int>();
        shared_ptr<int> res(make_shared<int>(st.top()));
        st.pop();
        return res;
    }
};

int main() {
}

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <exception>
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
    mutex mut;
    ThreadsafeStack() {}
    ThreadsafeStack(const ThreadsafeStack &other) {
        lock_guard<mutex> guard(mut);
        st = other.st;
    }
    ThreadsafeStack &operator=(const ThreadsafeStack &other) = delete;

public:
    bool empty() const { return st.empty(); }

    void push(int val) {
        lock_guard<mutex> guard(mut);
        st.push(val);
    }

    void pop(int &val) {
        lock_guard<mutex> guard(mut);
        assert(!st.empty());
        val = st.top();
        st.pop();
    }

    shared_ptr<int> pop() {
        lock_guard<mutex> guard(mut);
        assert(!st.empty());
        shared_ptr<int> res(make_shared<int>(st.top()));
        st.pop();
        return res;
    }
};

int main() {
}

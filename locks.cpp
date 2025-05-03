#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <exception>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <mutex>
#include <numeric>
#include <stack>
#include <thread>
#include <vector>

using namespace std;

struct Data;
void swap(Data &a, Data &b);

struct Data {
    int val;
    mutex mut;

    int getVal() {
        lock_guard<mutex> g(mut);
        return val;
    }

    friend void swap(Data &a, Data &b) {
        // undefined to aquire lock for self
        if (&a == &b)
            return;
        lock(a.mut, b.mut);
        lock_guard<mutex> g1(a.mut, adopt_lock);
        lock_guard<mutex> g2(b.mut, adopt_lock);
        swap(a.val, b.val);
    }
};

Data a, b;

void swapper(int &i) {
    for (; i < 10000; i++) {
        swap(a, b);
        if (i % 2 == 0) {
            assert(a.val == 1 && b.val == 100);
        } else {
            assert(b.val == 1 && a.val == 100);
        }
    }
}

int main() {
    a.val = 100;
    b.val = 1;
    int i = 0;
    for (int j = 0; j < 5; j++) {
        thread t(swapper, ref(i));
        t.detach();
    }
}

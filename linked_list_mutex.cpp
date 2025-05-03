#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <thread>
#include <vector>

using namespace std;

list<int> nums;
mutex numsMutex;

void insert_val(int i, int val) {
    auto it = begin(nums);
    advance(it, i);

    lock_guard<mutex> guard(numsMutex);
    nums.insert(it, val);
}

bool contains(int val) {
    lock_guard<mutex> guard(numsMutex);
    return find(begin(nums), end(nums), val) != end(nums);
}

void addOver(int n, int m) {
    for (int val = n; val <= n + m; val++) {
        insert_val(1, val);
    }
}

void checkContains(int n) {
    bool res = true;
    for (int val = 0; val < n; val++) {
        if (!contains(val)) {
            res = false;
            break;
        }
    }
    cout << (res ? "YES" : "NO") << "\n";
}

int main() {
    while (true) {
        cout << "<elements conttained> <elements added>\n";
        int n, m;
        cin >> n >> m;
        for (int val = 0; val < n; val++) {
            nums.push_back(val);
        }

        thread adder(addOver, n, m);
        thread checker(checkContains, n);

        adder.join();
        checker.join();
    }
}

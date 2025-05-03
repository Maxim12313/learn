#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

using namespace std;

void singleAcc(int l, int r, int64_t &result, vector<int> &nums) {
    result = accumulate(begin(nums) + l, begin(nums) + r, result);
}

int64_t allAcc(vector<int> &nums) {
    // int64_t total = 0;
    // thread t(singleAcc, 0, nums.size(), std::ref(total), std::ref(nums));
    // t.join();
    uint per = 5;
    uint desired = ceil(nums.size() / float(per));
    uint allowed = thread::hardware_concurrency();
    uint count = min(allowed, desired);
    vector<thread> threads(count);
    vector<int64_t> results(count, 0);
    for (int i = 0; i < count; i++) {
        int l = i * per;
        int r = min(nums.size(), size_t(l + per)); // exclusive
        threads[i] =
            thread(singleAcc, l, r, std::ref(results[i]), std::ref(nums));
    }
    for (int i = 0; i < count; i++) {
        threads[i].join();
    }
    int64_t total = accumulate(begin(results), end(results), 0LL);
    return total;
}

void test() {
    cout << "hello world\n";
}

int main() {
    while (true) {
        cout << "specify: n <n elements>\n";
        int n;
        cin >> n;
        vector<int> nums(n);
        for (int &val : nums)
            cin >> val;
        int64_t real = accumulate(begin(nums), end(nums), 0LL);
        int64_t got = allAcc(nums);
        cout << real << " " << got << "\n";
    }
}

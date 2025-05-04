#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <ratio>
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
    uint per = sqrt(nums.size());
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
    int n;
    cin >> n;
    vector<int> nums(n);
    for (int &val : nums)
        val = rand();

    auto t1 = chrono::high_resolution_clock::now();
    int64_t real = 0;
    singleAcc(0, n, real, nums);
    auto t2 = chrono::high_resolution_clock::now();
    int time1 = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();

    auto t3 = chrono::high_resolution_clock::now();
    int64_t got = allAcc(nums);
    auto t4 = chrono::high_resolution_clock::now();
    int time2 = chrono::duration_cast<chrono::milliseconds>(t4 - t3).count();

    cout << time1 << " " << time2 << "\n";
}

int main() {
    test();
    // while (true) {
    //     cout << "specify: n <n elements>\n";
    //     int n;
    //     cin >> n;
    //     vector<int> nums(n);
    //     for (int &val : nums)
    //         cin >> val;
    //     auto t1 = chrono::high_resolution_clock::now();
    //     int64_t real = accumulate(begin(nums), end(nums), 0LL);
    //     auto t2 = chrono::high_resolution_clock::now();
    //     int time1 = (t2 - t1).count();
    //
    //     auto t3 = chrono::high_resolution_clock::now();
    //     int64_t got = allAcc(nums);
    //     auto t4 = chrono::high_resolution_clock::now();
    //     int time2 = (t4 - t3).count();
    //
    //     cout << time1 << " " << time2 << "\n";
    // }
}

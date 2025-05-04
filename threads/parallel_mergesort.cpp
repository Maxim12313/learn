#include <cassert>
#include <chrono>
#include <future>
#include <iostream>
#include <vector>

using namespace std;

vector<int> merge(vector<int> a, vector<int> b) {
    vector<int> res(a.size() + b.size());
    int j = 0;
    int k = 0;
    for (int i = 0; i < res.size(); i++) {
        if (j < a.size() && k < b.size()) {
            if (a[j] < b[k]) {
                res[i] = a[j++];
            } else {
                res[i] = b[k++];
            }
        } else if (j < a.size()) {
            res[i] = a[j++];
        } else {
            res[i] = b[k++];
        }
    }
    return res;
}

vector<int> mergesort(vector<int> nums) {
    if (nums.size() <= 1)
        return nums;

    int mid = nums.size() / 2;

    vector<int> lo(begin(nums), begin(nums) + mid);
    vector<int> hi(begin(nums) + mid, end(nums));

    lo = mergesort(std::move(lo));
    hi = mergesort(std::move(hi));

    return merge(std::move(lo), std::move(hi));
}

vector<int> parallel_mergesort(vector<int> nums) {
    if (nums.size() <= 1)
        return nums;

    uint threads = thread::hardware_concurrency();
    if (nums.size() <= 1000 * threads || threads < 2) {
        return mergesort(nums);
    }

    int mid = nums.size() / 2;

    vector<int> lo(begin(nums), begin(nums) + mid);
    vector<int> hi(begin(nums) + mid, end(nums));

    auto lfuture = async(launch::async, parallel_mergesort, std::move(lo));
    auto rfuture = async(launch::async, parallel_mergesort, std::move(hi));

    lo = lfuture.get();
    hi = rfuture.get();

    return merge(std::move(lo), std::move(hi));
}

int main() {
    int n;
    cin >> n;
    vector<int> A(n);
    for (int &val : A) {
        val = rand();
    }
    auto t1 = chrono::high_resolution_clock::now();
    vector<int> test1 = mergesort(A);
    auto t2 = chrono::high_resolution_clock::now();
    int time1 = chrono::duration_cast<chrono::milliseconds>(t2 - t1).count();

    auto t3 = chrono::high_resolution_clock::now();
    vector<int> test2 = parallel_mergesort(A);
    auto t4 = chrono::high_resolution_clock::now();
    int time2 = chrono::duration_cast<chrono::milliseconds>(t4 - t3).count();

    cout << time1 << " " << time2 << "\n";
}

#include <cassert>
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
    // sort(begin(A), end(A));
    vector<int> test = mergesort(A);
    // assert(test == actual);
}

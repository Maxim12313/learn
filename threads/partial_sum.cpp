#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

int main() {
    vector<int> nums = {2, 2, 2, 2, 2, 2, 2};
    vector<int> presum(nums.size());
    partial_sum(begin(nums), end(nums), begin(presum));
    for (int val : nums)
        cout << val << ' ';
    cout << '\n';
    for (int val : presum)
        cout << val << ' ';
    cout << '\n';
}

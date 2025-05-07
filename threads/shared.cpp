#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

using namespace std;

vector<int> nums;
shared_mutex mtx;
int n;

void write_val(int val) {
    lock_guard<shared_mutex> lock(mtx);
    nums.push_back(val);
}

bool try_read_val(int idx, int &val) {
    shared_lock<shared_mutex> lock(mtx);
    if (idx >= nums.size())
        return false;
    val = nums[idx];
    return true;
}

void writer() {
    for (int i = 0; i < n; i++) {
        write_val(i);
    }
}
void reader() {
    for (int i = 0; i < n; i++) {
    }
}

int main() {
}

#include <cstdint>
#include <future>
#include <iostream>

using namespace std;

struct Hi {
    static int64_t longCompute() {
        for (int64_t i = 0; i < INT_MAX; i++) {
        }
        return 30;
    }
};

int main() {
    future<int64_t> res = async(launch::deferred, Hi::longCompute);
    cout << "stuff between\n";
    cout << res.get() << "\n";
    cout << "stuff between\n";
}

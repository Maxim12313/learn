#include <chrono>
#include <climits>
#include <cstdlib>
#include <future>
#include <iostream>

using namespace std;

int doLong() {
    for (int i = 0; i < INT_MAX; i++) {
    }
    return 0;
}

int main() {
    auto f = async(doLong);
    int i = 0;
    while (true) {
        // polling
        if (f.wait_for(chrono::seconds(0)) == future_status::ready) {
            break;
        }
        i++;
    }
    cout << i << "\n";
}

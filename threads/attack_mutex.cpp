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

class Protected {

public:
    list<int> data;
    mutex mut;
    template <typename Function>
    void evil(Function func) {
        func(data);
    }
};

list<int> *gotcha;

void attack(list<int> &data) {
    gotcha = &data;
}

int main() {
    Protected prot;
    prot.evil(attack);
    bool res = &prot.data == gotcha;
    cout << res << "\n";
}

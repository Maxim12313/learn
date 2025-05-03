#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <exception>
#include <functional>
#include <iostream>
#include <iterator>
#include <list>
#include <memory>
#include <mutex>
#include <numeric>
#include <stack>
#include <thread>
#include <vector>

using namespace std;

static shared_ptr<int> shared;

shared_ptr<int> &getShared() {
    return shared;
}

int main() {
}

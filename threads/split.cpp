#include <future>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

int step1(int a, int b) {
    return a * b;
}

int step2(int a, int b) {
    return a + b;
}
int step3(int a, int b) {
    return a / b;
}

int main() {
    int a = 20;
    int b = 4;
    auto f1 = async(launch::async, step1, a, b);
    auto f2 = async(launch::async, step2, a, b);
    auto f3 = async(launch::async, step3, a, b);
    int res = f1.get() + f2.get() + f3.get();
    cout << res << "\n";
}

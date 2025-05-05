#include <functional>
#include <iostream>

int mult(int a, int b) {
    return a * b;
}

int main() {
    auto f1 = std::bind(mult, std::placeholders::_1, 5);
    auto f2 = [](int a) { return mult(a, 5); };

    int val1 = f1(2);
    int val2 = f2(2);
    std::cout << val1 << ' ' << val2 << '\n';
}

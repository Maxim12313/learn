#include <condition_variable>
#include <future>
#include <queue>
#include <thread>

using namespace std;

queue<packaged_task<void()>> tasks;
mutex mut;
condition_variable ready;

void execTask(int &i, int n) {
}

int main() {
}

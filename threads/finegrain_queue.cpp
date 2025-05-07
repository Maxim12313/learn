#include <atomic>
#include <future>
#include <memory>
#include <mutex>

using namespace std;

class GoodQueue {
private:
    struct Node {
        int data;
        unique_ptr<Node> next;
    };

    mutex head_mtx;
    unique_ptr<Node> head;

    mutex tail_mtx;
    Node *tail;
    condition_variable condition;

    Node *get_tail() {
        lock_guard<mutex> guard(tail_mtx);
        return tail;
    }

public:
    GoodQueue() : head(make_unique<Node>()), tail(head.get()) {}
    GoodQueue(const GoodQueue &other) = delete;
    GoodQueue &operator=(const GoodQueue &other) = delete;

    // push on tail
    void push(int val) {
        lock_guard<mutex> tail_guard(tail_mtx);
        tail->data = val;
        tail->next = make_unique<Node>();
        tail = tail->next.get();
        condition.notify_one();
    }

    void wait_pop(int &val) {
        unique_lock<mutex> head_lck(head_mtx);
        condition.wait(head_lck, [this]() { return head.get() != get_tail(); });
        val = head.get()->data;
        head = std::move(head.get()->next);
    }

    // pop on head
    bool try_pop(int &val) {
        lock_guard<mutex> head_guard(head_mtx);
        if (get_tail() == head.get())
            return false;

        val = head.get()->data;
        head = std::move(head.get()->next);
        return true;
    }
};

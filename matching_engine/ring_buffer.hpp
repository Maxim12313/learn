#pragma once

#include <atomic>
#include <optional>

template <typename T, size_t N>
class RingBuffer {
    static_assert((N & (N - 1)) == 0, "Size must be a power of 2");
    T buffer[N];
    std::atomic<size_t> head{0};
    std::atomic<size_t> tail{0};

public:
    bool push(const T &item) {
        size_t h = head.load(std::memory_order_relaxed);
        size_t t = tail.load(std::memory_order_acquire);
        if (((h + 1) & (N - 1)) == (t & (N - 1)))
            return false;
        buffer[h & (N - 1)] = item;
        head.store(h + 1, std::memory_order_release);
        return true;
    }

    std::optional<T> pop() {
        size_t t = tail.load(std::memory_order_relaxed);
        size_t h = head.load(std::memory_order_acquire);
        if ((t & (N - 1)) == (h & (N - 1)))
            return std::nullopt;
        T item = buffer[t & (N - 1)];
        tail.store(t + 1, std::memory_order_release);
        return item;
    }
};

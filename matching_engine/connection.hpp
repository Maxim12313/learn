#pragma once
#include "config.hpp"
#include "match.hpp"
#include "order.hpp"
#include "ring_buffer.hpp"

struct Connection {
    RingBuffer<Order, MAX_EVENTS> &to_engine;
    RingBuffer<Match, MAX_EVENTS> &to_port;
    int port_wakeup_fd;
};

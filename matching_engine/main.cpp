#include "matching_engine.hpp"
#include "port.hpp"
#include <atomic>
#include <sys/eventfd.h>
#include <thread>

atomic<bool> running;

void port_run(Connection conn) {
    Port port(conn);
    port.init();
    port.run();
}

void matching_run(Connection &conn) {
    MatchingEngine engine(conn);
    engine.run();
}

int main() {
    running.store(true);
    RingBuffer<Order, MAX_EVENTS> to_engine;
    RingBuffer<Match, MAX_EVENTS> to_port;

    int port_wakeup_fd = eventfd(0, EFD_NONBLOCK);
    Connection conn{to_engine, to_port, port_wakeup_fd};

    thread port_thread(port_run, ref(conn));
    thread matching_thread(matching_run, ref(conn));

    port_thread.join();
    matching_thread.join();
}

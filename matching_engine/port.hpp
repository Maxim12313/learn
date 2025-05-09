#pragma once

#include "connection.hpp"
#include "order.hpp"
#include "ring_buffer.hpp"
#include <atomic>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <sys/epoll.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

#include "config.hpp"
#include "mapped_vector.hpp"

extern atomic<bool> running;

int setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

Order parse(const std::string &line, int user_id) {
    Order o;
    char side[8];
    sscanf(line.c_str(), "%s %d %f", side, &o.qty, &o.price);
    o.user_id = user_id;
    o.side = (strcmp(side, "BUY") == 0) ? Side::BUY : Side::SELL;
    return o;
}

class Port {
    int listen_fd, epoll_fd;
    unordered_map<int, std::string> buffer;
    MappedVector<int> clients;
    Connection conn;

    void handleWakeSignal() {
        uint64_t dummy;
        read(conn.port_wakeup_fd, &dummy, sizeof(dummy)); // clear wake
        vector<int> &fds = clients.get_data();
        cout << "sending\n";
        while (true) {
            auto res = conn.to_port.pop();
            if (!res.has_value())
                break;
            Match &m = *res;
            // can move this to class field
            char msg[1024];
            int length =
                snprintf(msg, sizeof(msg), "%d bought %d for %f from %d",
                         m.buyer_id, m.qty, m.price, m.seller_id);
            for (int fd : fds) {
                send(fd, msg, length, 0);
            }
        }
        cout << "finished\n";
    }

    void acceptClient() {
        sockaddr_in client_addr{};
        socklen_t len = sizeof(client_addr);
        int client_fd = accept(listen_fd, (sockaddr *)&client_addr, &len);
        setNonBlocking(client_fd);
        epoll_event ev{.events = EPOLLIN, .data = {.fd = client_fd}};
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev);
        buffer[client_fd] = "";
        clients.push(client_fd, client_fd);
        std::cout << client_fd << " connected\n";
    }

    void handleClient(int fd) {
        char buf[1024];
        ssize_t n = read(fd, buf, sizeof(buf));
        if (n <= 0) {
            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr);
            close(fd);
            buffer.erase(fd);
            std::cout << fd << " disconnected\n";
            clients.erase(fd);
            return;
        }

        buffer[fd].append(buf, n);
        while (true) {
            auto &data = buffer[fd];
            size_t pos = data.find('\n');
            if (pos == std::string::npos)
                break;
            std::string line = data.substr(0, pos);
            data.erase(0, pos + 1);
            Order order = parse(line, fd);
            conn.to_engine.push(order);
        }
    }

public:
    Port(Connection conn_in) : conn(conn_in) {}

    void init() {
        listen_fd = socket(AF_INET, SOCK_STREAM, 0);
        setNonBlocking(listen_fd);

        sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(9000);
        bind(listen_fd, (sockaddr *)&addr, sizeof(addr));
        listen(listen_fd, SOMAXCONN);

        epoll_fd = epoll_create1(0);

        // add listen_fd
        epoll_event ev{.events = EPOLLIN, .data = {.fd = listen_fd}};
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev);

        // add wakeup_fd
        epoll_event wake_ev{.events = EPOLLIN,
                            .data = {.fd = conn.port_wakeup_fd}};
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn.port_wakeup_fd, &wake_ev);
    }

    void run() {
        epoll_event events[64];
        while (running.load(memory_order_acquire)) {
            int n = epoll_wait(epoll_fd, events, 64, -1);
            for (int i = 0; i < n; ++i) {
                int fd = events[i].data.fd;
                if (fd == listen_fd) {
                    acceptClient();
                } else if (fd == conn.port_wakeup_fd) {
                    handleWakeSignal();
                } else {
                    handleClient(fd);
                }
            }
        }
    }
};

#pragma once
#include "config.hpp"
#include "connection.hpp"
#include "order.hpp"
#include <iostream>
#include <list>
#include <map>
#include <unordered_map>
#include <vector>

extern atomic<bool> running;

struct Match {
    Order a, b;
    int qty;
    float price;
};

float get_match_price(float buy_price, float sell_price, bool buy_is_recent) {
    return buy_is_recent ? sell_price : buy_price;
}

class MatchingEngine {
private:
    // store interesed one at begin
    map<float, list<Order>, greater<float>> buy_orders; // descending
    map<float, list<Order>> sell_orders;                // ascending
    unordered_map<int, list<Order>::iterator> id_to_iterator;
    Connection conn;

    vector<Match> make_matches(bool is_buy) {
        vector<Match> matches;
        while (!buy_orders.empty() && !sell_orders.empty()) {
            auto max_buy = begin(buy_orders);
            auto min_sell = begin(sell_orders);
            if (max_buy->first < min_sell->first)
                break;

            float price =
                get_match_price(max_buy->first, min_sell->first, is_buy);

            list<Order> &buys = max_buy->second;
            list<Order> &sells = min_sell->second;
            while (!buys.empty() && !sells.empty()) {
                Order &buy_order = buys.front();
                Order &sell_order = sells.front();

                int change = min(buy_order.qty, sell_order.qty);
                buy_order.qty -= change;
                sell_order.qty -= change;

                // order after changes, can easily recover knowing change
                matches.emplace_back(buy_order, sell_order, change, price);

                if (!buy_order.qty)
                    buys.pop_front();

                if (!sell_order.qty)
                    sells.pop_front();
            }
            if (buys.empty())
                buy_orders.erase(max_buy);
            if (sells.empty())
                sell_orders.erase(min_sell);
        }
        return matches;
    }

    void handle_order(Order order) {
        bool is_buy = order.side == Side::BUY;
        if (is_buy) {
            buy_orders[order.price].push_back(std::move(order));
        } else {
            sell_orders[order.price].push_back(std::move(order));
        }
        vector<Match> matches = make_matches(is_buy);
        if (matches.size())
            broadcast_matches(matches);
    }

    void broadcast_matches(vector<Match> &matches) {
        uint64_t garb = 1;
        write(conn.port_wakeup_fd, &garb, sizeof(garb));
        cout << "matches: " << matches.size() << "\n";
        for (auto [a, b, qty, price] : matches) {
            cout << a.price << " " << b.price << " " << qty << " " << price
                 << "\n";
        }
    }
    void cancel_order(list<Order>::iterator it) {}

public:
    MatchingEngine(Connection conn_in) : conn(conn_in) {}

    void run() {
        while (running.load(memory_order_acquire)) {
            auto res = conn.to_engine.pop();
            if (res.has_value()) {
                Order order = *res;
                handle_order(order);
            }
        }
    }
};

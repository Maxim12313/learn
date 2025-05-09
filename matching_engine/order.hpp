#pragma once
#include <string>

enum class Side { BUY, SELL };

struct Order {
    int user_id;
    int internal_order_id;
    int qty;
    float price;
    Side side;
};

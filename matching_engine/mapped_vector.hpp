#pragma once

#include "config.hpp"
#include <unordered_map>
#include <vector>

template <typename T>
class MappedVector {
private:
    vector<T> data;
    unordered_map<int, int> to_idx;   // from id
    unordered_map<int, int> from_idx; // to id

public:
    MappedVector() {}

    vector<T> &get_data() { return data; }

    void push(int id, T val) {
        to_idx[id] = data.size();
        from_idx[data.size()] = id;
        data.push_back(val);
    }

    void get(int id) { return data[to_idx[id]]; }

    void erase(int id) {
        int i = to_idx[id];
        int j = data.size() - 1;

        swap(data[i], data[j]);
        int id2 = from_idx[j];
        to_idx[id2] = i;

        data.pop_back();
        to_idx.erase(to_idx.find(id));
        from_idx.erase(from_idx.find(j));
    }
};

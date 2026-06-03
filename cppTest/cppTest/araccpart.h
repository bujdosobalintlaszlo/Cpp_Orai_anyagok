#pragma once
#include <vector>
#include <numeric>
#include <functional>
#include <cstddef>

template<typename T, typename Op = std::plus<T>>
class array_accumulate_partition {
public:
    array_accumulate_partition(T* a, int size)
        : a(a), size(size), partition_idx(0), original(a, a + size), op(Op()) {
    }

    template <std::size_t N>
    array_accumulate_partition(T(&arr)[N])
        : a(arr), size(N), partition_idx(0), original(arr, arr + N), op(Op()) {
    }

    void set_partition_idx(int idx) {
        if (idx < 0 || idx > size) return;        
        partition_idx = idx;


        for (int i = 0; i < size; ++i) {
            a[i] = original[i];
        }

        if (idx > 0) {
            T current_sum = original[0];
            for (int i = 1; i < idx; ++i) {
                current_sum = op(current_sum, original[i]);
                a[i] = current_sum;
            }
        }
    }

    T* begin() { return a + partition_idx; }
    T* end() { return a + size; }

    T* abegin() { return a; }
    T* aend() { return a + partition_idx; }

    array_accumulate_partition& operator>>(int shift) {
        set_partition_idx(partition_idx + shift);
        return *this;
    }

    array_accumulate_partition& operator<<(int shift) {
        set_partition_idx(partition_idx - shift);
        return *this;
    }

    friend array_accumulate_partition& operator<<(int shift, array_accumulate_partition& obj) {
        obj.set_partition_idx(obj.partition_idx - shift);
        return obj;
    }

    friend array_accumulate_partition& operator>>(int shift, array_accumulate_partition& obj) {
        obj.set_partition_idx(obj.partition_idx + shift);
        return obj;
    }



private:
    T* a;
    int size;
    int partition_idx;
    std::vector<T> original;
    Op op;
};
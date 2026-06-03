#pragma once
#include <vector>

template<typename T, typename Op = std::plus<T>>
class array_accumulate_view {
private:
    struct ArraySegment {
        const T* ptr;
        int length;
    };

    std::vector<ArraySegment> segments;
    int total_size = 0;  
    Op operation{};

    T sumUpTillIdx(int idx) const {
        T result{};
        bool first = true;
        int current_idx = 0;


        for (const auto& segment : segments) {
            for (int i = 0; i < segment.length; ++i) {
                if (current_idx > idx) {
                    return result;
                }

                if (first) {
                    result = segment.ptr[i]; // Csak a legelső szegmens legelső elemét másoljuk le
                    first = false;           // Utána kikapcsoljuk
                }
                else {
                    result = operation(result, segment.ptr[i]); // Minden más elemet kombinálunk az operation-nel
                }
                current_idx++;
            }
        }
        return result;
    }

public:
    array_accumulate_view() = default;
    array_accumulate_view(const T* storage, int size) {
        add(storage, size);
    }

    template<std::size_t N>
    array_accumulate_view(const T(&storage)[N]) {
        add(storage, static_cast<int>(N));
    }

    
    void add(const T* storage, int size) {
        segments.push_back({ storage, size });
        total_size += size;
    }

    template<std::size_t N>
    void add(const T(&storage)[N]) {
        add(storage, static_cast<int>(N));
    }

    int size() const {
        return total_size;
    }

    T at(int idx) const {
        return sumUpTillIdx(idx);
    }

    T operator[](int idx) const {
        return sumUpTillIdx(idx);
    }

    template<std::size_t N>
    array_accumulate_view& operator+=(const T(&storage)[N]) {
        add(storage); 
        return *this; // A += operátorok konvenció szerint mindig önmagukra mutató referenciát adnak vissza
    }
};
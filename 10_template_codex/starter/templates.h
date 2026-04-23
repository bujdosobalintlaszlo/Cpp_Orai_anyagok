#pragma once
#include <vector>
#include <algorithm>

// ============================================================================
// REQUIRED TYPES AND CLASSES FOR TESTS
// ============================================================================

/**
 * Container<T> class template - A generic bounded collection
 *
 * Requirements for tests:
 * - Constructor: Container(capacity)
 * - add(const T&) - returns false if full
 * - remove(const T&) - returns false if not found
 * - find(const T&) const - returns const T* (nullptr if not found)
 * - size() const, capacity() const, isFull() const, clear()
 *
 * Hints:
 * - How do you search for an element in a vector?
 * - What does it mean to parameterize a class on a type?
 *
 * Resources:
 * - https://en.cppreference.com/w/cpp/language/class_template
 */
template <typename T>
T clamp(T value, T min, T max);

template <typename T>
class Container {
    // TODO: Implement Container
private:
     std::vector<T> items;
     int capacity_;
public:
     Container(int capacity) : capacity_(capacity) {}

     bool add(const T& item){
          if(items.size() >= (size_t)capacity_) return false;
          items.push_back(item);
          return true;
     }

     bool remove(const T& item){
          auto it = std::find(items.begin(), items.end(), item);
          if (it != items.end()) {
                items.erase(it);
                return true;
          }
          return false;
     }
     
     const T* find(const T& item) const {
          auto it = std::find(items.begin(), items.end(), item);
          if(it == items.end()) return nullptr;
          return &(*it);
     }

     int size() const {return items.size();}
     int capacity() const {return capacity_;}
     bool isFull() const {return items.size() >= (size_t)capacity_;}
     void clear() {items.clear();}
};

/**
 * Stat<T> class template - A numeric stat with min/max bounds
 *
 * Requirements for tests:
 * - Constructor: Stat(base, min, max) - current value starts at base
 * - modify(amount) - adds amount, keeps value within [min, max]
 * - set(value) - sets value, keeps within [min, max]
 * - get() const - returns current value
 * - getBase() const - returns base value
 * - reset() - restores current value to base
 *
 * Hints:
 * - Read the tests to see what types T can be (int, float, double)
 */
template <typename T>
class Stat {
    // TODO: Implement Stat
private:
     T base;
     T min;
     T max;
     T curr;
public:
     Stat(T base, T min, T max) : base(base), max(max), min(min), curr(base) {}
     
     void modify(T amount){ curr = clamp(curr + amount, min, max); }
     void set(T val){ curr = clamp(val, min, max); }

     T get() const { return curr; }
     T getBase() const { return base; }

     void reset(){
          curr = base;
     }
};

/**
 * clamp<T> function template
 * Returns value constrained to [min, max] range
 */
template <typename T>
T clamp(T value, T min, T max) {
    // TODO: Implement
    return std::max(min, std::min(value, max));
}

/**
 * swap<T> function template
 * Exchanges the values of a and b
 */
template <typename T>
void swap(T& a, T& b) {
    // TODO: Implement
    T temp = std::move(a);
     a = std::move(b);
     b = std::move(temp);
}

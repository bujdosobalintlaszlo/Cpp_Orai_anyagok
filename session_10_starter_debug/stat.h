#pragma once

#include <algorithm>

#include "display.h"

// std::clamp

template <typename T>
T clamp(T value, T min, T max) {
  displayTypeInfo<T>("T");
  value = std::min(value, max);
  value = std::max(value, min);
  return value;
}

template <typename T>
class Stat {
  T current_;
  T max_;
public:
  explicit Stat(T value, T max) : current_(value), max_(max) {}

  void modify(T amount) {
    current_ = ::clamp(current_ + amount, T{0}, max_);
  }

  T getValue() const { return current_; }
  T getMaxValue() const { return max_; }
};


template <>
class Stat<std::string> {
  std::string current_;
  std::string max_;
public:
  explicit Stat(const std::string& value, const std::string& max) : current_(value), max_(max) {}

  void modify(const std::string& amount) {
    current_ = ::clamp(current_ + amount, std::string{""}, max_);
  }

  std::string getValue() const { return current_; }
  std::string getMaxValue() const { return max_; }
};

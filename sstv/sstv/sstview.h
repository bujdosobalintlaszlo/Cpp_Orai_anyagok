#pragma once
#include <set>
#include <vector>
#include <stack>
#include <list>

template<typename T, typename Compare = std::less<T>>
class set_stack_view {
    std::set<T, Compare>* set_;
    std::vector<T>        stack_;
    std::vector<T>        originals_;

public:
    set_stack_view(std::set<T, Compare>& s) : set_(&s) {
        for (const auto& e : s) {
            originals_.push_back(e);
            stack_.push_back(e);
        }
        s.clear();
    }

    ~set_stack_view() {
        set_->clear();
        for (const auto& e : originals_) set_->insert(e);
    }

    void push(const T& item) {
        if (set_->count(item)) return;
        set_->insert(item);
        stack_.push_back(item);
    }

    // 3-as: iterator range push (tetszőleges más set-ből)
    template<typename It>
    void push(It begin, It end) {
        for (auto it = begin; it != end; ++it)
            push(*it);
    }

    // 3-as: += másik set-ből
    template<typename OtherCompare>
    set_stack_view& operator+=(const std::set<T, OtherCompare>& other) {
        for (const auto& e : other)
            push(e);
        return *this;
    }

    // 3-as
    void pop() {
        if (stack_.empty()) return;
        set_->erase(stack_.back());
        stack_.pop_back();
    }

    const T& top() const { return stack_.back(); }
    T& top() { return stack_.back(); }

    // 5-ös: konverzió tetszőleges stack<T, Container> típusra
    template<typename Container>
    operator std::stack<T, Container>() const {
        Container c(stack_.begin(), stack_.end());
        return std::stack<T, Container>(c);
    }
};
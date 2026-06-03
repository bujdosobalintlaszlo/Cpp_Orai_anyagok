#ifndef PRIO_STACK_H
#define PRIO_STACK_H

#include <functional>
#include <map>
#include <stack>

template <typename T, typename Prio = double, typename Cmp = std::less<Prio>>
class priority_stack {
    typedef std::map<Prio, std::stack<T>, Cmp> map_t;
    typedef typename map_t::const_iterator map_cit_t;
    map_t d;

   public:
    void push(const Prio& p, const T& e) { d[p].push(e); }

    int size(const Prio& p) const {
        map_cit_t pi = d.find(p);
        return pi == d.end() ? 0 : pi->second.size();
    }

    int size() const {
        int s = 0;
        for (map_cit_t it = d.begin(); it != d.end(); ++it)
            s += it->second.size();
        return s;
    }

    T& top() { return d.rbegin()->second.top(); }

    void pop() {
        const Prio& p = d.rbegin()->first;
        d.rbegin()->second.pop();
        if (d.rbegin()->second.empty()) d.erase(p);
    }
};

#endif

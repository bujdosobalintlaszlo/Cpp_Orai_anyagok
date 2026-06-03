#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <functional>
#include <vector>

template <typename T, typename Cmp = std::less<T>>
class SortedList {
    typedef std::vector<T> DataT;
    DataT backing_list;

   public:
    using IteratorT = typename DataT::iterator;
    using ConstIteratorT = typename DataT::const_iterator;

    SortedList() {};

    template <typename It>
    SortedList(It begin, It end) : backing_list(begin, end) {
        std::sort(backing_list.begin(), backing_list.end(), Cmp());
    }

    void insert(const T& e) {
        IteratorT pos = std::lower_bound(backing_list.begin(), backing_list.end(), e, Cmp());
        backing_list.insert(pos, e);
    }

    void remove(const T& e) {
        std::pair<IteratorT, IteratorT> er =
            std::equal_range(backing_list.begin(), backing_list.end(), e, Cmp());
        backing_list.erase(er.first, er.second);
    }

    int size() const { return backing_list.size(); }

    const T& front() const { return backing_list.front(); }
    const T& back() const { return backing_list.back(); }

    ConstIteratorT begin() const { return backing_list.begin(); }
    ConstIteratorT end() const { return backing_list.end(); }
};

#endif

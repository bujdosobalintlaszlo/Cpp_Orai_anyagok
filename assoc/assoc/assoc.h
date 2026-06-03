#pragma once
#include <vector>
#include <utility>
#include <functional>

// Közös alaposztály az 5-ös osztályzatért
template <typename Key, typename Container>
class association_common {
public:
    virtual ~association_common() {}
};

template <
    typename Key,
    typename Container,
    typename Value = typename Container::value_type,
    typename Compare = std::equal_to<Key>,
    typename Iterator = typename Container::iterator // Automatikusan jó lesz mindennek!
>
class association : public association_common<Key, Container>
{
private:
    Container& container;
    std::vector<std::pair<Key, Iterator>> registry;
    Compare comp;

public:
    association(Container& c) : container(c) {}

    void associate(const Key& key, Iterator it) {
        registry.push_back(std::pair<Key, Iterator>(key, it));
    }

    Value find(const Key& key) const {
        for (size_t i = 0; i < registry.size(); ++i) {
            if (comp(registry[i].first, key)) {
                return *(registry[i].second);
            }
        }
        return Value();
    }

    bool has(const Key& key) const {
        for (size_t i = 0; i < registry.size(); ++i) {
            if (comp(registry[i].first, key)) return true;
        }
        return false;
    }

    Value& operator[](const Key& key) {
        for (size_t i = 0; i < registry.size(); ++i) {
            if (comp(registry[i].first, key)) {
                return *(registry[i].second);
            }
        }

        Iterator newIt = container.insert(container.end(), typename Container::value_type());
        registry.push_back(std::pair<Key, Iterator>(key, newIt));

        return *newIt;
    }
};
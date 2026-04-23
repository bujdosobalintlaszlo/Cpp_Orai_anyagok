#pragma once
#include "types.h"

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

struct Item {
    std::string name;
    float32 value;

    Item(const std::string& name, float32 value) : name(name), value(value) {}
};

inline std::ostream& operator<<(std::ostream& stream, const Item& item) {
    stream << "#Item " << item.name << ' ' << item.value << '\n';
    return stream;
}

inline std::istream& operator>>(std::istream& stream, Item& item) {
    std::string marker;
    stream >> marker >> item.name >> item.value;
    return stream;
}

class Inventory {
    std::vector<Item> items;

   public:
    void addItem(const std::string& name, int value) { items.push_back(Item(name, value)); }

    void removeItem(const std::string& name) {
        for (int i = 0; i < items.size(); i++) {
            if (name == items[i].name) {
                // Shift remaining items
                std::swap(items[i], items.back());
                items.pop_back();
            }
        }
    }

    size_t size() const { return items.size(); }

    const Item* findItem(const std::string& name) const {
        for (int i = 0; i < items.size(); i++) {
            if (name.compare(items[i].name.data()) == 0) {
                return &items[i];
            }
        }
        return nullptr;
    }

    void display() const {
        std::cout << "\nCurrent inventory (" << items.size() << "/" << items.capacity()
                  << " items):\n";
        if (items.empty()) {
            std::cout << "  (empty)\n";
            return;
        }

        for (int i = 0; i < items.size(); i++) {
            std::cout << "  " << (i + 1) << ". " << std::left << std::setw(20)
                      << items[i].name.data() << " - " << items[i].value << " gold\n";
        }
    }

    friend std::ostream& operator<<(std::ostream& stream, const Inventory&);
    friend std::istream& operator>>(std::istream& stream, Inventory& inv);
};

inline std::ostream& operator<<(std::ostream& stream, const Inventory& inv) {
    stream << "#Inventory\n";
    for (int i = 0, size = inv.items.size(); i < size; ++i) {
        stream << inv.items[i];
    }
    return stream;
}

inline std::istream& operator>>(std::istream& stream, Inventory& inv) {
    std::string marker;
    stream >> marker;
    Item item("", 0);
    while (stream >> item) {
        inv.items.push_back(item);
    }
    return stream;
}


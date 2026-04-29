#include "../priority_stack.h"
#include "../sorted_list.h"

#include <catch2/catch_all.hpp>

#include <deque>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// SortedList — exercises equivalent to 19_SortedList/main.cpp
// ---------------------------------------------------------------------------

struct IntGreater {
    bool operator()(int a, int b) const { return a > b; }
};

struct StringSizeLess {
    bool operator()(const std::string& a, const std::string& b) const {
        return a.size() < b.size();
    }
};

template <typename T>
struct VectorSizeLess {
    bool operator()(const std::vector<T>& a, const std::vector<T>& b) const {
        return a.size() < b.size();
    }
};

TEST_CASE("SortedList insert maintains ascending order", "[ds][sorted]") {
    SortedList<int> a;
    a.insert(3);
    a.insert(4);
    a.insert(2);
    REQUIRE(a.size() == 3);
    REQUIRE(a.front() == 2);
    REQUIRE(a.back() == 4);
}

TEST_CASE("SortedList copy and remove", "[ds][sorted]") {
    SortedList<int> a;
    a.insert(3);
    a.insert(4);
    a.insert(2);
    a.insert(5);
    a.remove(2);
    const SortedList<int> ca = a;
    REQUIRE(ca.size() == 3);
    REQUIRE(ca.front() == 3);
    REQUIRE(ca.back() == 5);
}

TEST_CASE("SortedList iterate sum", "[ds][sorted]") {
    SortedList<int> a;
    a.insert(3);
    a.insert(4);
    a.insert(2);
    const SortedList<int> ca = a;
    int sum = 0;
    for (auto it = ca.begin(); it != ca.end(); ++it)
        sum += *it;
    REQUIRE(sum == 9);
}

TEST_CASE("SortedList custom comparator desc via greater sort order", "[ds][sorted]") {
    std::vector<int> l{13, 10, 43, 15};
    SortedList<int, IntGreater> vsl(l.begin(), l.end());
    REQUIRE(vsl.front() == 43);
    REQUIRE(vsl.back() == 10);

    std::deque<int> d{2, 5};
    SortedList<int, IntGreater> dsl(d.begin(), d.end());
    REQUIRE(dsl.front() == 5);
    REQUIRE(dsl.back() == 2);
}

TEST_CASE("SortedList strings ordered by length", "[ds][sorted]") {
    SortedList<std::string, StringSizeLess> sl;
    sl.insert("hello");
    sl.insert("hi");
    sl.insert("bye");
    REQUIRE(sl.front() == "hi");
    REQUIRE(sl.back() == "hello");
}

// ---------------------------------------------------------------------------
// priority_stack — exercises equivalent to 20_PriorityStack/main.cpp
// ---------------------------------------------------------------------------

TEST_CASE("priority_stack mass push same payload distinct priorities", "[ds][prio]") {
    priority_stack<double> sd;
    for (int i = 0; i < 1000; ++i)
        sd.push(static_cast<double>(i), 6.7);
    REQUIRE(sd.size(0) == 1);
}

TEST_CASE("priority_stack tie-breaking stack order", "[ds][prio]") {
    priority_stack<int> si;
    si.push(5, 2);
    si.push(5, 3);
    si.push(4, 7);
    REQUIRE(si.top() == 3);
    REQUIRE(si.size(5) == 2);
}

TEST_CASE("priority_stack mutate top", "[ds][prio]") {
    priority_stack<int> si;
    si.push(5, 2);
    si.push(5, 3);
    si.push(4, 7);
    const priority_stack<int> csi = si;
    si.top() = 5;
    REQUIRE(si.top() == 5);
    REQUIRE(csi.size() == 3);
    si.pop();
    si.pop();
    priority_stack<int, double> sid;
    sid.push(6.4, 4);
    REQUIRE(si.size() == sid.size());
    REQUIRE(sid.top() == 4);
}

TEST_CASE("priority_stack string priority ordering", "[ds][prio]") {
    priority_stack<int, std::string, StringSizeLess> strst;
    strst.push("abcdg", 5);
    strst.push("dfg", 4);
    REQUIRE(strst.size() == 2);
    REQUIRE(strst.top() == 5);
}

// ---------------------------------------------------------------------------
// Integration: both types in one “session” story
// ---------------------------------------------------------------------------

TEST_CASE("Quest titles sorted, loot lines by priority", "[ds][integration]") {
    SortedList<std::string> titles;
    titles.insert("Slay the Dragon");
    titles.insert("Defeat the Goblin");
    titles.insert("Find the Amulet");
    auto it = titles.begin();
    REQUIRE(*it == "Defeat the Goblin");
    ++it;
    REQUIRE(*it == "Find the Amulet");
    ++it;
    REQUIRE(*it == "Slay the Dragon");

    priority_stack<std::string, int> lines;
    lines.push(1, "  - Gold Coins (10 gold)");
    lines.push(3, "  - Magic Amulet (200 gold)");
    lines.push(2, "  - Steel Sword (100 gold)");
    REQUIRE(lines.top() == "  - Magic Amulet (200 gold)");
    lines.pop();
    REQUIRE(lines.top() == "  - Steel Sword (100 gold)");
    lines.pop();
    REQUIRE(lines.top() == "  - Gold Coins (10 gold)");
}

#include "templates.h"

#include <iostream>
#include <string>
#include "templates.h"
int main() {
    std::cout << "================================\n";
    std::cout << "  TEMPLATE CODEX\n";
    std::cout << "================================\n";

    // TODO: Test Container<int>
    // - Create with capacity 10
    // - Add items: 10, 20, 30, 42, 50
    // - Find item 42
    // - Display size
	 Container<int> c(10);
	 c.add(10);
	 c.add(20);
	 c.add(30);
	 c.add(42);
	 c.add(50);
	 std::cout << "found:" << *(c.find(42)) << '\n';
	 std::cout << "size: " << c.size() << '\n';
    // TODO: Test Container<string>
    // - Create with capacity 5
    // - Add items: "hello", "world", "template"
    // - Find item "world"
	 Container<std::string> c2(5);
	 c2.add("hello");
	 c2.add("world");
	 c2.add("template");
	 std::cout << "found:" << *(c2.find("world")) << '\n';
    // TODO: Test Stat<int>
    // - Create with base=100, min=0, max=200
    // - Modify by +20
    // - Modify by -50
    // - Reset to base
	 Stat<int> s(100,0,200);
	 s.modify(20);
	 s.modify(-50);
	 s.reset();
    // TODO: Test Stat<double>
    // - Create with base=1.0, min=0.0, max=2.0
    // - Modify by +0.5
	 Stat<int> s2(1.0,0.0,2.0);
	 s2.modify(0.5);
	 s2.reset();

    // TODO: Test generic functions
    // - clamp(15, 0, 10)
    // - swap two integers
	 clamp(15,0,10);
	 int a = 43;
	 int b =90;
	 swap(a,b);

    return 0;
}

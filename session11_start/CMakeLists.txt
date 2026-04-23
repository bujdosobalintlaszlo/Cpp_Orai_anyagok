cmake_minimum_required(VERSION 3.14)
project(session11_stl_quest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

# Game executable
add_executable(dungeon_crawler main.cpp)

# Tests
enable_testing()
find_package(Catch2 3 QUIET)

if(Catch2_FOUND)
    add_executable(test_session11 tests/test_session11.cpp)
    target_link_libraries(test_session11 PRIVATE Catch2::Catch2WithMain)
    add_test(NAME Session11_Tests COMMAND test_session11)
else()
    message(STATUS "Catch2 not found - run: sudo apt install catch2 (or brew install catch2)")
endif()

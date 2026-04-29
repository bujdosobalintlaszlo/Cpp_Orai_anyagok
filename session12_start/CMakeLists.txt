cmake_minimum_required(VERSION 3.14)
project(session12_ds_quest)

set(CMAKE_CXX_STANDARD 20)
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
    add_executable(test_session12_ds tests/test_session12_ds.cpp)
    target_link_libraries(test_session12_ds PRIVATE Catch2::Catch2WithMain)
    add_test(NAME Session12_DS_Tests COMMAND test_session12_ds)
else()
    message(STATUS "Catch2 not found - run: sudo apt install catch2 (or brew install catch2)")
endif()

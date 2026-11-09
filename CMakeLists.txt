cmake_minimum_required(VERSION 3.15)
project(dsa_core_cpp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Phase 1: Linear Container Tests
add_executable(test_vector tests/test_vector.cpp)
add_executable(test_linked_list tests/test_linked_list.cpp)
add_executable(test_stack tests/test_stack.cpp)
add_executable(test_queue tests/test_queue.cpp)

enable_testing()
add_test(NAME TestVector COMMAND test_vector)
add_test(NAME TestLinkedList COMMAND test_linked_list)
add_test(NAME TestStack COMMAND test_stack)
add_test(NAME TestQueue COMMAND test_queue)

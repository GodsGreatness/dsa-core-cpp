cmake_minimum_required(VERSION 3.15)
project(dsa_core_cpp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# CLI Demo Target
add_executable(dsa_demo src/main.cpp)

# Linear Container Tests
add_executable(test_vector tests/test_vector.cpp)
add_executable(test_linked_list tests/test_linked_list.cpp)
add_executable(test_stack tests/test_stack.cpp)
add_executable(test_queue tests/test_queue.cpp)

# Tree Tests
add_executable(test_bst tests/test_bst.cpp)
add_executable(test_avl tests/test_avl.cpp)

# Heap Tests
add_executable(test_heap tests/test_heap.cpp)

# Graph Tests
add_executable(test_graph tests/test_graph.cpp)

# Sorting Tests
add_executable(test_sorting tests/test_sorting.cpp)

# Benchmark Targets
add_executable(benchmark_linear benchmarks/benchmark_linear.cpp)
add_executable(benchmark_trees benchmarks/benchmark_trees.cpp)
add_executable(benchmark_sorting benchmarks/benchmark_sorting.cpp)

# Example Targets
add_executable(example_vector examples/vector_example.cpp)
add_executable(example_avl examples/avl_example.cpp)
add_executable(example_graph examples/graph_example.cpp)

enable_testing()
add_test(NAME TestVector COMMAND test_vector)
add_test(NAME TestLinkedList COMMAND test_linked_list)
add_test(NAME TestStack COMMAND test_stack)
add_test(NAME TestQueue COMMAND test_queue)
add_test(NAME TestBST COMMAND test_bst)
add_test(NAME TestAVL COMMAND test_avl)
add_test(NAME TestHeap COMMAND test_heap)
add_test(NAME TestGraph COMMAND test_graph)
add_test(NAME TestSorting COMMAND test_sorting)

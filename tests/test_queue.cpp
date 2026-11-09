#include "dsa/linear/Queue.hpp"
#include <cassert>
#include <iostream>
#include <memory>
#include <string>

void testFIFOOrdering() {
    std::cout << "[Test Queue] FIFO ordering & basic operations... ";
    dsa::Queue<int> queue;
    assert(queue.empty());
    assert(queue.size() == 0);

    queue.push(10);
    queue.push(20);
    queue.push(30);

    assert(!queue.empty());
    assert(queue.size() == 3);
    assert(queue.front() == 10);
    assert(queue.back() == 30);

    queue.pop();
    assert(queue.size() == 2);
    assert(queue.front() == 20);
    assert(queue.back() == 30);

    queue.pop();
    assert(queue.size() == 1);
    assert(queue.front() == 30);
    assert(queue.back() == 30);

    queue.pop();
    assert(queue.empty());
    assert(queue.size() == 0);

    std::cout << "PASSED\n";
}

void testInterleavedOperations() {
    std::cout << "[Test Queue] Interleaved push/pop cycle... ";
    dsa::Queue<int> queue;

    for (int i = 0; i < 100; ++i) {
        queue.push(i);
    }
    assert(queue.size() == 100);

    for (int i = 0; i < 50; ++i) {
        assert(queue.front() == i);
        queue.pop();
    }
    assert(queue.size() == 50);

    for (int i = 100; i < 150; ++i) {
        queue.push(i);
    }
    assert(queue.size() == 100);

    for (int i = 50; i < 150; ++i) {
        assert(queue.front() == i);
        queue.pop();
    }
    assert(queue.empty());

    std::cout << "PASSED\n";
}

void testEmplaceAndMoveOnly() {
    std::cout << "[Test Queue] Emplace & Move-only types (std::unique_ptr)... ";
    dsa::Queue<std::unique_ptr<int>> queue;

    queue.push(std::make_unique<int>(100));
    queue.emplace(new int(200));

    assert(queue.size() == 2);
    assert(*queue.front() == 100);
    assert(*queue.back() == 200);

    queue.pop();
    assert(queue.size() == 1);
    assert(*queue.front() == 200);

    std::cout << "PASSED\n";
}

void testUnderflowExceptions() {
    std::cout << "[Test Queue] Underflow exception handling... ";
    dsa::Queue<int> emptyQueue;

    bool caughtFront = false;
    try {
        emptyQueue.front();
    } catch (const std::underflow_error&) {
        caughtFront = true;
    }
    assert(caughtFront);

    bool caughtBack = false;
    try {
        emptyQueue.back();
    } catch (const std::underflow_error&) {
        caughtBack = true;
    }
    assert(caughtBack);

    bool caughtPop = false;
    try {
        emptyQueue.pop();
    } catch (const std::underflow_error&) {
        caughtPop = true;
    }
    assert(caughtPop);

    std::cout << "PASSED\n";
}

int main() {
    std::cout << "===========================================\n";
    std::cout << " Running Queue<T> Test Suite\n";
    std::cout << "===========================================\n";

    testFIFOOrdering();
    testInterleavedOperations();
    testEmplaceAndMoveOnly();
    testUnderflowExceptions();

    std::cout << ">>> ALL QUEUE TESTS PASSED! <<<\n\n";
    return 0;
}

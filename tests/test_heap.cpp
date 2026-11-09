#include "dsa/heap/PriorityQueue.hpp"
#include <cassert>
#include <functional>
#include <iostream>
#include <vector>

void testMaxHeap() {
    std::cout << "[Test PriorityQueue] Max-Heap default behavior... ";
    dsa::PriorityQueue<int> maxHeap;
    assert(maxHeap.empty());
    assert(maxHeap.size() == 0);

    maxHeap.push(30);
    maxHeap.push(10);
    maxHeap.push(50);
    maxHeap.push(20);
    maxHeap.push(40);

    assert(maxHeap.size() == 5);
    assert(maxHeap.top() == 50);

    std::vector<int> extracted;
    while (!maxHeap.empty()) {
        extracted.push_back(maxHeap.top());
        maxHeap.pop();
    }

    assert(extracted == (std::vector<int>{50, 40, 30, 20, 10}));
    assert(maxHeap.empty());

    std::cout << "PASSED\n";
}

void testMinHeap() {
    std::cout << "[Test PriorityQueue] Min-Heap behavior (std::greater<T>)... ";
    dsa::PriorityQueue<int, std::greater<int>> minHeap;

    minHeap.push(30);
    minHeap.push(10);
    minHeap.push(50);
    minHeap.push(20);
    minHeap.push(40);

    assert(minHeap.size() == 5);
    assert(minHeap.top() == 10);

    std::vector<int> extracted;
    while (!minHeap.empty()) {
        extracted.push_back(minHeap.top());
        minHeap.pop();
    }

    assert(extracted == (std::vector<int>{10, 20, 30, 40, 50}));
    assert(minHeap.empty());

    std::cout << "PASSED\n";
}

void testFloydBottomUpHeapify() {
    std::cout << "[Test PriorityQueue] Floyd's O(n) Heapify construction... ";
    dsa::PriorityQueue<int> heap{15, 10, 20, 8, 12, 18, 25};

    assert(heap.size() == 7);
    assert(heap.top() == 25);

    std::vector<int> extracted;
    while (!heap.empty()) {
        extracted.push_back(heap.top());
        heap.pop();
    }

    assert(extracted == (std::vector<int>{25, 20, 18, 15, 12, 10, 8}));

    std::cout << "PASSED\n";
}

void testUnderflowExceptions() {
    std::cout << "[Test PriorityQueue] Underflow exceptions... ";
    dsa::PriorityQueue<int> emptyHeap;

    bool caughtTop = false;
    try {
        emptyHeap.top();
    } catch (const std::underflow_error&) {
        caughtTop = true;
    }
    assert(caughtTop);

    bool caughtPop = false;
    try {
        emptyHeap.pop();
    } catch (const std::underflow_error&) {
        caughtPop = true;
    }
    assert(caughtPop);

    std::cout << "PASSED\n";
}

int main() {
    std::cout << "===========================================\n";
    std::cout << " Running PriorityQueue<T> Test Suite\n";
    std::cout << "===========================================\n";

    testMaxHeap();
    testMinHeap();
    testFloydBottomUpHeapify();
    testUnderflowExceptions();

    std::cout << ">>> ALL PRIORITY QUEUE TESTS PASSED! <<<\n\n";
    return 0;
}

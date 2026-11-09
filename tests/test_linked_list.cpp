#include "dsa/linear/LinkedList.hpp"
#include <cassert>
#include <iostream>
#include <memory>
#include <string>

void testBasicOperations() {
    std::cout << "[Test LinkedList] Basic push/pop operations... ";
    dsa::LinkedList<int> list;
    assert(list.empty());
    assert(list.size() == 0);

    list.push_back(10);
    list.push_back(20);
    list.push_front(5);
    // Sequence: 5, 10, 20
    assert(list.size() == 3);
    assert(list.front() == 5);
    assert(list.back() == 20);

    list.pop_front(); // Sequence: 10, 20
    assert(list.size() == 2);
    assert(list.front() == 10);

    list.pop_back(); // Sequence: 10
    assert(list.size() == 1);
    assert(list.front() == 10);
    assert(list.back() == 10);

    list.pop_back();
    assert(list.empty());
    assert(list.size() == 0);

    std::cout << "PASSED\n";
}

void testBidirectionalIterators() {
    std::cout << "[Test LinkedList] Bidirectional Iteration... ";
    dsa::LinkedList<int> list{1, 2, 3, 4, 5};
    assert(list.size() == 5);

    // Forward iteration
    int expected = 1;
    for (auto it = list.begin(); it != list.end(); ++it) {
        assert(*it == expected);
        ++expected;
    }

    // Backward iteration
    auto revIt = list.end();
    --revIt;
    expected = 5;
    while (true) {
        assert(*revIt == expected);
        --expected;
        if (revIt == list.begin()) break;
        --revIt;
    }
    assert(expected == 0);

    std::cout << "PASSED\n";
}

void testInsertAndErase() {
    std::cout << "[Test LinkedList] Insert & Erase at arbitrary positions... ";
    dsa::LinkedList<std::string> list{"alpha", "gamma"};

    // Insert "beta" before "gamma"
    auto it = list.begin();
    ++it; // Points to "gamma"
    list.insert(it, "beta");

    // Sequence: "alpha", "beta", "gamma"
    assert(list.size() == 3);
    it = list.begin();
    assert(*it == "alpha");
    ++it;
    assert(*it == "beta");
    ++it;
    assert(*it == "gamma");

    // Erase "beta"
    it = list.begin();
    ++it; // Points to "beta"
    auto nextIt = list.erase(it);
    assert(*nextIt == "gamma");
    assert(list.size() == 2);
    assert(list.front() == "alpha");
    assert(list.back() == "gamma");

    std::cout << "PASSED\n";
}

void testCopyAndMoveSemantics() {
    std::cout << "[Test LinkedList] Copy and Move semantics... ";
    dsa::LinkedList<int> original{100, 200, 300};

    // Deep Copy
    dsa::LinkedList<int> copy = original;
    assert(copy.size() == 3);
    copy.push_back(400);
    assert(copy.size() == 4);
    assert(original.size() == 3); // Original unaffected

    // Move Construction
    dsa::LinkedList<int> moved = std::move(original);
    assert(moved.size() == 3);
    assert(original.empty());

    // Copy Assignment
    dsa::LinkedList<int> assigned;
    assigned = copy;
    assert(assigned.size() == 4);

    // Move Assignment
    dsa::LinkedList<int> moveAssigned;
    moveAssigned = std::move(moved);
    assert(moveAssigned.size() == 3);
    assert(moved.empty());

    std::cout << "PASSED\n";
}

void testMoveOnlyTypes() {
    std::cout << "[Test LinkedList] Move-only types (std::unique_ptr)... ";
    dsa::LinkedList<std::unique_ptr<int>> list;

    list.push_back(std::make_unique<int>(10));
    list.push_front(std::make_unique<int>(5));
    list.emplace_back(new int(20));

    assert(list.size() == 3);
    assert(*list.front() == 5);
    assert(*list.back() == 20);

    dsa::LinkedList<std::unique_ptr<int>> moved = std::move(list);
    assert(moved.size() == 3);
    assert(list.empty());

    std::cout << "PASSED\n";
}

void testExceptions() {
    std::cout << "[Test LinkedList] Exception safety on empty access... ";
    dsa::LinkedList<int> emptyList;

    bool caughtFront = false;
    try {
        emptyList.front();
    } catch (const std::out_of_range&) {
        caughtFront = true;
    }
    assert(caughtFront);

    bool caughtBack = false;
    try {
        emptyList.back();
    } catch (const std::out_of_range&) {
        caughtBack = true;
    }
    assert(caughtBack);

    bool caughtPopFront = false;
    try {
        emptyList.pop_front();
    } catch (const std::out_of_range&) {
        caughtPopFront = true;
    }
    assert(caughtPopFront);

    bool caughtPopBack = false;
    try {
        emptyList.pop_back();
    } catch (const std::out_of_range&) {
        caughtPopBack = true;
    }
    assert(caughtPopBack);

    bool caughtEraseEnd = false;
    try {
        emptyList.erase(emptyList.end());
    } catch (const std::out_of_range&) {
        caughtEraseEnd = true;
    }
    assert(caughtEraseEnd);

    std::cout << "PASSED\n";
}

int main() {
    std::cout << "===========================================\n";
    std::cout << " Running LinkedList<T> Test Suite\n";
    std::cout << "===========================================\n";

    testBasicOperations();
    testBidirectionalIterators();
    testInsertAndErase();
    testCopyAndMoveSemantics();
    testMoveOnlyTypes();
    testExceptions();

    std::cout << ">>> ALL LINKEDLIST TESTS PASSED! <<<\n\n";
    return 0;
}

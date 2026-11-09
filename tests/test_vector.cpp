#include "dsa/linear/Vector.hpp"
#include <cassert>
#include <iostream>
#include <memory>
#include <string>

struct NonDefaultConstructible {
    int value;
    explicit NonDefaultConstructible(int v) : value(v) {}
};

void testBasicOperations() {
    std::cout << "[Test Vector] Basic operations... ";
    dsa::Vector<int> vec;
    assert(vec.empty());
    assert(vec.size() == 0);
    assert(vec.capacity() == 0);

    for (int i = 1; i <= 10; ++i) {
        vec.push_back(i * 10);
    }

    assert(!vec.empty());
    assert(vec.size() == 10);
    assert(vec.capacity() >= 10);

    for (std::size_t i = 0; i < 10; ++i) {
        assert(vec[i] == static_cast<int>((i + 1) * 10));
        assert(vec.at(i) == static_cast<int>((i + 1) * 10));
    }

    assert(vec.front() == 10);
    assert(vec.back() == 100);

    vec.pop_back();
    assert(vec.size() == 9);
    assert(vec.back() == 90);

    std::cout << "PASSED\n";
}

void testCopyAndMoveSemantics() {
    std::cout << "[Test Vector] Copy and Move semantics... ";
    dsa::Vector<std::string> orig{"alpha", "beta", "gamma"};
    assert(orig.size() == 3);

    // Deep Copy
    dsa::Vector<std::string> copy = orig;
    assert(copy.size() == 3);
    assert(copy[0] == "alpha");
    copy[0] = "mutated";
    assert(orig[0] == "alpha"); // Original remains untouched

    // Move Construction
    dsa::Vector<std::string> moved = std::move(orig);
    assert(moved.size() == 3);
    assert(moved[0] == "alpha");
    assert(orig.empty());
    assert(orig.data() == nullptr);

    // Copy Assignment
    dsa::Vector<std::string> assigned;
    assigned = copy;
    assert(assigned.size() == 3);
    assert(assigned[0] == "mutated");

    // Move Assignment
    dsa::Vector<std::string> moveAssigned;
    moveAssigned = std::move(moved);
    assert(moveAssigned.size() == 3);
    assert(moved.empty());

    std::cout << "PASSED\n";
}

void testMoveOnlyTypes() {
    std::cout << "[Test Vector] Move-only types (std::unique_ptr)... ";
    dsa::Vector<std::unique_ptr<int>> ptrVec;

    for (int i = 0; i < 5; ++i) {
        ptrVec.push_back(std::make_unique<int>(i * 5));
    }

    assert(ptrVec.size() == 5);
    for (int i = 0; i < 5; ++i) {
        assert(*ptrVec[i] == i * 5);
    }

    ptrVec.emplace_back(new int(999));
    assert(ptrVec.size() == 6);
    assert(*ptrVec.back() == 999);

    // Test moving vector of move-only types
    dsa::Vector<std::unique_ptr<int>> target = std::move(ptrVec);
    assert(target.size() == 6);
    assert(*target.back() == 999);
    assert(ptrVec.empty());

    std::cout << "PASSED\n";
}

void testNonDefaultConstructible() {
    std::cout << "[Test Vector] Non-default constructible structs... ";
    dsa::Vector<NonDefaultConstructible> vec;
    vec.reserve(10);
    assert(vec.capacity() == 10);
    assert(vec.size() == 0);

    vec.emplace_back(42);
    vec.emplace_back(84);
    assert(vec.size() == 2);
    assert(vec[0].value == 42);
    assert(vec[1].value == 84);

    std::cout << "PASSED\n";
}

void testExceptionSafetyAndBounds() {
    std::cout << "[Test Vector] Exception safety & bounds checking... ";
    dsa::Vector<int> vec{1, 2, 3};

    bool caught = false;
    try {
        vec.at(3); // Out of bounds
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);

    dsa::Vector<int> emptyVec;
    bool caughtFront = false;
    try {
        emptyVec.front();
    } catch (const std::out_of_range&) {
        caughtFront = true;
    }
    assert(caughtFront);

    bool caughtPop = false;
    try {
        emptyVec.pop_back();
    } catch (const std::out_of_range&) {
        caughtPop = true;
    }
    assert(caughtPop);

    std::cout << "PASSED\n";
}

void testIteratorsAndGrowth() {
    std::cout << "[Test Vector] Iterators and Range-based loops... ";
    dsa::Vector<int> vec{10, 20, 30, 40, 50};

    int sum = 0;
    for (int val : vec) {
        sum += val;
    }
    assert(sum == 150);

    vec.resize(3);
    assert(vec.size() == 3);
    assert(vec.back() == 30);

    vec.resize(6, 99);
    assert(vec.size() == 6);
    assert(vec[3] == 99 && vec[4] == 99 && vec[5] == 99);

    vec.shrink_to_fit();
    assert(vec.capacity() == 6);

    std::cout << "PASSED\n";
}

int main() {
    std::cout << "===========================================\n";
    std::cout << " Running Vector<T> Test Suite\n";
    std::cout << "===========================================\n";

    testBasicOperations();
    testCopyAndMoveSemantics();
    testMoveOnlyTypes();
    testNonDefaultConstructible();
    testExceptionSafetyAndBounds();
    testIteratorsAndGrowth();

    std::cout << ">>> ALL VECTOR TESTS PASSED! <<<\n\n";
    return 0;
}

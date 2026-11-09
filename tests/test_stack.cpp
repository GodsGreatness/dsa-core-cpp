#include "dsa/linear/Stack.hpp"
#include <cassert>
#include <iostream>
#include <memory>
#include <string>

void testLIFOOrdering() {
    std::cout << "[Test Stack] LIFO ordering & basic operations... ";
    dsa::Stack<int> stack;
    assert(stack.empty());
    assert(stack.size() == 0);

    stack.push(10);
    stack.push(20);
    stack.push(30);

    assert(!stack.empty());
    assert(stack.size() == 3);
    assert(stack.top() == 30);

    stack.pop();
    assert(stack.size() == 2);
    assert(stack.top() == 20);

    stack.pop();
    assert(stack.size() == 1);
    assert(stack.top() == 10);

    stack.pop();
    assert(stack.empty());
    assert(stack.size() == 0);

    std::cout << "PASSED\n";
}

void testEmplaceAndMoveOnly() {
    std::cout << "[Test Stack] Emplace & Move-only types (std::unique_ptr)... ";
    dsa::Stack<std::unique_ptr<std::string>> stack;

    stack.push(std::make_unique<std::string>("first"));
    stack.emplace(new std::string("second"));

    assert(stack.size() == 2);
    assert(*stack.top() == "second");

    stack.pop();
    assert(stack.size() == 1);
    assert(*stack.top() == "first");

    std::cout << "PASSED\n";
}

void testCopyAndMove() {
    std::cout << "[Test Stack] Copy and Move semantics... ";
    dsa::Stack<std::string> stack;
    stack.push("A");
    stack.push("B");

    dsa::Stack<std::string> copy = stack;
    assert(copy.size() == 2);
    assert(copy.top() == "B");

    dsa::Stack<std::string> moved = std::move(stack);
    assert(moved.size() == 2);
    assert(moved.top() == "B");

    std::cout << "PASSED\n";
}

void testUnderflowExceptions() {
    std::cout << "[Test Stack] Underflow exception handling... ";
    dsa::Stack<int> emptyStack;

    bool caughtTop = false;
    try {
        emptyStack.top();
    } catch (const std::underflow_error&) {
        caughtTop = true;
    }
    assert(caughtTop);

    bool caughtPop = false;
    try {
        emptyStack.pop();
    } catch (const std::underflow_error&) {
        caughtPop = true;
    }
    assert(caughtPop);

    std::cout << "PASSED\n";
}

int main() {
    std::cout << "===========================================\n";
    std::cout << " Running Stack<T> Test Suite\n";
    std::cout << "===========================================\n";

    testLIFOOrdering();
    testEmplaceAndMoveOnly();
    testCopyAndMove();
    testUnderflowExceptions();

    std::cout << ">>> ALL STACK TESTS PASSED! <<<\n\n";
    return 0;
}

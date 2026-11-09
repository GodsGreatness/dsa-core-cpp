#include "dsa/trees/BST.hpp"
#include <cassert>
#include <iostream>
#include <vector>

void testInsertAndContains() {
    std::cout << "[Test BST] Insert and Contains... ";
    dsa::BST<int> tree;
    assert(tree.empty());
    assert(tree.size() == 0);

    assert(tree.insert(50));
    assert(tree.insert(30));
    assert(tree.insert(70));
    assert(tree.insert(20));
    assert(tree.insert(40));
    assert(!tree.insert(30)); // Duplicate rejected

    assert(tree.size() == 5);
    assert(tree.contains(50));
    assert(tree.contains(20));
    assert(tree.contains(70));
    assert(!tree.contains(999));

    assert(tree.min() == 20);
    assert(tree.max() == 70);

    std::cout << "PASSED\n";
}

void testTraversals() {
    std::cout << "[Test BST] Traversals (In-order, Pre-order, Level-order)... ";
    dsa::BST<int> tree{50, 30, 70, 20, 40, 60, 80};

    // Inorder should produce sorted output
    std::vector<int> inorderResult;
    tree.inorder([&](int val) { inorderResult.push_back(val); });
    assert(inorderResult == (std::vector<int>{20, 30, 40, 50, 60, 70, 80}));

    // Preorder: Root, Left, Right
    std::vector<int> preorderResult;
    tree.preorder([&](int val) { preorderResult.push_back(val); });
    assert(preorderResult == (std::vector<int>{50, 30, 20, 40, 70, 60, 80}));

    // Level-order (BFS)
    std::vector<int> levelResult;
    tree.levelorder([&](int val) { levelResult.push_back(val); });
    assert(levelResult == (std::vector<int>{50, 30, 70, 20, 40, 60, 80}));

    std::cout << "PASSED\n";
}

void testNodeDeletion() {
    std::cout << "[Test BST] Node deletion (0, 1, and 2 children)... ";
    dsa::BST<int> tree{50, 30, 70, 20, 40, 60, 80, 65};

    // Case 1: Delete Leaf Node (20)
    assert(tree.remove(20));
    assert(!tree.contains(20));
    assert(tree.size() == 7);

    // Case 2: Delete Node with 1 child (60 has right child 65)
    assert(tree.remove(60));
    assert(!tree.contains(60));
    assert(tree.contains(65));
    assert(tree.size() == 6);

    // Case 3: Delete Node with 2 children (50 root)
    assert(tree.remove(50));
    assert(!tree.contains(50));
    assert(tree.size() == 5);

    // Verify ordering is maintained after all deletions
    std::vector<int> remaining = tree.toVector();
    assert(remaining == (std::vector<int>{30, 40, 65, 70, 80}));

    std::cout << "PASSED\n";
}

void testCopyAndMove() {
    std::cout << "[Test BST] Copy & Move semantics... ";
    dsa::BST<int> tree{10, 5, 15};

    // Deep Copy
    dsa::BST<int> copy = tree;
    assert(copy.size() == 3);
    copy.insert(25);
    assert(copy.size() == 4);
    assert(tree.size() == 3); // Original unaffected

    // Move Construction
    dsa::BST<int> moved = std::move(tree);
    assert(moved.size() == 3);
    assert(tree.empty());

    std::cout << "PASSED\n";
}

int main() {
    std::cout << "===========================================\n";
    std::cout << " Running BST<T> Test Suite\n";
    std::cout << "===========================================\n";

    testInsertAndContains();
    testTraversals();
    testNodeDeletion();
    testCopyAndMove();

    std::cout << ">>> ALL BST TESTS PASSED! <<<\n\n";
    return 0;
}

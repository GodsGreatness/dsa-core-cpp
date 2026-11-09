#include "dsa/trees/AVLTree.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

void testRotationsLL() {
    std::cout << "[Test AVL] Single Right Rotation (LL Case)... ";
    dsa::AVLTree<int> tree;
    // LL Case: Inserting descending sequence: 30, 20, 10
    tree.insert(30);
    tree.insert(20);
    tree.insert(10); // Causes balance factor violation at 30 -> Right rotation

    assert(tree.size() == 3);
    assert(tree.height() == 2);
    assert(tree.verifyInvariants());
    assert(tree.toVector() == (std::vector<int>{10, 20, 30}));

    std::cout << "PASSED\n";
}

void testRotationsRR() {
    std::cout << "[Test AVL] Single Left Rotation (RR Case)... ";
    dsa::AVLTree<int> tree;
    // RR Case: Inserting ascending sequence: 10, 20, 30
    tree.insert(10);
    tree.insert(20);
    tree.insert(30); // Causes balance factor violation at 10 -> Left rotation

    assert(tree.size() == 3);
    assert(tree.height() == 2);
    assert(tree.verifyInvariants());
    assert(tree.toVector() == (std::vector<int>{10, 20, 30}));

    std::cout << "PASSED\n";
}

void testRotationsLR() {
    std::cout << "[Test AVL] Double Rotation (LR Case)... ";
    dsa::AVLTree<int> tree;
    // LR Case: 30, 10, 20
    tree.insert(30);
    tree.insert(10);
    tree.insert(20); // Left rotation on 10, Right rotation on 30

    assert(tree.size() == 3);
    assert(tree.height() == 2);
    assert(tree.verifyInvariants());
    assert(tree.toVector() == (std::vector<int>{10, 20, 30}));

    std::cout << "PASSED\n";
}

void testRotationsRL() {
    std::cout << "[Test AVL] Double Rotation (RL Case)... ";
    dsa::AVLTree<int> tree;
    // RL Case: 10, 30, 20
    tree.insert(10);
    tree.insert(30);
    tree.insert(20); // Right rotation on 30, Left rotation on 10

    assert(tree.size() == 3);
    assert(tree.height() == 2);
    assert(tree.verifyInvariants());
    assert(tree.toVector() == (std::vector<int>{10, 20, 30}));

    std::cout << "PASSED\n";
}

void testRebalancingOnDeletion() {
    std::cout << "[Test AVL] Rebalancing on deletion... ";
    dsa::AVLTree<int> tree{50, 25, 75, 15, 35, 65, 85, 10};
    assert(tree.verifyInvariants());

    // Deleting 85 causes right subtree of 50 to become shorter, rebalancing 50
    assert(tree.remove(85));
    assert(!tree.contains(85));
    assert(tree.verifyInvariants());

    // Deleting 75 and 65
    assert(tree.remove(75));
    assert(tree.remove(65));
    assert(tree.verifyInvariants());

    std::cout << "PASSED\n";
}

void testFuzzRandomizedStress() {
    std::cout << "[Test AVL] Randomized Stress Test (5,000 insertions & deletions)... ";
    dsa::AVLTree<int> tree;
    std::mt19937 rng(1337); // Deterministic seed
    std::uniform_int_distribution<int> dist(1, 100000);

    std::vector<int> inserted;
    inserted.reserve(5000);

    // 1. Mass insertions
    for (int i = 0; i < 5000; ++i) {
        int val = dist(rng);
        if (tree.insert(val)) {
            inserted.push_back(val);
        }
    }

    assert(tree.verifyInvariants());
    assert(tree.size() == inserted.size());

    // In-order traversal must be strictly ascending
    std::vector<int> sortedVals = tree.toVector();
    assert(std::is_sorted(sortedVals.begin(), sortedVals.end()));

    // 2. Mass deletions
    std::shuffle(inserted.begin(), inserted.end(), rng);
    std::size_t toDelete = inserted.size() / 2;

    for (std::size_t i = 0; i < toDelete; ++i) {
        assert(tree.remove(inserted[i]));
    }

    assert(tree.verifyInvariants());
    assert(tree.size() == inserted.size() - toDelete);

    // Verify remaining elements are still strictly sorted
    std::vector<int> remainingVals = tree.toVector();
    assert(std::is_sorted(remainingVals.begin(), remainingVals.end()));

    std::cout << "PASSED\n";
}

int main() {
    std::cout << "===========================================\n";
    std::cout << " Running AVLTree<T> Test Suite (Flagship)\n";
    std::cout << "===========================================\n";

    testRotationsLL();
    testRotationsRR();
    testRotationsLR();
    testRotationsRL();
    testRebalancingOnDeletion();
    testFuzzRandomizedStress();

    std::cout << ">>> ALL AVL TREE TESTS & INVARIANT CHECKS PASSED! <<<\n\n";
    return 0;
}

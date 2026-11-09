#include "dsa/trees/AVLTree.hpp"
#include <iostream>

int main() {
    std::cout << "=== dsa::AVLTree Flagship Example ===\n\n";

    dsa::AVLTree<int> tree;

    // Insert keys in descending order (would degrade regular BST to O(N) linked list)
    std::cout << "Inserting keys [50, 40, 30, 20, 10]...\n";
    tree.insert(50);
    tree.insert(40);
    tree.insert(30);
    tree.insert(20);
    tree.insert(10);

    std::cout << "Tree Size: " << tree.size() << "\n";
    std::cout << "Tree Height: " << tree.height() << " (O(log N) balance maintained!)\n";
    std::cout << "Invariants Valid: " << (tree.verifyInvariants() ? "YES (|BF| <= 1)" : "NO") << "\n\n";

    std::cout << "In-order traversal: ";
    tree.inorder([](int val) { std::cout << val << " "; });
    std::cout << "\n";

    std::cout << "\nRemoving key 30...\n";
    tree.remove(30);
    std::cout << "After removal, Invariants Valid: " << (tree.verifyInvariants() ? "YES" : "NO") << "\n";

    return 0;
}

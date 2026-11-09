#include "dsa/algorithms/GraphAlgorithms.hpp"
#include "dsa/algorithms/Sorting.hpp"
#include "dsa/heap/PriorityQueue.hpp"
#include "dsa/linear/LinkedList.hpp"
#include "dsa/linear/Queue.hpp"
#include "dsa/linear/Stack.hpp"
#include "dsa/linear/Vector.hpp"
#include "dsa/trees/AVLTree.hpp"
#include "dsa/trees/BST.hpp"
#include <iostream>
#include <string>

void runVectorDemo() {
    std::cout << "\n--- Vector Demo ---\n";
    dsa::Vector<std::string> vec;
    vec.push_back("Data");
    vec.push_back("Structures");
    vec.push_back("Core");
    std::cout << "Vector Size: " << vec.size() << ", Capacity: " << vec.capacity() << "\n";
    for (std::size_t i = 0; i < vec.size(); ++i) {
        std::cout << "  [" << i << "] " << vec[i] << "\n";
    }
}

void runAVLDemo() {
    std::cout << "\n--- Flagship AVL Tree Demo ---\n";
    dsa::AVLTree<int> tree;
    std::cout << "Inserting [10, 20, 30, 40, 50, 25] into AVL tree...\n";
    for (int v : {10, 20, 30, 40, 50, 25}) {
        tree.insert(v);
    }
    std::cout << "AVL Tree Size: " << tree.size() << ", Height: " << tree.height() << "\n";
    std::cout << "Balance Invariant Check: " << (tree.verifyInvariants() ? "PASSED (|BF| <= 1)" : "FAILED") << "\n";
    std::cout << "Sorted In-order Sequence: ";
    tree.inorder([](int v) { std::cout << v << " "; });
    std::cout << "\n";
}

void runGraphDemo() {
    std::cout << "\n--- Graph & Dijkstra Demo ---\n";
    dsa::Graph<std::string, double> g(false);
    g.addEdge("NodeA", "NodeB", 4.0);
    g.addEdge("NodeA", "NodeC", 2.0);
    g.addEdge("NodeC", "NodeB", 1.0);
    g.addEdge("NodeB", "NodeD", 5.0);

    auto res = dsa::GraphAlgorithms::dijkstra(g, std::string("NodeA"));
    std::cout << "Shortest path NodeA -> NodeD: ";
    auto path = res.getPathTo("NodeD");
    for (std::size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i] << (i + 1 < path.size() ? " -> " : "");
    }
    std::cout << " (Total Distance: " << res.distanceTo("NodeD") << ")\n";
}

void runSortingDemo() {
    std::cout << "\n--- Sorting Suite Demo ---\n";
    std::vector<int> data{45, 12, 85, 32, 89, 39, 69, 44, 42, 1, 45};
    std::cout << "Original: ";
    for (int v : data) std::cout << v << " ";
    std::cout << "\n";

    dsa::Sorting::quickSort(data);
    std::cout << "Sorted (QuickSort): ";
    for (int v : data) std::cout << v << " ";
    std::cout << "\n";
}

void printMenu() {
    std::cout << "\n==============================\n";
    std::cout << "      DSA Core C++17 CLI      \n";
    std::cout << "==============================\n";
    std::cout << "  1. Vector Demo\n";
    std::cout << "  2. AVL Tree Invariant Demo\n";
    std::cout << "  3. Graph & Dijkstra Demo\n";
    std::cout << "  4. Sorting Algorithms Demo\n";
    std::cout << "  0. Exit\n";
    std::cout << "Enter choice: ";
}

int main(int argc, char* argv[]) {
    if (argc > 1 && (std::string(argv[1]) == "--demo" || std::string(argv[1]) == "-d")) {
        runVectorDemo();
        runAVLDemo();
        runGraphDemo();
        runSortingDemo();
        return 0;
    }

    std::cout << "DSA Core C++17 Library & Demonstration CLI\n";
    runVectorDemo();
    runAVLDemo();
    runGraphDemo();
    runSortingDemo();
    std::cout << "\nAll demonstrations completed successfully.\n";

    return 0;
}

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

void printHelp() {
    std::cout << "DSA Core C++17 Library & Demonstration CLI\n\n";
    std::cout << "Usage: ./dsa_demo [options]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --all, -a        Run all module demonstrations (default)\n";
    std::cout << "  --vector         Run Vector demonstration\n";
    std::cout << "  --avl            Run AVL Tree self-balancing demonstration\n";
    std::cout << "  --graph          Run Graph & Dijkstra demonstration\n";
    std::cout << "  --sort           Run Sorting suite demonstration\n";
    std::cout << "  --version, -v    Display version information\n";
    std::cout << "  --help, -h       Display this help message\n\n";
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--version" || arg == "-v") {
            std::cout << "DSA Core C++17 v1.0.0\n";
            return 0;
        } else if (arg == "--help" || arg == "-h") {
            printHelp();
            return 0;
        } else if (arg == "--vector") {
            runVectorDemo();
            return 0;
        } else if (arg == "--avl") {
            runAVLDemo();
            return 0;
        } else if (arg == "--graph") {
            runGraphDemo();
            return 0;
        } else if (arg == "--sort") {
            runSortingDemo();
            return 0;
        }
    }

    std::cout << "DSA Core C++17 Library & Demonstration CLI\n";
    runVectorDemo();
    runAVLDemo();
    runGraphDemo();
    runSortingDemo();
    std::cout << "\nAll demonstrations completed successfully.\n";

    return 0;
}

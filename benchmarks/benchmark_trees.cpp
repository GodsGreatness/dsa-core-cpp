#include "dsa/linear/Vector.hpp"
#include "dsa/trees/AVLTree.hpp"
#include "dsa/trees/BST.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

int main() {
    std::cout << "========================================================================\n";
    std::cout << "  Empirical Scaling Benchmark: Search Time vs Scale (AVL vs BST vs Linear)\n";
    std::cout << "========================================================================\n\n";

    std::vector<std::size_t> scales = {1000, 10000, 50000, 100000};
    std::mt19937 rng(42);

    std::cout << "+------------+----------------------+----------------------+----------------------+\n";
    std::cout << "| Scale (N)  | AVL Search (O(logN)) | BST Search (O(logN)) | Linear Search (O(N)) |\n";
    std::cout << "+------------+----------------------+----------------------+----------------------+\n";

    for (std::size_t n : scales) {
        std::uniform_int_distribution<int> dist(1, static_cast<int>(n * 10));
        std::vector<int> dataset;
        dataset.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            dataset.push_back(dist(rng));
        }

        dsa::AVLTree<int> avl;
        dsa::BST<int> bst;
        dsa::Vector<int> vec;

        for (int v : dataset) {
            avl.insert(v);
            bst.insert(v);
            vec.push_back(v);
        }

        // Generate query batch (1,000 lookups)
        const int numQueries = 1000;
        std::vector<int> queries;
        queries.reserve(numQueries);
        for (int i = 0; i < numQueries; ++i) {
            queries.push_back(dist(rng));
        }

        // 1. AVL Search
        auto t0 = std::chrono::high_resolution_clock::now();
        volatile int avlFound = 0;
        for (int q : queries) {
            if (avl.contains(q)) avlFound = avlFound + 1;
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        double avlTime = std::chrono::duration<double, std::micro>(t1 - t0).count();

        // 2. BST Search
        auto t2 = std::chrono::high_resolution_clock::now();
        volatile int bstFound = 0;
        for (int q : queries) {
            if (bst.contains(q)) bstFound = bstFound + 1;
        }
        auto t3 = std::chrono::high_resolution_clock::now();
        double bstTime = std::chrono::duration<double, std::micro>(t3 - t2).count();

        // 3. Vector Linear Search
        auto t4 = std::chrono::high_resolution_clock::now();
        volatile int vecFound = 0;
        for (int q : queries) {
            for (std::size_t idx = 0; idx < vec.size(); ++idx) {
                if (vec[idx] == q) {
                    vecFound = vecFound + 1;
                    break;
                }
            }
        }
        auto t5 = std::chrono::high_resolution_clock::now();
        double vecTime = std::chrono::duration<double, std::micro>(t5 - t4).count();

        std::cout << "| " << std::setw(10) << n << " | "
                  << std::setw(17) << std::fixed << std::setprecision(2) << avlTime << " us | "
                  << std::setw(17) << std::fixed << std::setprecision(2) << bstTime << " us | "
                  << std::setw(17) << std::fixed << std::setprecision(2) << vecTime << " us |\n";
    }

    std::cout << "+------------+----------------------+----------------------+----------------------+\n\n";
    return 0;
}

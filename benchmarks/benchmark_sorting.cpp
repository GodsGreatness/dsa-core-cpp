#include "dsa/algorithms/Sorting.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>

int main() {
    std::cout << "========================================================================\n";
    std::cout << "  Empirical Scaling Benchmark: Sorting Algorithms (O(n^2) vs O(n log n))\n";
    std::cout << "========================================================================\n\n";

    std::vector<std::size_t> scales = {1000, 5000, 10000, 25000};
    std::mt19937 rng(1234);

    std::cout << "+------------+---------------------+---------------------+---------------------+---------------------+\n";
    std::cout << "| Scale (N)  | InsertionSort (O(N2)| MergeSort (O(NlogN))| QuickSort (O(NlogN))| HeapSort (O(NlogN)) |\n";
    std::cout << "+------------+---------------------+---------------------+---------------------+---------------------+\n";

    for (std::size_t n : scales) {
        std::uniform_int_distribution<int> dist(-100000, 100000);
        std::vector<int> original;
        original.reserve(n);
        for (std::size_t i = 0; i < n; ++i) {
            original.push_back(dist(rng));
        }

        // 1. Insertion Sort
        auto d1 = original;
        auto t0 = std::chrono::high_resolution_clock::now();
        dsa::Sorting::insertionSort(d1);
        auto t1 = std::chrono::high_resolution_clock::now();
        double insertTime = std::chrono::duration<double, std::milli>(t1 - t0).count();

        // 2. Merge Sort
        auto d2 = original;
        auto t2 = std::chrono::high_resolution_clock::now();
        dsa::Sorting::mergeSort(d2);
        auto t3 = std::chrono::high_resolution_clock::now();
        double mergeTime = std::chrono::duration<double, std::milli>(t3 - t2).count();

        // 3. Quick Sort
        auto d3 = original;
        auto t4 = std::chrono::high_resolution_clock::now();
        dsa::Sorting::quickSort(d3);
        auto t5 = std::chrono::high_resolution_clock::now();
        double quickTime = std::chrono::duration<double, std::milli>(t5 - t4).count();

        // 4. Heap Sort
        auto d4 = original;
        auto t6 = std::chrono::high_resolution_clock::now();
        dsa::Sorting::heapSort(d4);
        auto t7 = std::chrono::high_resolution_clock::now();
        double heapTime = std::chrono::duration<double, std::milli>(t7 - t6).count();

        std::cout << "| " << std::setw(10) << n << " | "
                  << std::setw(16) << std::fixed << std::setprecision(2) << insertTime << " ms | "
                  << std::setw(16) << std::fixed << std::setprecision(2) << mergeTime << " ms | "
                  << std::setw(16) << std::fixed << std::setprecision(2) << quickTime << " ms | "
                  << std::setw(16) << std::fixed << std::setprecision(2) << heapTime << " ms |\n";
    }

    std::cout << "+------------+---------------------+---------------------+---------------------+---------------------+\n\n";
    return 0;
}

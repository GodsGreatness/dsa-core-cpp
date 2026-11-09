#include "dsa/algorithms/Sorting.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <vector>

void testInsertionSort() {
    std::cout << "[Test Sorting] Insertion Sort correctness & stability... ";
    std::vector<int> data{64, 25, 12, 22, 11};
    dsa::Sorting::insertionSort(data);
    assert(data == (std::vector<int>{11, 12, 22, 25, 64}));

    // Already sorted
    dsa::Sorting::insertionSort(data);
    assert(data == (std::vector<int>{11, 12, 22, 25, 64}));

    // Single element & empty
    std::vector<int> single{42};
    dsa::Sorting::insertionSort(single);
    assert(single.size() == 1 && single[0] == 42);

    std::vector<int> empty;
    dsa::Sorting::insertionSort(empty);
    assert(empty.empty());

    std::cout << "PASSED\n";
}

void testMergeSortStability() {
    std::cout << "[Test Sorting] Merge Sort stability verification... ";
    struct Item {
        int key;
        int originalIndex;
    };

    std::vector<Item> items = {
        {3, 0}, {1, 1}, {3, 2}, {2, 3}, {3, 4}, {1, 5}
    };

    dsa::Sorting::mergeSort(items, [](const Item& a, const Item& b) {
        return a.key < b.key;
    });

    // Check sorted by key
    for (std::size_t i = 1; i < items.size(); ++i) {
        assert(items[i - 1].key <= items[i].key);
    }

    // Check stability: for key 1, index 1 appears before index 5
    assert(items[0].key == 1 && items[0].originalIndex == 1);
    assert(items[1].key == 1 && items[1].originalIndex == 5);

    // Check stability: for key 3, indices are 0, 2, 4 in that order
    assert(items[3].key == 3 && items[3].originalIndex == 0);
    assert(items[4].key == 3 && items[4].originalIndex == 2);
    assert(items[5].key == 3 && items[5].originalIndex == 4);

    std::cout << "PASSED\n";
}

void testQuickSort() {
    std::cout << "[Test Sorting] Quick Sort with median-of-3 & duplicates... ";
    std::vector<int> data{10, 80, 30, 90, 40, 50, 70};
    dsa::Sorting::quickSort(data);
    assert(std::is_sorted(data.begin(), data.end()));

    // All duplicates
    std::vector<int> dups(100, 5);
    dsa::Sorting::quickSort(dups);
    assert(std::is_sorted(dups.begin(), dups.end()));

    // Reverse sorted
    std::vector<int> rev;
    for (int i = 1000; i >= 1; --i) rev.push_back(i);
    dsa::Sorting::quickSort(rev);
    assert(std::is_sorted(rev.begin(), rev.end()));

    std::cout << "PASSED\n";
}

void testHeapSort() {
    std::cout << "[Test Sorting] In-place Heap Sort... ";
    std::vector<int> data{12, 11, 13, 5, 6, 7};
    dsa::Sorting::heapSort(data);
    assert(data == (std::vector<int>{5, 6, 7, 11, 12, 13}));

    std::cout << "PASSED\n";
}

void testRandomizedStressAll() {
    std::cout << "[Test Sorting] Randomized stress test across all 4 algorithms (5,000 elements)... ";
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(-10000, 10000);

    std::vector<int> dataset;
    dataset.reserve(5000);
    for (int i = 0; i < 5000; ++i) {
        dataset.push_back(dist(rng));
    }

    auto d1 = dataset;
    dsa::Sorting::insertionSort(d1);
    assert(std::is_sorted(d1.begin(), d1.end()));

    auto d2 = dataset;
    dsa::Sorting::mergeSort(d2);
    assert(std::is_sorted(d2.begin(), d2.end()));

    auto d3 = dataset;
    dsa::Sorting::quickSort(d3);
    assert(std::is_sorted(d3.begin(), d3.end()));

    auto d4 = dataset;
    dsa::Sorting::heapSort(d4);
    assert(std::is_sorted(d4.begin(), d4.end()));

    std::cout << "PASSED\n";
}

int main() {
    std::cout << "===========================================\n";
    std::cout << " Running Sorting Algorithms Test Suite\n";
    std::cout << "===========================================\n";

    testInsertionSort();
    testMergeSortStability();
    testQuickSort();
    testHeapSort();
    testRandomizedStressAll();

    std::cout << ">>> ALL SORTING TESTS PASSED! <<<\n\n";
    return 0;
}

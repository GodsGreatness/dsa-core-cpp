#ifndef DSA_ALGORITHMS_SORTING_HPP
#define DSA_ALGORITHMS_SORTING_HPP

#include <algorithm>
#include <functional>
#include <iterator>
#include <utility>
#include <vector>

namespace dsa {

class Sorting {
public:
    // -------------------------------------------------------------
    // 1. Insertion Sort (O(n^2) worst, O(n) best, O(1) space, Stable)
    // -------------------------------------------------------------
    template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
    static void insertionSort(RandomIt first, RandomIt last, Compare comp = Compare()) {
        if (first == last) return;

        for (RandomIt i = std::next(first); i != last; ++i) {
            auto key = std::move(*i);
            RandomIt j = i;
            while (j != first && comp(key, *std::prev(j))) {
                *j = std::move(*std::prev(j));
                --j;
            }
            *j = std::move(key);
        }
    }

    template <typename Container, typename Compare = std::less<typename Container::value_type>>
    static void insertionSort(Container& c, Compare comp = Compare()) {
        insertionSort(std::begin(c), std::end(c), comp);
    }

    // -------------------------------------------------------------
    // 2. Merge Sort (O(n log n) worst/avg, O(n) space, Stable)
    // -------------------------------------------------------------
    template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
    static void mergeSort(RandomIt first, RandomIt last, Compare comp = Compare()) {
        auto count = std::distance(first, last);
        if (count <= 1) return;

        RandomIt mid = first + count / 2;
        mergeSort(first, mid, comp);
        mergeSort(mid, last, comp);

        using ValueType = typename std::iterator_traits<RandomIt>::value_type;
        std::vector<ValueType> temp;
        temp.reserve(count);

        RandomIt left = first;
        RandomIt right = mid;

        while (left != mid && right != last) {
            if (comp(*right, *left)) {
                temp.push_back(std::move(*right));
                ++right;
            } else {
                temp.push_back(std::move(*left));
                ++left;
            }
        }

        while (left != mid) {
            temp.push_back(std::move(*left));
            ++left;
        }
        while (right != last) {
            temp.push_back(std::move(*right));
            ++right;
        }

        std::move(temp.begin(), temp.end(), first);
    }

    template <typename Container, typename Compare = std::less<typename Container::value_type>>
    static void mergeSort(Container& c, Compare comp = Compare()) {
        mergeSort(std::begin(c), std::end(c), comp);
    }

    // -------------------------------------------------------------
    // 3. Quick Sort (O(n log n) avg, Median-of-3, O(log n) stack)
    // -------------------------------------------------------------
    template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
    static void quickSort(RandomIt first, RandomIt last, Compare comp = Compare()) {
        while (std::distance(first, last) > 1) {
            // Cut-off to insertion sort for small subarrays (hybrid quicksort optimization)
            if (std::distance(first, last) <= 16) {
                insertionSort(first, last, comp);
                break;
            }

            RandomIt pivot = partition(first, last, comp);

            // Tail-call optimization: recurse into smaller partition, loop on larger
            if (std::distance(first, pivot) < std::distance(pivot + 1, last)) {
                quickSort(first, pivot, comp);
                first = pivot + 1;
            } else {
                quickSort(pivot + 1, last, comp);
                last = pivot;
            }
        }
    }

    template <typename Container, typename Compare = std::less<typename Container::value_type>>
    static void quickSort(Container& c, Compare comp = Compare()) {
        quickSort(std::begin(c), std::end(c), comp);
    }

private:
    template <typename RandomIt, typename Compare>
    static RandomIt medianOfThree(RandomIt a, RandomIt b, RandomIt c, Compare& comp) {
        if (comp(*b, *a)) std::iter_swap(a, b);
        if (comp(*c, *b)) std::iter_swap(b, c);
        if (comp(*b, *a)) std::iter_swap(a, b);
        return b;
    }

    template <typename RandomIt, typename Compare>
    static RandomIt partition(RandomIt first, RandomIt last, Compare& comp) {
        RandomIt mid = first + std::distance(first, last) / 2;
        RandomIt pivotIt = medianOfThree(first, mid, last - 1, comp);
        std::iter_swap(pivotIt, last - 1); // place pivot at end

        auto& pivotVal = *(last - 1);
        RandomIt i = first;

        for (RandomIt j = first; j != last - 1; ++j) {
            if (comp(*j, pivotVal)) {
                std::iter_swap(i, j);
                ++i;
            }
        }
        std::iter_swap(i, last - 1);
        return i;
    }

public:
    // -------------------------------------------------------------
    // 4. Heap Sort (O(n log n) worst/avg, O(1) auxiliary space)
    // -------------------------------------------------------------
    template <typename RandomIt, typename Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
    static void heapSort(RandomIt first, RandomIt last, Compare comp = Compare()) {
        auto n = std::distance(first, last);
        if (n <= 1) return;

        // Bottom-up build heap: O(n)
        for (auto i = n / 2 - 1; i >= 0; --i) {
            siftDown(first, n, i, comp);
        }

        // Repeatedly extract max: O(n log n)
        for (auto i = n - 1; i > 0; --i) {
            std::iter_swap(first, first + i);
            siftDown(first, i, 0, comp);
        }
    }

    template <typename Container, typename Compare = std::less<typename Container::value_type>>
    static void heapSort(Container& c, Compare comp = Compare()) {
        heapSort(std::begin(c), std::end(c), comp);
    }

private:
    template <typename RandomIt, typename Compare>
    static void siftDown(RandomIt first, std::ptrdiff_t n, std::ptrdiff_t root, Compare& comp) {
        while (2 * root + 1 < n) {
            std::ptrdiff_t best = root;
            std::ptrdiff_t left = 2 * root + 1;
            std::ptrdiff_t right = 2 * root + 2;

            if (left < n && comp(*(first + best), *(first + left))) {
                best = left;
            }
            if (right < n && comp(*(first + best), *(first + right))) {
                best = right;
            }

            if (best != root) {
                std::iter_swap(first + root, first + best);
                root = best;
            } else {
                break;
            }
        }
    }
};

} // namespace dsa

#endif // DSA_ALGORITHMS_SORTING_HPP

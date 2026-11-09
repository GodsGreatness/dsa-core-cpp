#include "dsa/linear/LinkedList.hpp"
#include "dsa/linear/Vector.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <vector>

int main() {
    std::cout << "========================================================================\n";
    std::cout << "  Empirical Scaling Benchmark: Vector vs LinkedList Insertion & Access\n";
    std::cout << "========================================================================\n\n";

    std::vector<std::size_t> scales = {1000, 10000, 50000, 100000};

    std::cout << "+------------+-----------------------+-----------------------+-----------------------+\n";
    std::cout << "| Scale (N)  | Vector PushBack (us)  | List PushBack (us)    | Vector Access Time(us)|\n";
    std::cout << "+------------+-----------------------+-----------------------+-----------------------+\n";

    for (std::size_t n : scales) {
        // 1. Vector PushBack
        auto t0 = std::chrono::high_resolution_clock::now();
        dsa::Vector<int> vec;
        for (std::size_t i = 0; i < n; ++i) {
            vec.push_back(static_cast<int>(i));
        }
        auto t1 = std::chrono::high_resolution_clock::now();
        double vecPushTime = std::chrono::duration<double, std::micro>(t1 - t0).count();

        // 2. LinkedList PushBack
        auto t2 = std::chrono::high_resolution_clock::now();
        dsa::LinkedList<int> list;
        for (std::size_t i = 0; i < n; ++i) {
            list.push_back(static_cast<int>(i));
        }
        auto t3 = std::chrono::high_resolution_clock::now();
        double listPushTime = std::chrono::duration<double, std::micro>(t3 - t2).count();

        // 3. Vector Random Access (10,000 accesses)
        auto t4 = std::chrono::high_resolution_clock::now();
        volatile long long sum = 0;
        for (std::size_t i = 0; i < n; ++i) {
            sum += vec[i];
        }
        auto t5 = std::chrono::high_resolution_clock::now();
        double vecAccessTime = std::chrono::duration<double, std::micro>(t5 - t4).count();

        std::cout << "| " << std::setw(10) << n << " | "
                  << std::setw(18) << std::fixed << std::setprecision(2) << vecPushTime << " us | "
                  << std::setw(18) << std::fixed << std::setprecision(2) << listPushTime << " us | "
                  << std::setw(18) << std::fixed << std::setprecision(2) << vecAccessTime << " us |\n";
    }

    std::cout << "+------------+-----------------------+-----------------------+-----------------------+\n\n";
    return 0;
}

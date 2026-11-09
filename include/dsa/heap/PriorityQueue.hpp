#ifndef DSA_HEAP_PRIORITY_QUEUE_HPP
#define DSA_HEAP_PRIORITY_QUEUE_HPP

#include "dsa/linear/Vector.hpp"
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace dsa {

template <typename T, typename Compare = std::less<T>>
class PriorityQueue {
private:
    Vector<T> m_heap;
    Compare m_comp{};

    static std::size_t parent(std::size_t i) noexcept {
        return (i - 1) / 2;
    }

    static std::size_t leftChild(std::size_t i) noexcept {
        return 2 * i + 1;
    }

    static std::size_t rightChild(std::size_t i) noexcept {
        return 2 * i + 2;
    }

    void siftUp(std::size_t index) {
        while (index > 0) {
            std::size_t p = parent(index);
            if (m_comp(m_heap[p], m_heap[index])) {
                using std::swap;
                swap(m_heap[p], m_heap[index]);
                index = p;
            } else {
                break;
            }
        }
    }

    void siftDown(std::size_t index) {
        std::size_t size = m_heap.size();
        while (leftChild(index) < size) {
            std::size_t best = index;
            std::size_t left = leftChild(index);
            std::size_t right = rightChild(index);

            if (left < size && m_comp(m_heap[best], m_heap[left])) {
                best = left;
            }
            if (right < size && m_comp(m_heap[best], m_heap[right])) {
                best = right;
            }

            if (best != index) {
                using std::swap;
                swap(m_heap[index], m_heap[best]);
                index = best;
            } else {
                break;
            }
        }
    }

    // Floyd's linear-time O(n) bottom-up heapify algorithm
    void heapify() {
        if (m_heap.size() <= 1) return;
        for (std::ptrdiff_t i = static_cast<std::ptrdiff_t>((m_heap.size() / 2) - 1); i >= 0; --i) {
            siftDown(static_cast<std::size_t>(i));
        }
    }

public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;

    // -------------------------------------------------------------
    // Constructors & Rule of Five
    // -------------------------------------------------------------

    PriorityQueue() = default;

    explicit PriorityQueue(const Compare& comp) : m_comp(comp) {}

    // Linear-time O(n) construction from initializer list
    PriorityQueue(std::initializer_list<T> init, const Compare& comp = Compare())
        : m_comp(comp) {
        for (const auto& item : init) {
            m_heap.push_back(item);
        }
        heapify();
    }

    // Linear-time O(n) construction from existing vector
    explicit PriorityQueue(const Vector<T>& elements, const Compare& comp = Compare())
        : m_heap(elements), m_comp(comp) {
        heapify();
    }

    explicit PriorityQueue(Vector<T>&& elements, const Compare& comp = Compare())
        : m_heap(std::move(elements)), m_comp(comp) {
        heapify();
    }

    // -------------------------------------------------------------
    // Capacity
    // -------------------------------------------------------------

    [[nodiscard]] bool empty() const noexcept { return m_heap.empty(); }
    [[nodiscard]] size_type size() const noexcept { return m_heap.size(); }

    // -------------------------------------------------------------
    // Element Access
    // -------------------------------------------------------------

    const_reference top() const {
        if (empty()) {
            throw std::underflow_error("PriorityQueue::top: queue is empty");
        }
        return m_heap.front();
    }

    // -------------------------------------------------------------
    // Modifiers
    // -------------------------------------------------------------

    void push(const T& value) {
        m_heap.push_back(value);
        siftUp(m_heap.size() - 1);
    }

    void push(T&& value) {
        m_heap.push_back(std::move(value));
        siftUp(m_heap.size() - 1);
    }

    template <typename... Args>
    void emplace(Args&&... args) {
        m_heap.emplace_back(std::forward<Args>(args)...);
        siftUp(m_heap.size() - 1);
    }

    void pop() {
        if (empty()) {
            throw std::underflow_error("PriorityQueue::pop: queue is empty");
        }
        using std::swap;
        swap(m_heap.front(), m_heap.back());
        m_heap.pop_back();
        if (!empty()) {
            siftDown(0);
        }
    }

    void clear() noexcept {
        m_heap.clear();
    }
};

} // namespace dsa

#endif // DSA_HEAP_PRIORITY_QUEUE_HPP

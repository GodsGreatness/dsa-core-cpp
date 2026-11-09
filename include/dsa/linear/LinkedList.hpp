#ifndef DSA_LINEAR_LINKED_LIST_HPP
#define DSA_LINEAR_LINKED_LIST_HPP

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace dsa {

template <typename T>
class LinkedList {
private:
    // Base node struct without data payload to allow sentinel nodes
    // without requiring T to be default-constructible.
    struct NodeBase {
        NodeBase* prev{nullptr};
        NodeBase* next{nullptr};
    };

    struct Node : public NodeBase {
        T data;

        template <typename... Args>
        explicit Node(Args&&... args) : data(std::forward<Args>(args)...) {}
    };

    NodeBase m_head; // Sentinel head
    NodeBase m_tail; // Sentinel tail
    std::size_t m_size{0};

    void initSentinels() noexcept {
        m_head.next = &m_tail;
        m_head.prev = nullptr;
        m_tail.prev = &m_head;
        m_tail.next = nullptr;
        m_size = 0;
    }

    Node* toNode(NodeBase* base) noexcept {
        return static_cast<Node*>(base);
    }

    const Node* toNode(const NodeBase* base) const noexcept {
        return static_cast<const Node*>(base);
    }

public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = T&;
    using const_reference = const T&;

    // -------------------------------------------------------------
    // Bidirectional Iterators
    // -------------------------------------------------------------
    class Iterator {
        friend class LinkedList;

    private:
        NodeBase* m_current{nullptr};

        explicit Iterator(NodeBase* node) noexcept : m_current(node) {}

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator() noexcept = default;

        reference operator*() const {
            return static_cast<Node*>(m_current)->data;
        }

        pointer operator->() const {
            return &(static_cast<Node*>(m_current)->data);
        }

        Iterator& operator++() noexcept {
            m_current = m_current->next;
            return *this;
        }

        Iterator operator++(int) noexcept {
            Iterator tmp = *this;
            m_current = m_current->next;
            return tmp;
        }

        Iterator& operator--() noexcept {
            m_current = m_current->prev;
            return *this;
        }

        Iterator operator--(int) noexcept {
            Iterator tmp = *this;
            m_current = m_current->prev;
            return tmp;
        }

        bool operator==(const Iterator& other) const noexcept {
            return m_current == other.m_current;
        }

        bool operator!=(const Iterator& other) const noexcept {
            return m_current != other.m_current;
        }
    };

    class ConstIterator {
        friend class LinkedList;

    private:
        const NodeBase* m_current{nullptr};

        explicit ConstIterator(const NodeBase* node) noexcept : m_current(node) {}

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        ConstIterator() noexcept = default;
        ConstIterator(const Iterator& it) noexcept : m_current(it.m_current) {}

        reference operator*() const {
            return static_cast<const Node*>(m_current)->data;
        }

        pointer operator->() const {
            return &(static_cast<const Node*>(m_current)->data);
        }

        ConstIterator& operator++() noexcept {
            m_current = m_current->next;
            return *this;
        }

        ConstIterator operator++(int) noexcept {
            ConstIterator tmp = *this;
            m_current = m_current->next;
            return tmp;
        }

        ConstIterator& operator--() noexcept {
            m_current = m_current->prev;
            return *this;
        }

        ConstIterator operator--(int) noexcept {
            ConstIterator tmp = *this;
            m_current = m_current->prev;
            return tmp;
        }

        bool operator==(const ConstIterator& other) const noexcept {
            return m_current == other.m_current;
        }

        bool operator!=(const ConstIterator& other) const noexcept {
            return m_current != other.m_current;
        }
    };

    using iterator = Iterator;
    using const_iterator = ConstIterator;

    // -------------------------------------------------------------
    // Constructors & Rule of Five
    // -------------------------------------------------------------

    LinkedList() noexcept {
        initSentinels();
    }

    LinkedList(std::initializer_list<T> init) {
        initSentinels();
        try {
            for (const auto& item : init) {
                push_back(item);
            }
        } catch (...) {
            clear();
            throw;
        }
    }

    // Copy Constructor
    LinkedList(const LinkedList& other) {
        initSentinels();
        try {
            for (const auto& item : other) {
                push_back(item);
            }
        } catch (...) {
            clear();
            throw;
        }
    }

    // Move Constructor
    LinkedList(LinkedList&& other) noexcept {
        initSentinels();
        if (!other.empty()) {
            m_head.next = other.m_head.next;
            other.m_head.next->prev = &m_head;

            m_tail.prev = other.m_tail.prev;
            other.m_tail.prev->next = &m_tail;

            m_size = other.m_size;

            other.initSentinels();
        }
    }

    // Destructor
    ~LinkedList() {
        clear();
    }

    // Copy Assignment
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            LinkedList temp(other);
            swap(temp);
        }
        return *this;
    }

    // Move Assignment
    LinkedList& operator=(LinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            if (!other.empty()) {
                m_head.next = other.m_head.next;
                other.m_head.next->prev = &m_head;

                m_tail.prev = other.m_tail.prev;
                other.m_tail.prev->next = &m_tail;

                m_size = other.m_size;

                other.initSentinels();
            }
        }
        return *this;
    }

    void swap(LinkedList& other) noexcept {
        if (this == &other) return;

        bool thisEmpty = empty();
        bool otherEmpty = other.empty();

        if (thisEmpty && otherEmpty) return;

        if (thisEmpty && !otherEmpty) {
            m_head.next = other.m_head.next;
            m_head.next->prev = &m_head;
            m_tail.prev = other.m_tail.prev;
            m_tail.prev->next = &m_tail;
            m_size = other.m_size;
            other.initSentinels();
        } else if (!thisEmpty && otherEmpty) {
            other.m_head.next = m_head.next;
            other.m_head.next->prev = &other.m_head;
            other.m_tail.prev = m_tail.prev;
            other.m_tail.prev->next = &other.m_tail;
            other.m_size = m_size;
            initSentinels();
        } else {
            NodeBase* thisFirst = m_head.next;
            NodeBase* thisLast = m_tail.prev;
            NodeBase* otherFirst = other.m_head.next;
            NodeBase* otherLast = other.m_tail.prev;

            m_head.next = otherFirst;
            otherFirst->prev = &m_head;
            m_tail.prev = otherLast;
            otherLast->next = &m_tail;

            other.m_head.next = thisFirst;
            thisFirst->prev = &other.m_head;
            other.m_tail.prev = thisLast;
            thisLast->next = &other.m_tail;

            std::swap(m_size, other.m_size);
        }
    }

    // -------------------------------------------------------------
    // Capacity
    // -------------------------------------------------------------

    [[nodiscard]] bool empty() const noexcept { return m_size == 0; }
    [[nodiscard]] size_type size() const noexcept { return m_size; }

    // -------------------------------------------------------------
    // Element Access
    // -------------------------------------------------------------

    reference front() {
        if (empty()) throw std::out_of_range("LinkedList::front: list is empty");
        return toNode(m_head.next)->data;
    }

    const_reference front() const {
        if (empty()) throw std::out_of_range("LinkedList::front: list is empty");
        return toNode(m_head.next)->data;
    }

    reference back() {
        if (empty()) throw std::out_of_range("LinkedList::back: list is empty");
        return toNode(m_tail.prev)->data;
    }

    const_reference back() const {
        if (empty()) throw std::out_of_range("LinkedList::back: list is empty");
        return toNode(m_tail.prev)->data;
    }

    // -------------------------------------------------------------
    // Iterators
    // -------------------------------------------------------------

    iterator begin() noexcept { return iterator(m_head.next); }
    iterator end() noexcept { return iterator(&m_tail); }
    const_iterator begin() const noexcept { return const_iterator(m_head.next); }
    const_iterator end() const noexcept { return const_iterator(&m_tail); }
    const_iterator cbegin() const noexcept { return const_iterator(m_head.next); }
    const_iterator cend() const noexcept { return const_iterator(&m_tail); }

    // -------------------------------------------------------------
    // Modifiers
    // -------------------------------------------------------------

    void clear() noexcept {
        NodeBase* curr = m_head.next;
        while (curr != &m_tail) {
            NodeBase* next = curr->next;
            delete toNode(curr);
            curr = next;
        }
        initSentinels();
    }

    void push_back(const T& value) {
        emplace_back(value);
    }

    void push_back(T&& value) {
        emplace_back(std::move(value));
    }

    template <typename... Args>
    reference emplace_back(Args&&... args) {
        return *emplace(end(), std::forward<Args>(args)...);
    }

    void push_front(const T& value) {
        emplace_front(value);
    }

    void push_front(T&& value) {
        emplace_front(std::move(value));
    }

    template <typename... Args>
    reference emplace_front(Args&&... args) {
        return *emplace(begin(), std::forward<Args>(args)...);
    }

    void pop_back() {
        if (empty()) throw std::out_of_range("LinkedList::pop_back: list is empty");
        erase(iterator(m_tail.prev));
    }

    void pop_front() {
        if (empty()) throw std::out_of_range("LinkedList::pop_front: list is empty");
        erase(begin());
    }

    template <typename... Args>
    iterator emplace(const_iterator pos, Args&&... args) {
        NodeBase* curr = const_cast<NodeBase*>(pos.m_current);
        NodeBase* prevNode = curr->prev;

        Node* newNode = new Node(std::forward<Args>(args)...);
        newNode->prev = prevNode;
        newNode->next = curr;

        prevNode->next = newNode;
        curr->prev = newNode;

        ++m_size;
        return iterator(newNode);
    }

    iterator insert(const_iterator pos, const T& value) {
        return emplace(pos, value);
    }

    iterator insert(const_iterator pos, T&& value) {
        return emplace(pos, std::move(value));
    }

    iterator erase(const_iterator pos) {
        if (pos == end()) {
            throw std::out_of_range("LinkedList::erase: cannot erase end iterator");
        }

        NodeBase* target = const_cast<NodeBase*>(pos.m_current);
        NodeBase* prevNode = target->prev;
        NodeBase* nextNode = target->next;

        prevNode->next = nextNode;
        nextNode->prev = prevNode;

        delete toNode(target);
        --m_size;

        return iterator(nextNode);
    }
};

template <typename T>
void swap(LinkedList<T>& a, LinkedList<T>& b) noexcept {
    a.swap(b);
}

} // namespace dsa

#endif // DSA_LINEAR_LINKED_LIST_HPP

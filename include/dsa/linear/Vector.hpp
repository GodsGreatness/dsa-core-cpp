#ifndef DSA_LINEAR_VECTOR_HPP
#define DSA_LINEAR_VECTOR_HPP

#include <cstddef>
#include <initializer_list>
#include <memory>
#include <new>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace dsa {

template <typename T>
class Vector {
public:
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = T*;
    using const_iterator = const T*;

private:
    pointer m_data{nullptr};
    size_type m_size{0};
    size_type m_capacity{0};

    // Allocate uninitialized storage for n elements
    static pointer allocateRaw(size_type n) {
        if (n == 0) return nullptr;
        return static_cast<pointer>(::operator new(n * sizeof(T)));
    }

    // Deallocate raw storage
    static void deallocateRaw(pointer ptr) noexcept {
        ::operator delete(static_cast<void*>(ptr));
    }

    // Destroy a range of constructed elements
    static void destroyRange(pointer first, pointer last) noexcept {
        for (pointer p = first; p != last; ++p) {
            p->~T();
        }
    }

public:
    // -------------------------------------------------------------
    // Constructors & Rule of Five
    // -------------------------------------------------------------

    // Default constructor
    Vector() noexcept = default;

    // Sized constructor with fill value
    explicit Vector(size_type count, const T& value = T()) {
        if (count > 0) {
            m_data = allocateRaw(count);
            m_capacity = count;
            size_type constructed = 0;
            try {
                for (; constructed < count; ++constructed) {
                    new (static_cast<void*>(m_data + constructed)) T(value);
                }
                m_size = count;
            } catch (...) {
                destroyRange(m_data, m_data + constructed);
                deallocateRaw(m_data);
                throw;
            }
        }
    }

    // Initializer list constructor
    Vector(std::initializer_list<T> init) {
        if (init.size() > 0) {
            m_data = allocateRaw(init.size());
            m_capacity = init.size();
            size_type constructed = 0;
            try {
                for (const auto& item : init) {
                    new (static_cast<void*>(m_data + constructed)) T(item);
                    ++constructed;
                }
                m_size = init.size();
            } catch (...) {
                destroyRange(m_data, m_data + constructed);
                deallocateRaw(m_data);
                throw;
            }
        }
    }

    // Copy constructor (deep copy)
    Vector(const Vector& other) {
        if (other.m_size > 0) {
            m_data = allocateRaw(other.m_size);
            m_capacity = other.m_size;
            size_type constructed = 0;
            try {
                for (; constructed < other.m_size; ++constructed) {
                    new (static_cast<void*>(m_data + constructed)) T(other.m_data[constructed]);
                }
                m_size = other.m_size;
            } catch (...) {
                destroyRange(m_data, m_data + constructed);
                deallocateRaw(m_data);
                throw;
            }
        }
    }

    // Move constructor (transfer ownership, zero out other)
    Vector(Vector&& other) noexcept
        : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity) {
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    // Destructor (destroy all elements, release raw storage)
    ~Vector() {
        clear();
        deallocateRaw(m_data);
    }

    // Copy assignment (Strong exception safety via copy-and-swap)
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector temp(other);
            swap(temp);
        }
        return *this;
    }

    // Move assignment
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            clear();
            deallocateRaw(m_data);

            m_data = other.m_data;
            m_size = other.m_size;
            m_capacity = other.m_capacity;

            other.m_data = nullptr;
            other.m_size = 0;
            other.m_capacity = 0;
        }
        return *this;
    }

    void swap(Vector& other) noexcept {
        using std::swap;
        swap(m_data, other.m_data);
        swap(m_size, other.m_size);
        swap(m_capacity, other.m_capacity);
    }

    // -------------------------------------------------------------
    // Capacity & Allocation Management
    // -------------------------------------------------------------

    [[nodiscard]] bool empty() const noexcept { return m_size == 0; }
    [[nodiscard]] size_type size() const noexcept { return m_size; }
    [[nodiscard]] size_type capacity() const noexcept { return m_capacity; }

    void reserve(size_type new_cap) {
        if (new_cap <= m_capacity) return;

        pointer new_data = allocateRaw(new_cap);
        size_type constructed = 0;

        try {
            for (; constructed < m_size; ++constructed) {
                if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
                    new (static_cast<void*>(new_data + constructed)) T(std::move(m_data[constructed]));
                } else {
                    new (static_cast<void*>(new_data + constructed)) T(m_data[constructed]);
                }
            }
        } catch (...) {
            destroyRange(new_data, new_data + constructed);
            deallocateRaw(new_data);
            throw;
        }

        destroyRange(m_data, m_data + m_size);
        deallocateRaw(m_data);

        m_data = new_data;
        m_capacity = new_cap;
    }

    void shrink_to_fit() {
        if (m_capacity > m_size) {
            if (m_size == 0) {
                deallocateRaw(m_data);
                m_data = nullptr;
                m_capacity = 0;
                return;
            }

            pointer new_data = allocateRaw(m_size);
            size_type constructed = 0;
            try {
                for (; constructed < m_size; ++constructed) {
                    new (static_cast<void*>(new_data + constructed)) T(std::move_if_noexcept(m_data[constructed]));
                }
            } catch (...) {
                destroyRange(new_data, new_data + constructed);
                deallocateRaw(new_data);
                throw;
            }

            destroyRange(m_data, m_data + m_size);
            deallocateRaw(m_data);

            m_data = new_data;
            m_capacity = m_size;
        }
    }

    // -------------------------------------------------------------
    // Element Access
    // -------------------------------------------------------------

    reference operator[](size_type index) noexcept { return m_data[index]; }
    const_reference operator[](size_type index) const noexcept { return m_data[index]; }

    reference at(size_type index) {
        if (index >= m_size) {
            throw std::out_of_range("Vector::at: index out of bounds");
        }
        return m_data[index];
    }

    const_reference at(size_type index) const {
        if (index >= m_size) {
            throw std::out_of_range("Vector::at: index out of bounds");
        }
        return m_data[index];
    }

    reference front() {
        if (empty()) throw std::out_of_range("Vector::front: vector is empty");
        return m_data[0];
    }

    const_reference front() const {
        if (empty()) throw std::out_of_range("Vector::front: vector is empty");
        return m_data[0];
    }

    reference back() {
        if (empty()) throw std::out_of_range("Vector::back: vector is empty");
        return m_data[m_size - 1];
    }

    const_reference back() const {
        if (empty()) throw std::out_of_range("Vector::back: vector is empty");
        return m_data[m_size - 1];
    }

    pointer data() noexcept { return m_data; }
    const_pointer data() const noexcept { return m_data; }

    // -------------------------------------------------------------
    // Iterators
    // -------------------------------------------------------------

    iterator begin() noexcept { return m_data; }
    iterator end() noexcept { return m_data + m_size; }
    const_iterator begin() const noexcept { return m_data; }
    const_iterator end() const noexcept { return m_data + m_size; }
    const_iterator cbegin() const noexcept { return m_data; }
    const_iterator cend() const noexcept { return m_data + m_size; }

    // -------------------------------------------------------------
    // Modifiers
    // -------------------------------------------------------------

    void clear() noexcept {
        destroyRange(m_data, m_data + m_size);
        m_size = 0;
    }

    void push_back(const T& value) {
        emplace_back(value);
    }

    void push_back(T&& value) {
        emplace_back(std::move(value));
    }

    template <typename... Args>
    reference emplace_back(Args&&... args) {
        if (m_size == m_capacity) {
            size_type next_cap = (m_capacity == 0) ? 1 : m_capacity * 2;
            reserve(next_cap);
        }
        pointer target = m_data + m_size;
        new (static_cast<void*>(target)) T(std::forward<Args>(args)...);
        ++m_size;
        return *target;
    }

    void pop_back() {
        if (empty()) throw std::out_of_range("Vector::pop_back: vector is empty");
        --m_size;
        m_data[m_size].~T();
    }

    void resize(size_type count, const T& value = T()) {
        if (count < m_size) {
            destroyRange(m_data + count, m_data + m_size);
            m_size = count;
        } else if (count > m_size) {
            if (count > m_capacity) {
                reserve(count);
            }
            while (m_size < count) {
                new (static_cast<void*>(m_data + m_size)) T(value);
                ++m_size;
            }
        }
    }
};

template <typename T>
void swap(Vector<T>& a, Vector<T>& b) noexcept {
    a.swap(b);
}

} // namespace dsa

#endif // DSA_LINEAR_VECTOR_HPP

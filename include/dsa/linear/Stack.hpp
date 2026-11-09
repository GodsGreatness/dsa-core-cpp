#ifndef DSA_LINEAR_STACK_HPP
#define DSA_LINEAR_STACK_HPP

#include "Vector.hpp"
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace dsa {

template <typename T, typename Container = Vector<T>>
class Stack {
private:
    Container m_container;

public:
    using value_type = typename Container::value_type;
    using size_type = typename Container::size_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;
    using container_type = Container;

    // -------------------------------------------------------------
    // Constructors & Rule of Five
    // -------------------------------------------------------------

    Stack() = default;
    explicit Stack(const Container& cont) : m_container(cont) {}
    explicit Stack(Container&& cont) : m_container(std::move(cont)) {}

    // -------------------------------------------------------------
    // Capacity
    // -------------------------------------------------------------

    [[nodiscard]] bool empty() const noexcept { return m_container.empty(); }
    [[nodiscard]] size_type size() const noexcept { return m_container.size(); }

    // -------------------------------------------------------------
    // Element Access
    // -------------------------------------------------------------

    reference top() {
        if (empty()) {
            throw std::underflow_error("Stack::top: stack is empty");
        }
        return m_container.back();
    }

    const_reference top() const {
        if (empty()) {
            throw std::underflow_error("Stack::top: stack is empty");
        }
        return m_container.back();
    }

    // -------------------------------------------------------------
    // Modifiers
    // -------------------------------------------------------------

    void push(const T& value) {
        m_container.push_back(value);
    }

    void push(T&& value) {
        m_container.push_back(std::move(value));
    }

    template <typename... Args>
    decltype(auto) emplace(Args&&... args) {
        return m_container.emplace_back(std::forward<Args>(args)...);
    }

    void pop() {
        if (empty()) {
            throw std::underflow_error("Stack::pop: stack is empty");
        }
        m_container.pop_back();
    }

    void clear() noexcept {
        m_container.clear();
    }
};

} // namespace dsa

#endif // DSA_LINEAR_STACK_HPP

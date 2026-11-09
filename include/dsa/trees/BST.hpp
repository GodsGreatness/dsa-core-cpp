#ifndef DSA_TREES_BST_HPP
#define DSA_TREES_BST_HPP

#include <algorithm>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <memory>
#include <queue>
#include <stdexcept>
#include <utility>
#include <vector>

namespace dsa {

template <typename T, typename Compare = std::less<T>>
class BST {
private:
    struct Node {
        T data;
        std::unique_ptr<Node> left{nullptr};
        std::unique_ptr<Node> right{nullptr};

        template <typename... Args>
        explicit Node(Args&&... args) : data(std::forward<Args>(args)...) {}
    };

    std::unique_ptr<Node> m_root{nullptr};
    std::size_t m_size{0};
    Compare m_comp{};

    // Helper: deep copy of subtree
    std::unique_ptr<Node> cloneSubtree(const Node* node) const {
        if (!node) return nullptr;
        auto newNode = std::make_unique<Node>(node->data);
        newNode->left = cloneSubtree(node->left.get());
        newNode->right = cloneSubtree(node->right.get());
        return newNode;
    }

    // Helper: calculate height
    std::size_t calculateHeight(const Node* node) const {
        if (!node) return 0;
        return 1 + std::max(calculateHeight(node->left.get()), calculateHeight(node->right.get()));
    }

    // Helper: recursive insert
    bool insertRec(std::unique_ptr<Node>& node, const T& value) {
        if (!node) {
            node = std::make_unique<Node>(value);
            ++m_size;
            return true;
        }

        if (m_comp(value, node->data)) {
            return insertRec(node->left, value);
        } else if (m_comp(node->data, value)) {
            return insertRec(node->right, value);
        }
        return false; // Duplicate key
    }

    bool insertRec(std::unique_ptr<Node>& node, T&& value) {
        if (!node) {
            node = std::make_unique<Node>(std::move(value));
            ++m_size;
            return true;
        }

        if (m_comp(value, node->data)) {
            return insertRec(node->left, std::move(value));
        } else if (m_comp(node->data, value)) {
            return insertRec(node->right, std::move(value));
        }
        return false;
    }

    // Helper: recursive search
    const Node* findRec(const Node* node, const T& value) const {
        if (!node) return nullptr;
        if (m_comp(value, node->data)) {
            return findRec(node->left.get(), value);
        } else if (m_comp(node->data, value)) {
            return findRec(node->right.get(), value);
        }
        return node;
    }

    // Helper: find minimum node in a subtree
    Node* findMin(Node* node) const {
        while (node && node->left) {
            node = node->left.get();
        }
        return node;
    }

    // Helper: recursive remove
    bool removeRec(std::unique_ptr<Node>& node, const T& value) {
        if (!node) return false;

        if (m_comp(value, node->data)) {
            return removeRec(node->left, value);
        } else if (m_comp(node->data, value)) {
            return removeRec(node->right, value);
        } else {
            // Node found. 3 Cases:
            // Case 1 & 2: 0 or 1 child
            if (!node->left) {
                node = std::move(node->right);
            } else if (!node->right) {
                node = std::move(node->left);
            } else {
                // Case 3: 2 children. Find in-order successor (min in right subtree)
                Node* successor = findMin(node->right.get());
                node->data = successor->data;
                removeRec(node->right, successor->data);
                return true;
            }
            --m_size;
            return true;
        }
    }

    template <typename Func>
    void inorderRec(const Node* node, Func&& func) const {
        if (!node) return;
        inorderRec(node->left.get(), func);
        func(node->data);
        inorderRec(node->right.get(), func);
    }

    template <typename Func>
    void preorderRec(const Node* node, Func&& func) const {
        if (!node) return;
        func(node->data);
        preorderRec(node->left.get(), func);
        preorderRec(node->right.get(), func);
    }

    template <typename Func>
    void postorderRec(const Node* node, Func&& func) const {
        if (!node) return;
        postorderRec(node->left.get(), func);
        postorderRec(node->right.get(), func);
        func(node->data);
    }

public:
    using value_type = T;
    using size_type = std::size_t;

    // -------------------------------------------------------------
    // Constructors & Rule of Five
    // -------------------------------------------------------------

    BST() noexcept = default;

    BST(std::initializer_list<T> init) {
        for (const auto& item : init) {
            insert(item);
        }
    }

    // Copy Constructor (Deep clone)
    BST(const BST& other) : m_size(other.m_size), m_comp(other.m_comp) {
        m_root = cloneSubtree(other.m_root.get());
    }

    // Move Constructor
    BST(BST&& other) noexcept
        : m_root(std::move(other.m_root)), m_size(other.m_size), m_comp(other.m_comp) {
        other.m_size = 0;
    }

    // Destructor (automatic recursive cleanup via unique_ptr)
    ~BST() = default;

    // Copy Assignment
    BST& operator=(const BST& other) {
        if (this != &other) {
            BST temp(other);
            swap(temp);
        }
        return *this;
    }

    // Move Assignment
    BST& operator=(BST&& other) noexcept {
        if (this != &other) {
            m_root = std::move(other.m_root);
            m_size = other.m_size;
            m_comp = other.m_comp;
            other.m_size = 0;
        }
        return *this;
    }

    void swap(BST& other) noexcept {
        using std::swap;
        swap(m_root, other.m_root);
        swap(m_size, other.m_size);
        swap(m_comp, other.m_comp);
    }

    // -------------------------------------------------------------
    // Capacity & Status
    // -------------------------------------------------------------

    [[nodiscard]] bool empty() const noexcept { return m_size == 0; }
    [[nodiscard]] size_type size() const noexcept { return m_size; }
    [[nodiscard]] size_type height() const { return calculateHeight(m_root.get()); }

    void clear() noexcept {
        m_root.reset();
        m_size = 0;
    }

    // -------------------------------------------------------------
    // Modifiers & Operations
    // -------------------------------------------------------------

    bool insert(const T& value) {
        return insertRec(m_root, value);
    }

    bool insert(T&& value) {
        return insertRec(m_root, std::move(value));
    }

    bool remove(const T& value) {
        return removeRec(m_root, value);
    }

    [[nodiscard]] bool contains(const T& value) const {
        return findRec(m_root.get(), value) != nullptr;
    }

    const T& min() const {
        if (empty()) throw std::runtime_error("BST::min: tree is empty");
        return findMin(m_root.get())->data;
    }

    const T& max() const {
        if (empty()) throw std::runtime_error("BST::max: tree is empty");
        const Node* curr = m_root.get();
        while (curr->right) {
            curr = curr->right.get();
        }
        return curr->data;
    }

    // -------------------------------------------------------------
    // Traversals
    // -------------------------------------------------------------

    template <typename Func>
    void inorder(Func&& func) const {
        inorderRec(m_root.get(), std::forward<Func>(func));
    }

    template <typename Func>
    void preorder(Func&& func) const {
        preorderRec(m_root.get(), std::forward<Func>(func));
    }

    template <typename Func>
    void postorder(Func&& func) const {
        postorderRec(m_root.get(), std::forward<Func>(func));
    }

    template <typename Func>
    void levelorder(Func&& func) const {
        if (!m_root) return;
        std::queue<const Node*> q;
        q.push(m_root.get());

        while (!q.empty()) {
            const Node* curr = q.front();
            q.pop();
            func(curr->data);
            if (curr->left) q.push(curr->left.get());
            if (curr->right) q.push(curr->right.get());
        }
    }

    [[nodiscard]] std::vector<T> toVector() const {
        std::vector<T> result;
        result.reserve(m_size);
        inorder([&result](const T& val) { result.push_back(val); });
        return result;
    }
};

template <typename T, typename Compare>
void swap(BST<T, Compare>& a, BST<T, Compare>& b) noexcept {
    a.swap(b);
}

} // namespace dsa

#endif // DSA_TREES_BST_HPP

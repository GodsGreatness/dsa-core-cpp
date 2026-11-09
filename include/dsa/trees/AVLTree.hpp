#ifndef DSA_TREES_AVL_TREE_HPP
#define DSA_TREES_AVL_TREE_HPP

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
class AVLTree {
private:
    struct Node {
        T data;
        int height{1};
        std::unique_ptr<Node> left{nullptr};
        std::unique_ptr<Node> right{nullptr};

        template <typename... Args>
        explicit Node(Args&&... args) : data(std::forward<Args>(args)...), height(1) {}
    };

    std::unique_ptr<Node> m_root{nullptr};
    std::size_t m_size{0};
    Compare m_comp{};

    // Helper: Node height getter (0 for nullptr)
    static int getHeight(const Node* node) noexcept {
        return node ? node->height : 0;
    }

    // Helper: Recalculate node height
    static void updateHeight(Node* node) noexcept {
        if (node) {
            node->height = 1 + std::max(getHeight(node->left.get()), getHeight(node->right.get()));
        }
    }

    // Helper: Compute balance factor: height(left) - height(right)
    static int getBalanceFactor(const Node* node) noexcept {
        return node ? getHeight(node->left.get()) - getHeight(node->right.get()) : 0;
    }

    // -------------------------------------------------------------
    // Rotation Primitives (All maintain strict pointer ownership)
    // -------------------------------------------------------------

    // LL Case: Single Right Rotation
    std::unique_ptr<Node> rotateRight(std::unique_ptr<Node> y) {
        std::unique_ptr<Node> x = std::move(y->left);
        y->left = std::move(x->right);
        updateHeight(y.get());

        x->right = std::move(y);
        updateHeight(x.get());
        return x;
    }

    // RR Case: Single Left Rotation
    std::unique_ptr<Node> rotateLeft(std::unique_ptr<Node> x) {
        std::unique_ptr<Node> y = std::move(x->right);
        x->right = std::move(y->left);
        updateHeight(x.get());

        y->left = std::move(x);
        updateHeight(y.get());
        return y;
    }

    // LR Case: Left Rotation on Left Child, then Right Rotation on Node
    std::unique_ptr<Node> rotateLeftRight(std::unique_ptr<Node> node) {
        node->left = rotateLeft(std::move(node->left));
        return rotateRight(std::move(node));
    }

    // RL Case: Right Rotation on Right Child, then Left Rotation on Node
    std::unique_ptr<Node> rotateRightLeft(std::unique_ptr<Node> node) {
        node->right = rotateRight(std::move(node->right));
        return rotateLeft(std::move(node));
    }

    // Rebalance a node if balance factor invariant is violated (|BF| > 1)
    std::unique_ptr<Node> rebalance(std::unique_ptr<Node> node) {
        if (!node) return nullptr;

        updateHeight(node.get());
        int bf = getBalanceFactor(node.get());

        // Left Heavy
        if (bf > 1) {
            if (getBalanceFactor(node->left.get()) >= 0) {
                return rotateRight(std::move(node)); // LL
            } else {
                return rotateLeftRight(std::move(node)); // LR
            }
        }

        // Right Heavy
        if (bf < -1) {
            if (getBalanceFactor(node->right.get()) <= 0) {
                return rotateLeft(std::move(node)); // RR
            } else {
                return rotateRightLeft(std::move(node)); // RL
            }
        }

        return node;
    }

    // -------------------------------------------------------------
    // Insertion & Deletion Implementation
    // -------------------------------------------------------------

    std::unique_ptr<Node> insertRec(std::unique_ptr<Node> node, const T& value, bool& inserted) {
        if (!node) {
            inserted = true;
            ++m_size;
            return std::make_unique<Node>(value);
        }

        if (m_comp(value, node->data)) {
            node->left = insertRec(std::move(node->left), value, inserted);
        } else if (m_comp(node->data, value)) {
            node->right = insertRec(std::move(node->right), value, inserted);
        } else {
            inserted = false; // Duplicate key
            return node;
        }

        return rebalance(std::move(node));
    }

    std::unique_ptr<Node> insertRec(std::unique_ptr<Node> node, T&& value, bool& inserted) {
        if (!node) {
            inserted = true;
            ++m_size;
            return std::make_unique<Node>(std::move(value));
        }

        if (m_comp(value, node->data)) {
            node->left = insertRec(std::move(node->left), std::move(value), inserted);
        } else if (m_comp(node->data, value)) {
            node->right = insertRec(std::move(node->right), std::move(value), inserted);
        } else {
            inserted = false;
            return node;
        }

        return rebalance(std::move(node));
    }

    Node* findMin(Node* node) const noexcept {
        while (node && node->left) {
            node = node->left.get();
        }
        return node;
    }

    std::unique_ptr<Node> removeRec(std::unique_ptr<Node> node, const T& value, bool& removed) {
        if (!node) {
            removed = false;
            return nullptr;
        }

        if (m_comp(value, node->data)) {
            node->left = removeRec(std::move(node->left), value, removed);
        } else if (m_comp(node->data, value)) {
            node->right = removeRec(std::move(node->right), value, removed);
        } else {
            removed = true;
            --m_size;

            // 0 or 1 child
            if (!node->left) {
                return std::move(node->right);
            } else if (!node->right) {
                return std::move(node->left);
            } else {
                // 2 children: copy successor data, delete successor from right subtree
                Node* successor = findMin(node->right.get());
                node->data = successor->data;
                bool dummy = false;
                ++m_size; // compensate for the recursive call which will decrement size
                node->right = removeRec(std::move(node->right), successor->data, dummy);
            }
        }

        return rebalance(std::move(node));
    }

    std::unique_ptr<Node> cloneSubtree(const Node* node) const {
        if (!node) return nullptr;
        auto newNode = std::make_unique<Node>(node->data);
        newNode->height = node->height;
        newNode->left = cloneSubtree(node->left.get());
        newNode->right = cloneSubtree(node->right.get());
        return newNode;
    }

    template <typename Func>
    void inorderRec(const Node* node, Func&& func) const {
        if (!node) return;
        inorderRec(node->left.get(), func);
        func(node->data);
        inorderRec(node->right.get(), func);
    }

    // Recursive Invariant Validation
    bool checkInvariants(const Node* node, const T* minVal, const T* maxVal) const {
        if (!node) return true;

        // 1. BST property
        if (minVal && !m_comp(*minVal, node->data)) return false;
        if (maxVal && !m_comp(node->data, *maxVal)) return false;

        // 2. Height property
        int expectedHeight = 1 + std::max(getHeight(node->left.get()), getHeight(node->right.get()));
        if (node->height != expectedHeight) return false;

        // 3. AVL Balance Factor property: |BF| <= 1
        int bf = getBalanceFactor(node);
        if (bf < -1 || bf > 1) return false;

        return checkInvariants(node->left.get(), minVal, &(node->data)) &&
               checkInvariants(node->right.get(), &(node->data), maxVal);
    }

public:
    using value_type = T;
    using size_type = std::size_t;

    // -------------------------------------------------------------
    // Constructors & Rule of Five
    // -------------------------------------------------------------

    AVLTree() noexcept = default;

    AVLTree(std::initializer_list<T> init) {
        for (const auto& item : init) {
            insert(item);
        }
    }

    // Copy Constructor
    AVLTree(const AVLTree& other) : m_size(other.m_size), m_comp(other.m_comp) {
        m_root = cloneSubtree(other.m_root.get());
    }

    // Move Constructor
    AVLTree(AVLTree&& other) noexcept
        : m_root(std::move(other.m_root)), m_size(other.m_size), m_comp(other.m_comp) {
        other.m_size = 0;
    }

    // Destructor
    ~AVLTree() = default;

    // Copy Assignment
    AVLTree& operator=(const AVLTree& other) {
        if (this != &other) {
            AVLTree temp(other);
            swap(temp);
        }
        return *this;
    }

    // Move Assignment
    AVLTree& operator=(AVLTree&& other) noexcept {
        if (this != &other) {
            m_root = std::move(other.m_root);
            m_size = other.m_size;
            m_comp = other.m_comp;
            other.m_size = 0;
        }
        return *this;
    }

    void swap(AVLTree& other) noexcept {
        using std::swap;
        swap(m_root, other.m_root);
        swap(m_size, other.m_size);
        swap(m_comp, other.m_comp);
    }

    // -------------------------------------------------------------
    // Capacity & Invariants
    // -------------------------------------------------------------

    [[nodiscard]] bool empty() const noexcept { return m_size == 0; }
    [[nodiscard]] size_type size() const noexcept { return m_size; }
    [[nodiscard]] int height() const noexcept { return getHeight(m_root.get()); }

    void clear() noexcept {
        m_root.reset();
        m_size = 0;
    }

    // Flagship Invariant Validator: Verifies BST ordering, correct height, and |BF| <= 1 recursively
    [[nodiscard]] bool verifyInvariants() const {
        return checkInvariants(m_root.get(), nullptr, nullptr);
    }

    // -------------------------------------------------------------
    // Modifiers & Operations
    // -------------------------------------------------------------

    bool insert(const T& value) {
        bool inserted = false;
        m_root = insertRec(std::move(m_root), value, inserted);
        return inserted;
    }

    bool insert(T&& value) {
        bool inserted = false;
        m_root = insertRec(std::move(m_root), std::move(value), inserted);
        return inserted;
    }

    bool remove(const T& value) {
        bool removed = false;
        m_root = removeRec(std::move(m_root), value, removed);
        return removed;
    }

    [[nodiscard]] bool contains(const T& value) const {
        const Node* curr = m_root.get();
        while (curr) {
            if (m_comp(value, curr->data)) {
                curr = curr->left.get();
            } else if (m_comp(curr->data, value)) {
                curr = curr->right.get();
            } else {
                return true;
            }
        }
        return false;
    }

    const T& min() const {
        if (empty()) throw std::runtime_error("AVLTree::min: tree is empty");
        return findMin(m_root.get())->data;
    }

    const T& max() const {
        if (empty()) throw std::runtime_error("AVLTree::max: tree is empty");
        const Node* curr = m_root.get();
        while (curr->right) {
            curr = curr->right.get();
        }
        return curr->data;
    }

    template <typename Func>
    void inorder(Func&& func) const {
        inorderRec(m_root.get(), std::forward<Func>(func));
    }

    [[nodiscard]] std::vector<T> toVector() const {
        std::vector<T> result;
        result.reserve(m_size);
        inorder([&result](const T& val) { result.push_back(val); });
        return result;
    }
};

template <typename T, typename Compare>
void swap(AVLTree<T, Compare>& a, AVLTree<T, Compare>& b) noexcept {
    a.swap(b);
}

} // namespace dsa

#endif // DSA_TREES_AVL_TREE_HPP

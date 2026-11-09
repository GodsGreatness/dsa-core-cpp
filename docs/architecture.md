# Architecture & Memory Management

This document details the architectural layout, memory ownership guarantees, and template design principles underpinning `dsa-core-cpp`.

---

## 1. Library Organization

The project is structured under the top-level namespace `dsa`:

```text
include/dsa/
├── linear/
│   ├── Vector.hpp           # Dynamic resizing contiguous buffer
│   ├── LinkedList.hpp       # Doubly-linked list with sentinels
│   ├── Stack.hpp            # LIFO container adapter
│   └── Queue.hpp            # FIFO container adapter
├── trees/
│   ├── BST.hpp              # Unbalanced Binary Search Tree
│   └── AVLTree.hpp          # Self-Balancing AVL Tree (Flagship)
├── heap/
│   └── PriorityQueue.hpp    # Min/Max Binary Heap
├── graph/
│   └── Graph.hpp            # Generic Adjacency List graph
└── algorithms/
    ├── GraphAlgorithms.hpp  # BFS, DFS, Cycle, TopoSort, Dijkstra
    └── Sorting.hpp          # Insertion, Merge, Quick, Heap Sort
```

---

## 2. Resource Management & RAII Principles

Every container in `dsa-core-cpp` adheres strictly to **Resource Acquisition Is Initialization (RAII)**:

### A. Raw Buffer Allocation in `Vector<T>`
Standard textbook implementations frequently write:
```cpp
// ANTI-PATTERN:
T* data = new T[capacity];
```
This forces all `capacity` elements to be default-constructed immediately, which:
1. Breaks when `T` lacks a default constructor (`T()`).
2. Wastes CPU cycles constructing objects in uninitialized capacity.

Instead, `dsa::Vector<T>` allocates raw byte storage:
```cpp
pointer ptr = static_cast<pointer>(::operator new(n * sizeof(T)));
```
Elements are constructed strictly in-place via **placement new**:
```cpp
new (static_cast<void*>(target)) T(std::forward<Args>(args)...);
```
and destroyed explicitly via `p->~T()` upon element removal or buffer deallocation.

### B. Tree Node Ownership via `std::unique_ptr`
Tree nodes in `BST<T>` and `AVLTree<T>` express explicit exclusive ownership through `std::unique_ptr<Node>`:
```cpp
struct Node {
    T data;
    int height{1};
    std::unique_ptr<Node> left{nullptr};
    std::unique_ptr<Node> right{nullptr};
};
```
* **Rotations:** Rotations move ownership seamlessly using `std::move()`, preventing memory leaks even if an exception occurs mid-rotation.
* **Destruction:** When a tree is cleared or goes out of scope, the destructor cascades automatically down subtrees without manual pointer traversals.

### C. Sentinel Node Design in `LinkedList<T>`
To avoid allocating dummy `T` values for sentinels, `LinkedList<T>` separates node topology from payload:
```cpp
struct NodeBase {
    NodeBase* prev{nullptr};
    NodeBase* next{nullptr};
};

struct Node : public NodeBase {
    T data;
};
```
Sentinels `m_head` and `m_tail` are lightweight `NodeBase` instances that never construct `T`, completely eliminating edge cases for empty lists while supporting non-default-constructible types.

---

## 3. Rule of Five & Exception Safety

Every data structure implements the complete **Rule of Five**:
1. **Destructor:** Releases all allocated resources.
2. **Copy Constructor:** Performs a clean deep copy.
3. **Copy Assignment Operator:** Uses the **copy-and-swap idiom** for strong exception safety:
   ```cpp
   Vector& operator=(const Vector& other) {
       if (this != &other) {
           Vector temp(other);
           swap(temp);
       }
       return *this;
   }
   ```
4. **Move Constructor:** Marked `noexcept`, steals internal pointers and zeroes the source.
5. **Move Assignment Operator:** Swaps or transfers resources cleanly, leaving source in a valid empty state.

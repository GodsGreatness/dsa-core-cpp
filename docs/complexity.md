# Asymptotic Complexity Reference

This document provides a comprehensive asymptotic time and space complexity analysis for all data structures and algorithms implemented in `dsa-core-cpp`.

---

## 1. Linear Data Structures

| Data Structure | Access | Search | Insertion | Deletion | Space Complexity | Notes |
| :--- | :---: | :---: | :---: | :---: | :---: | :--- |
| **`Vector<T>`** | $O(1)$ | $O(n)$ | $O(1)^*$ | $O(1)$ | $O(n)$ | $^*$Amortized $O(1)$ push_back with geometric doubling ($2\times$). Uninitialized raw storage avoids redundant default constructions. |
| **`LinkedList<T>`** | $O(n)$ | $O(n)$ | $O(1)$ | $O(1)$ | $O(n)$ | Doubly-linked with sentinel head and tail nodes. $O(1)$ push/pop at head/tail. |
| **`Stack<T>`** | $O(1)$ (top) | $O(n)$ | $O(1)$ (push) | $O(1)$ (pop) | $O(n)$ | LIFO adapter backed by `Vector<T>`. |
| **`Queue<T>`** | $O(1)$ (front) | $O(n)$ | $O(1)$ (push) | $O(1)$ (pop) | $O(n)$ | FIFO adapter backed by `LinkedList<T>`. |

---

## 2. Tree Data Structures

| Tree Structure | Search (Avg) | Search (Worst) | Insert (Avg) | Insert (Worst) | Delete (Avg) | Delete (Worst) | Space | Invariant |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :--- |
| **`BST<T>`** | $O(\log n)$ | $O(n)$ | $O(\log n)$ | $O(n)$ | $O(\log n)$ | $O(n)$ | $O(n)$ | Degenerates to $O(n)$ on sorted input. |
| **`AVLTree<T>`** | $O(\log n)$ | $O(\log n)$ | $O(\log n)$ | $O(\log n)$ | $O(\log n)$ | $O(\log n)$ | $O(n)$ | Strict height balance: $|height(left) - height(right)| \le 1$. Rotations restore balance in $O(1)$ per backtrack step. |

---

## 3. Priority Queue & Heap

| Operation | Time Complexity | Notes |
| :--- | :---: | :--- |
| **`push()`** | $O(\log n)$ | Sift-up from leaf to root. |
| **`pop()`** | $O(\log n)$ | Sift-down from root to leaf. |
| **`top()`** | $O(1)$ | Peek at root element. |
| **Constructor (Heapify)** | $\mathbf{O(n)}$ | Floyd's bottom-up linear construction algorithm: $\sum_{h=0}^{\lfloor\log n\rfloor} \frac{n}{2^{h+1}} O(h) = O(n)$. |

---

## 4. Graph Algorithms

Given a graph $G = (V, E)$ represented via an adjacency list:

| Algorithm | Time Complexity | Auxiliary Space | Graph Type | Preconditions |
| :--- | :---: | :---: | :--- | :--- |
| **Breadth-First Search (BFS)** | $O(V + E)$ | $O(V)$ | Directed / Undirected | None |
| **Depth-First Search (DFS)** | $O(V + E)$ | $O(V)$ | Directed / Undirected | None |
| **Cycle Detection** | $O(V + E)$ | $O(V)$ | Directed (3-color) / Undirected (parent track) | None |
| **Topological Sort** | $O(V + E)$ | $O(V)$ | Directed Acyclic Graph (DAG) | Graph must be directed and acyclic. |
| **Dijkstra's Shortest Path** | $\mathbf{O((V + E) \log V)}$ | $O(V)$ | Weighted (Directed / Undirected) | **Strict Precondition: All edge weights $w \ge 0$.** |

---

## 5. Sorting Algorithms

| Algorithm | Best Time | Average Time | Worst Time | Space | Stable | Method |
| :--- | :---: | :---: | :---: | :---: | :---: | :--- |
| **Insertion Sort** | $O(n)$ | $O(n^2)$ | $O(n^2)$ | $O(1)$ | **Yes** | In-place, adaptive. Excellent for small or nearly sorted collections ($n \le 16$). |
| **Merge Sort** | $O(n \log n)$ | $O(n \log n)$ | $O(n \log n)$ | $O(n)$ | **Yes** | Divide and conquer. Guaranteed $O(n \log n)$ upper bound and stable. |
| **Quick Sort** | $O(n \log n)$ | $O(n \log n)$ | $O(n^2)$ | $O(\log n)$ | No | Median-of-three pivot selection, tail-call recursion optimization, hybrid insertion fallback for $n \le 16$. |
| **Heap Sort** | $O(n \log n)$ | $O(n \log n)$ | $O(n \log n)$ | $\mathbf{O(1)}$ | No | In-place comparison sort using Floyd's bottom-up heapification. |

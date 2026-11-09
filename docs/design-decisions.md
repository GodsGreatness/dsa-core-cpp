# Design Decisions & Trade-Offs

This document captures the engineering rationales behind key architectural decisions made throughout the project.

---

## 1. Why Header-Only Template Design?
* **Rationale:** As a generic algorithmic library, all container and algorithm classes must be parameterized by type (`template <typename T>`). Placing implementations in header files allows the compiler to instantiate templates on demand with full inlining opportunities, eliminating translation unit boundary overhead.

---

## 2. Invariant Validation in `AVLTree<T>`
* **Rationale:** Merely running `insert()` and `search()` does not verify whether self-balancing is working correctly. A degraded tree can pass binary search tests while running at $O(n)$ degraded performance.
* **Solution:** `AVLTree<T>::verifyInvariants()` recursively verifies:
  1. **Strict BST Ordering:** $\forall x \in \text{Left}(u), x < u$ and $\forall y \in \text{Right}(u), y > u$.
  2. **Height Accuracy:** $H(u) = 1 + \max(H(\text{Left}), H(\text{Right}))$.
  3. **Balance Factor Invariant:** $|H(\text{Left}) - H(\text{Right})| \le 1$.
* **Testing Impact:** Our test suite verifies this invariant after every single isolated rotation ($LL, RR, LR, RL$) and across 5,000 randomized insertions and deletions.

---

## 3. Dijkstra Negative Weight Precondition
* **Rationale:** Dijkstra's algorithm relies fundamentally on the greedy choice property: once a vertex is settled from the priority queue, its computed shortest distance is immutable. With negative edge weights, this invariant breaks, leading to incorrect distances.
* **Engineering Choice:** Rather than silently producing incorrect outputs, `GraphAlgorithms::dijkstra()` inspects edge weights in the graph and explicitly throws `std::invalid_argument` if any edge weight $w < 0$.

---

## 4. Floyd's Bottom-Up Linear Heap Construction
* **Rationale:** Inserting $n$ elements into an empty priority queue takes $O(n \log n)$ time.
* **Optimization:** When initializing `PriorityQueue<T>` from an existing array or initializer list, we apply **Floyd's bottom-up heapification algorithm**:
  $$\sum_{h=0}^{\lfloor\log n\rfloor} \frac{n}{2^{h+1}} O(h) = O(n)$$
  This constructs the entire binary heap in strict linear $O(n)$ time.

---

## 5. Hybrid QuickSort with Insertion Fallback
* **Rationale:** Recursive partitioning incurs stack overhead and cache misses on tiny subarrays.
* **Optimization:** For subarrays with size $n \le 16$, `Sorting::quickSort` terminates recursive partitioning and delegates to `Sorting::insertionSort`, which is faster on small contiguous blocks due to high cache locality and minimal branch mispredictions. Furthermore, **tail-call optimization** ensures the recursion stack depth never exceeds $O(\log n)$.

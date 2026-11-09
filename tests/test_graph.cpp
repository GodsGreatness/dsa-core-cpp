#include "dsa/algorithms/GraphAlgorithms.hpp"
#include "dsa/graph/Graph.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

void testGraphTopology() {
    std::cout << "[Test Graph] Directed vs Undirected topology... ";
    dsa::Graph<std::string> undir(false);
    undir.addEdge("A", "B", 5.0);
    assert(undir.hasVertex("A") && undir.hasVertex("B"));
    assert(undir.hasEdge("A", "B") && undir.hasEdge("B", "A"));
    assert(undir.numVertices() == 2 && undir.numEdges() == 1);

    dsa::Graph<std::string> dir(true);
    dir.addEdge("A", "B", 5.0);
    assert(dir.hasEdge("A", "B"));
    assert(!dir.hasEdge("B", "A"));
    assert(dir.numEdges() == 1);

    dir.removeEdge("A", "B");
    assert(!dir.hasEdge("A", "B"));
    assert(dir.numEdges() == 0);

    std::cout << "PASSED\n";
}

void testBFSandDFS() {
    std::cout << "[Test Graph] BFS and DFS traversals... ";
    dsa::Graph<int> g(true);
    g.addEdge(1, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 4);
    g.addEdge(3, 5);

    std::vector<int> bfsOrder;
    dsa::GraphAlgorithms::bfs(g, 1, [&](int v) { bfsOrder.push_back(v); });
    assert(bfsOrder.size() == 5);
    assert(bfsOrder[0] == 1);
    // Level 1: 2 and 3
    assert((bfsOrder[1] == 2 && bfsOrder[2] == 3) || (bfsOrder[1] == 3 && bfsOrder[2] == 2));

    std::vector<int> dfsOrder;
    dsa::GraphAlgorithms::dfs(g, 1, [&](int v) { dfsOrder.push_back(v); });
    assert(dfsOrder.size() == 5);
    assert(dfsOrder[0] == 1);

    std::cout << "PASSED\n";
}

void testCycleDetection() {
    std::cout << "[Test Graph] Cycle detection (Directed & Undirected)... ";
    // Directed DAG
    dsa::Graph<int> dag(true);
    dag.addEdge(1, 2);
    dag.addEdge(2, 3);
    dag.addEdge(1, 3);
    assert(!dsa::GraphAlgorithms::hasCycle(dag));

    // Directed Cyclic
    dsa::Graph<int> cyclicDir(true);
    cyclicDir.addEdge(1, 2);
    cyclicDir.addEdge(2, 3);
    cyclicDir.addEdge(3, 1);
    assert(dsa::GraphAlgorithms::hasCycle(cyclicDir));

    // Undirected Tree vs Cycle
    dsa::Graph<int> tree(false);
    tree.addEdge(1, 2);
    tree.addEdge(2, 3);
    assert(!dsa::GraphAlgorithms::hasCycle(tree));

    tree.addEdge(3, 1); // Forms triangle cycle
    assert(dsa::GraphAlgorithms::hasCycle(tree));

    std::cout << "PASSED\n";
}

void testTopologicalSort() {
    std::cout << "[Test Graph] Topological sorting (Kahn's algorithm)... ";
    dsa::Graph<std::string> dag(true);
    dag.addEdge("CS101", "CS201");
    dag.addEdge("CS101", "MATH101");
    dag.addEdge("CS201", "CS301");
    dag.addEdge("MATH101", "CS301");

    std::vector<std::string> order = dsa::GraphAlgorithms::topologicalSort(dag);
    assert(order.size() == 4);
    assert(order[0] == "CS101");
    assert(order.back() == "CS301");

    // Cycle check throws invalid_argument
    dag.addEdge("CS301", "CS101"); // Back-edge cycle
    bool caught = false;
    try {
        dsa::GraphAlgorithms::topologicalSort(dag);
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);

    std::cout << "PASSED\n";
}

void testDijkstraShortestPath() {
    std::cout << "[Test Graph] Dijkstra's Algorithm & Path Reconstruction... ";
    dsa::Graph<std::string, double> g(false);
    g.addEdge("A", "B", 4.0);
    g.addEdge("A", "C", 2.0);
    g.addEdge("C", "B", 1.0);
    g.addEdge("B", "D", 5.0);
    g.addEdge("C", "D", 8.0);

    auto result = dsa::GraphAlgorithms::dijkstra(g, std::string("A"));

    // Shortest path A -> B is through C (2.0 + 1.0 = 3.0, less than direct 4.0)
    assert(result.distanceTo("B") == 3.0);
    assert(result.distanceTo("C") == 2.0);
    // Shortest path A -> D is A -> C -> B -> D (2.0 + 1.0 + 5.0 = 8.0)
    assert(result.distanceTo("D") == 8.0);

    std::vector<std::string> pathD = result.getPathTo("D");
    assert(pathD == (std::vector<std::string>{"A", "C", "B", "D"}));

    std::cout << "PASSED\n";
}

void testDijkstraNegativeWeightConstraint() {
    std::cout << "[Test Graph] Dijkstra negative weight rejection... ";
    dsa::Graph<std::string, double> g(true);
    g.addEdge("A", "B", 5.0);
    g.addEdge("B", "C", -2.0); // Negative weight edge!

    bool caught = false;
    try {
        dsa::GraphAlgorithms::dijkstra(g, std::string("A"));
    } catch (const std::invalid_argument& ex) {
        caught = true;
    }
    assert(caught);

    std::cout << "PASSED\n";
}

int main() {
    std::cout << "===========================================\n";
    std::cout << " Running Graph & GraphAlgorithms Test Suite\n";
    std::cout << "===========================================\n";

    testGraphTopology();
    testBFSandDFS();
    testCycleDetection();
    testTopologicalSort();
    testDijkstraShortestPath();
    testDijkstraNegativeWeightConstraint();

    std::cout << ">>> ALL GRAPH TESTS PASSED! <<<\n\n";
    return 0;
}

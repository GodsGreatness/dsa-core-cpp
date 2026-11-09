#ifndef DSA_ALGORITHMS_GRAPH_ALGORITHMS_HPP
#define DSA_ALGORITHMS_GRAPH_ALGORITHMS_HPP

#include "dsa/graph/Graph.hpp"
#include <algorithm>
#include <limits>
#include <queue>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace dsa {

class GraphAlgorithms {
public:
    // -------------------------------------------------------------
    // Breadth-First Search (BFS)
    // -------------------------------------------------------------
    template <typename Vertex, typename Weight, typename Visitor>
    static void bfs(const Graph<Vertex, Weight>& graph, const Vertex& start, Visitor&& visitor) {
        if (!graph.hasVertex(start)) {
            throw std::out_of_range("GraphAlgorithms::bfs: start vertex not in graph");
        }

        std::unordered_set<Vertex> visited;
        std::queue<Vertex> q;

        visited.insert(start);
        q.push(start);

        while (!q.empty()) {
            Vertex curr = q.front();
            q.pop();

            visitor(curr);

            for (const auto& edge : graph.neighbors(curr)) {
                if (visited.find(edge.target) == visited.end()) {
                    visited.insert(edge.target);
                    q.push(edge.target);
                }
            }
        }
    }

    // -------------------------------------------------------------
    // Depth-First Search (DFS)
    // -------------------------------------------------------------
    template <typename Vertex, typename Weight, typename Visitor>
    static void dfs(const Graph<Vertex, Weight>& graph, const Vertex& start, Visitor&& visitor) {
        if (!graph.hasVertex(start)) {
            throw std::out_of_range("GraphAlgorithms::dfs: start vertex not in graph");
        }

        std::unordered_set<Vertex> visited;
        dfsHelper(graph, start, visited, visitor);
    }

private:
    template <typename Vertex, typename Weight, typename Visitor>
    static void dfsHelper(const Graph<Vertex, Weight>& graph,
                          const Vertex& curr,
                          std::unordered_set<Vertex>& visited,
                          Visitor& visitor) {
        visited.insert(curr);
        visitor(curr);

        for (const auto& edge : graph.neighbors(curr)) {
            if (visited.find(edge.target) == visited.end()) {
                dfsHelper(graph, edge.target, visited, visitor);
            }
        }
    }

public:
    // -------------------------------------------------------------
    // Cycle Detection
    // -------------------------------------------------------------
    template <typename Vertex, typename Weight>
    static bool hasCycle(const Graph<Vertex, Weight>& graph) {
        if (graph.isDirected()) {
            // 3-color DFS (0 = unvisited, 1 = visiting/gray, 2 = visited/black)
            std::unordered_map<Vertex, int> state;
            for (const auto& v : graph.getVertices()) {
                state[v] = 0;
            }

            for (const auto& v : graph.getVertices()) {
                if (state[v] == 0) {
                    if (directedCycleDFS(graph, v, state)) {
                        return true;
                    }
                }
            }
            return false;
        } else {
            // Undirected graph cycle detection with parent tracking
            std::unordered_set<Vertex> visited;
            for (const auto& v : graph.getVertices()) {
                if (visited.find(v) == visited.end()) {
                    if (undirectedCycleDFS(graph, v, Vertex{}, visited, false)) {
                        return true;
                    }
                }
            }
            return false;
        }
    }

private:
    template <typename Vertex, typename Weight>
    static bool directedCycleDFS(const Graph<Vertex, Weight>& graph,
                                 const Vertex& curr,
                                 std::unordered_map<Vertex, int>& state) {
        state[curr] = 1; // Visiting (in current recursion stack)

        for (const auto& edge : graph.neighbors(curr)) {
            if (state[edge.target] == 1) {
                return true; // Back-edge detected
            }
            if (state[edge.target] == 0) {
                if (directedCycleDFS(graph, edge.target, state)) {
                    return true;
                }
            }
        }

        state[curr] = 2; // Fully visited
        return false;
    }

    template <typename Vertex, typename Weight>
    static bool undirectedCycleDFS(const Graph<Vertex, Weight>& graph,
                                   const Vertex& curr,
                                   const Vertex& parent,
                                   std::unordered_set<Vertex>& visited,
                                   bool hasParent) {
        visited.insert(curr);

        for (const auto& edge : graph.neighbors(curr)) {
            if (visited.find(edge.target) == visited.end()) {
                if (undirectedCycleDFS(graph, edge.target, curr, visited, true)) {
                    return true;
                }
            } else if (hasParent && edge.target != parent) {
                return true;
            }
        }
        return false;
    }

public:
    // -------------------------------------------------------------
    // Topological Sort (Kahn's Algorithm)
    // -------------------------------------------------------------
    template <typename Vertex, typename Weight>
    static std::vector<Vertex> topologicalSort(const Graph<Vertex, Weight>& graph) {
        if (!graph.isDirected()) {
            throw std::invalid_argument("GraphAlgorithms::topologicalSort: graph must be directed (DAG)");
        }

        std::unordered_map<Vertex, std::size_t> inDegree;
        for (const auto& v : graph.getVertices()) {
            inDegree[v] = 0;
        }

        for (const auto& pair : graph.adjacencyList()) {
            for (const auto& edge : pair.second) {
                inDegree[edge.target]++;
            }
        }

        std::queue<Vertex> zeroInDegree;
        for (const auto& pair : inDegree) {
            if (pair.second == 0) {
                zeroInDegree.push(pair.first);
            }
        }

        std::vector<Vertex> order;
        order.reserve(graph.numVertices());

        while (!zeroInDegree.empty()) {
            Vertex u = zeroInDegree.front();
            zeroInDegree.pop();
            order.push_back(u);

            for (const auto& edge : graph.neighbors(u)) {
                if (--inDegree[edge.target] == 0) {
                    zeroInDegree.push(edge.target);
                }
            }
        }

        if (order.size() != graph.numVertices()) {
            throw std::invalid_argument("GraphAlgorithms::topologicalSort: graph contains a cycle");
        }

        return order;
    }

    // -------------------------------------------------------------
    // Dijkstra's Shortest Path
    // -------------------------------------------------------------
    template <typename Vertex, typename Weight>
    struct ShortestPathResult {
        std::unordered_map<Vertex, Weight> distances;
        std::unordered_map<Vertex, Vertex> predecessors;
        Vertex source;

        bool hasPathTo(const Vertex& dest) const {
            return distances.find(dest) != distances.end() &&
                   distances.at(dest) != std::numeric_limits<Weight>::infinity();
        }

        Weight distanceTo(const Vertex& dest) const {
            if (!hasPathTo(dest)) {
                throw std::out_of_range("No path to destination vertex");
            }
            return distances.at(dest);
        }

        std::vector<Vertex> getPathTo(const Vertex& dest) const {
            if (!hasPathTo(dest)) {
                return {};
            }

            std::vector<Vertex> path;
            Vertex curr = dest;
            while (curr != source) {
                path.push_back(curr);
                curr = predecessors.at(curr);
            }
            path.push_back(source);
            std::reverse(path.begin(), path.end());
            return path;
        }
    };

    template <typename Vertex, typename Weight>
    static ShortestPathResult<Vertex, Weight> dijkstra(const Graph<Vertex, Weight>& graph, const Vertex& start) {
        if (!graph.hasVertex(start)) {
            throw std::out_of_range("GraphAlgorithms::dijkstra: start vertex not in graph");
        }

        // Explicit Check: Dijkstra requires non-negative edge weights
        for (const auto& pair : graph.adjacencyList()) {
            for (const auto& edge : pair.second) {
                if (edge.weight < static_cast<Weight>(0)) {
                    throw std::invalid_argument("GraphAlgorithms::dijkstra: negative edge weights are not supported");
                }
            }
        }

        ShortestPathResult<Vertex, Weight> result;
        result.source = start;

        for (const auto& v : graph.getVertices()) {
            result.distances[v] = std::numeric_limits<Weight>::infinity();
        }
        result.distances[start] = static_cast<Weight>(0);

        // Min-heap of (distance, vertex)
        using Element = std::pair<Weight, Vertex>;
        std::priority_queue<Element, std::vector<Element>, std::greater<Element>> pq;
        pq.push({static_cast<Weight>(0), start});

        std::unordered_set<Vertex> settled;

        while (!pq.empty()) {
            auto [currDist, u] = pq.top();
            pq.pop();

            if (settled.find(u) != settled.end()) continue;
            settled.insert(u);

            for (const auto& edge : graph.neighbors(u)) {
                Vertex v = edge.target;
                Weight weight = edge.weight;

                if (currDist + weight < result.distances[v]) {
                    result.distances[v] = currDist + weight;
                    result.predecessors[v] = u;
                    pq.push({result.distances[v], v});
                }
            }
        }

        return result;
    }
};

} // namespace dsa

#endif // DSA_ALGORITHMS_GRAPH_ALGORITHMS_HPP

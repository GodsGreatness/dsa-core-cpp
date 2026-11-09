#ifndef DSA_GRAPH_GRAPH_HPP
#define DSA_GRAPH_GRAPH_HPP

#include <cstddef>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

namespace dsa {

template <typename Vertex, typename Weight = double>
class Graph {
public:
    struct Edge {
        Vertex target;
        Weight weight;

        Edge(Vertex tgt, Weight w) : target(std::move(tgt)), weight(w) {}

        bool operator==(const Edge& other) const {
            return target == other.target && weight == other.weight;
        }
    };

private:
    bool m_directed{false};
    std::unordered_map<Vertex, std::vector<Edge>> m_adjList;
    std::size_t m_edgeCount{0};

public:
    using vertex_type = Vertex;
    using weight_type = Weight;
    using edge_type = Edge;

    explicit Graph(bool isDirected = false) noexcept : m_directed(isDirected) {}

    [[nodiscard]] bool isDirected() const noexcept { return m_directed; }
    [[nodiscard]] std::size_t numVertices() const noexcept { return m_adjList.size(); }
    [[nodiscard]] std::size_t numEdges() const noexcept { return m_edgeCount; }
    [[nodiscard]] bool empty() const noexcept { return m_adjList.empty(); }

    void clear() noexcept {
        m_adjList.clear();
        m_edgeCount = 0;
    }

    bool addVertex(const Vertex& v) {
        if (m_adjList.find(v) != m_adjList.end()) {
            return false;
        }
        m_adjList[v] = {};
        return true;
    }

    bool hasVertex(const Vertex& v) const {
        return m_adjList.find(v) != m_adjList.end();
    }

    bool removeVertex(const Vertex& v) {
        auto it = m_adjList.find(v);
        if (it == m_adjList.end()) {
            return false;
        }

        // Subtract outgoing edges
        m_edgeCount -= it->second.size();
        m_adjList.erase(it);

        // Remove incoming edges from all other vertices
        for (auto& pair : m_adjList) {
            auto& edges = pair.second;
            for (auto edgeIt = edges.begin(); edgeIt != edges.end();) {
                if (edgeIt->target == v) {
                    edgeIt = edges.erase(edgeIt);
                    if (m_directed) {
                        --m_edgeCount;
                    }
                } else {
                    ++edgeIt;
                }
            }
        }

        return true;
    }

    bool addEdge(const Vertex& u, const Vertex& v, Weight weight = static_cast<Weight>(1)) {
        addVertex(u);
        addVertex(v);

        // Check if edge already exists
        for (const auto& edge : m_adjList[u]) {
            if (edge.target == v) {
                return false; // Edge already exists
            }
        }

        m_adjList[u].emplace_back(v, weight);
        ++m_edgeCount;

        if (!m_directed && u != v) {
            m_adjList[v].emplace_back(u, weight);
        }

        return true;
    }

    bool hasEdge(const Vertex& u, const Vertex& v) const {
        auto it = m_adjList.find(u);
        if (it == m_adjList.end()) return false;
        for (const auto& edge : it->second) {
            if (edge.target == v) return true;
        }
        return false;
    }

    Weight getWeight(const Vertex& u, const Vertex& v) const {
        auto it = m_adjList.find(u);
        if (it == m_adjList.end()) {
            throw std::out_of_range("Graph::getWeight: source vertex not found");
        }
        for (const auto& edge : it->second) {
            if (edge.target == v) {
                return edge.weight;
            }
        }
        throw std::out_of_range("Graph::getWeight: edge does not exist");
    }

    bool removeEdge(const Vertex& u, const Vertex& v) {
        auto itU = m_adjList.find(u);
        if (itU == m_adjList.end()) return false;

        bool removed = false;
        for (auto it = itU->second.begin(); it != itU->second.end(); ++it) {
            if (it->target == v) {
                itU->second.erase(it);
                --m_edgeCount;
                removed = true;
                break;
            }
        }

        if (!m_directed && u != v) {
            auto itV = m_adjList.find(v);
            if (itV != m_adjList.end()) {
                for (auto it = itV->second.begin(); it != itV->second.end(); ++it) {
                    if (it->target == u) {
                        itV->second.erase(it);
                        break;
                    }
                }
            }
        }

        return removed;
    }

    const std::vector<Edge>& neighbors(const Vertex& v) const {
        auto it = m_adjList.find(v);
        if (it == m_adjList.end()) {
            throw std::out_of_range("Graph::neighbors: vertex not found");
        }
        return it->second;
    }

    std::vector<Vertex> getVertices() const {
        std::vector<Vertex> result;
        result.reserve(m_adjList.size());
        for (const auto& pair : m_adjList) {
            result.push_back(pair.first);
        }
        return result;
    }

    const std::unordered_map<Vertex, std::vector<Edge>>& adjacencyList() const noexcept {
        return m_adjList;
    }
};

} // namespace dsa

#endif // DSA_GRAPH_GRAPH_HPP

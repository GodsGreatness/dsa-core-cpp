#include "dsa/algorithms/GraphAlgorithms.hpp"
#include "dsa/graph/Graph.hpp"
#include <iostream>
#include <string>

int main() {
    std::cout << "=== dsa::Graph & Dijkstra Shortest Path Example ===\n\n";

    dsa::Graph<std::string, double> cityMap(false); // Undirected weighted graph

    cityMap.addEdge("Accra", "Kumasi", 250.0);
    cityMap.addEdge("Accra", "Cape Coast", 150.0);
    cityMap.addEdge("Cape Coast", "Takoradi", 80.0);
    cityMap.addEdge("Cape Coast", "Kumasi", 180.0);
    cityMap.addEdge("Kumasi", "Tamale", 390.0);
    cityMap.addEdge("Takoradi", "Kumasi", 240.0);

    std::cout << "Running Dijkstra's algorithm from Accra...\n";
    auto route = dsa::GraphAlgorithms::dijkstra(cityMap, std::string("Accra"));

    std::vector<std::string> destinations = {"Kumasi", "Cape Coast", "Takoradi", "Tamale"};

    for (const auto& dest : destinations) {
        std::cout << "  - Route to " << dest << " (Distance: " << route.distanceTo(dest) << " km): ";
        auto path = route.getPathTo(dest);
        for (std::size_t i = 0; i < path.size(); ++i) {
            std::cout << path[i] << (i + 1 < path.size() ? " -> " : "");
        }
        std::cout << "\n";
    }

    return 0;
}

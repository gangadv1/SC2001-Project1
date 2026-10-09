#include <fstream>
#include <filesystem>
#include <string>

// Import your implementation without its example main().
#define main matrix_example_main
#include "dijkstra_matrix.cpp"
#undef main

// Import your teammate's implementation.
// Rename INF to avoid conflicting with your INF constant.
#define INF HEAP_INF
#define main heap_example_main
#include "../part_b/dijkstra_heap.cpp"
#undef main
#undef INF

int main() {
    int passed = 0;

    for (const auto& entry :
         std::filesystem::directory_iterator("../datasets")) {

        if (entry.path().extension() != ".txt") {
            continue;
        }

        std::ifstream file(entry.path());

        int V;
        long long E;

        if (!(file >> V >> E) || V <= 0 || E < 0) {
            std::cerr << "Invalid dataset: " << entry.path() << '\n';
            return 1;
        }

        GraphMatrix matrixGraph(V);
        GraphAdjList heapGraph(V);

        for (long long i = 0; i < E; ++i) {
            int u, v;
            long long weight;

            if (!(file >> u >> v >> weight) ||
                u < 0 || u >= V ||
                v < 0 || v >= V ||
                weight < 0) {
                std::cerr << "Invalid edge: " << entry.path() << '\n';
                return 1;
            }

            matrixGraph.addEdge(u, v, weight);
            heapGraph.addEdge(u, v, weight);
        }

        auto matrixDistances = matrixGraph.dijkstra(0);
        auto heapDistances = heapGraph.dijkstra(0);

        for (int v = 0; v < V; ++v) {
            if (matrixDistances[v] != heapDistances[v]) {
                std::cerr << "FAIL: "
                          << entry.path().filename().string()
                          << " at vertex " << v << '\n';
                return 1;
            }
        }

        std::cout << "PASS: "
                  << entry.path().filename().string() << '\n';
        ++passed;
    }

    if (passed == 0) {
        std::cerr << "No datasets found.\n";
        return 1;
    }

    std::cout << "\nAll " << passed
              << " datasets produced matching distances.\n";
}
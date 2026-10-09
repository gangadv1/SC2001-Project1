#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <chrono>
#include <string>
#include <iomanip>
#include <stdexcept>

using namespace std;

const long long INF = numeric_limits<long long>::max();

class GraphMatrix {
public:
    int V;
    vector<vector<long long>> matrix;

    GraphMatrix(int numVertices)
        : V(numVertices),
          matrix(numVertices, vector<long long>(numVertices, -1)) {}

    void addEdge(int u, int v, long long weight) {
        matrix[u][v] = weight;
    }

    vector<long long> dijkstra(int source) {
        vector<long long> dist(V, INF);
        vector<bool> visited(V, false);

        dist[source] = 0;

        for (int iteration = 0; iteration < V; ++iteration) {
            int u = -1;

            // Array priority queue: find the smallest priority.
            for (int v = 0; v < V; ++v) {
                if (!visited[v] &&
                    (u == -1 || dist[v] < dist[u])) {
                    u = v;
                }
            }

            if (u == -1 || dist[u] == INF) {
                break;
            }

            visited[u] = true;

            // Scan the selected vertex's matrix row.
            for (int v = 0; v < V; ++v) {
                if (!visited[v] && matrix[u][v] != -1) {
                    long long weight = matrix[u][v];

                    if (weight <= INF - dist[u]) {
                        long long candidate = dist[u] + weight;

                        if (candidate < dist[v]) {
                            dist[v] = candidate;
                        }
                    }
                }
            }
        }

        return dist;
    }
};

// Consume each result outside the timer.
volatile unsigned long long resultChecksum = 0;

void benchmarkGraph(
    const string& filename,
    ofstream& results,
    int trials = 5
) {
    // Paths are relative to the part_a folder.
    ifstream fin("../datasets/" + filename);

    if (!fin.is_open()) {
        throw runtime_error("Cannot open dataset: " + filename);
    }

    int V;
    long long E;

    if (!(fin >> V >> E) || V <= 0 || E < 0) {
        throw runtime_error("Invalid graph header: " + filename);
    }

    GraphMatrix graph(V);

    for (long long i = 0; i < E; ++i) {
        int u, v;
        long long weight;

        if (!(fin >> u >> v >> weight) ||
            u < 0 || u >= V ||
            v < 0 || v >= V ||
            weight < 0 || weight == INF) {
            throw runtime_error("Invalid edge in: " + filename);
        }

        // Directed edge: do not add the reverse edge.
        graph.addEdge(u, v, weight);
    }

    fin.close();

    double totalTimeMs = 0.0;

    for (int trial = 0; trial < trials; ++trial) {
        auto start = chrono::steady_clock::now();

        vector<long long> dist = graph.dijkstra(0);

        auto end = chrono::steady_clock::now();

        totalTimeMs +=
            chrono::duration<double, milli>(end - start).count();

        // Use the distances after timing.
        unsigned long long checksum = 0;

        for (long long distance : dist) {
            if (distance != INF) {
                checksum += static_cast<unsigned long long>(distance);
            }
        }

        resultChecksum = resultChecksum + checksum;
    }

    double averageTimeMs = totalTimeMs / trials;

    cout << filename << " | V=" << V
         << " | E=" << E
         << " | " << averageTimeMs << " ms\n";

    results << filename << ',' << V << ',' << E << ','
            << averageTimeMs << '\n';
}

int main() {
    vector<string> files = {
        // Fixed V = 1000, varying E.
        "graph_V1000_E2000.txt",
        "graph_V1000_E10000.txt",
        "graph_V1000_E50000.txt",
        "graph_V1000_E200000.txt",
        "graph_V1000_E500000.txt",

        // Dense graphs, varying V.
        "graph_dense_V200.txt",
        "graph_dense_V400.txt",
        "graph_dense_V600.txt",
        "graph_dense_V800.txt",
        "graph_dense_V1000.txt",

        // Sparse graphs, varying V.
        "graph_sparse_V500.txt",
        "graph_sparse_V1000.txt",
        "graph_sparse_V2000.txt",
        "graph_sparse_V4000.txt",
        "graph_sparse_V8000.txt"
    };

    ofstream results("matrix_results.csv");

    if (!results.is_open()) {
        cerr << "Cannot create matrix_results.csv\n";
        return 1;
    }

    results << "Filename,V,E,Average_Time_ms\n";
    results << fixed << setprecision(6);

    cout << fixed << setprecision(6);
    cout << "Average runtime over 5 trials per graph\n\n";

    try {
        for (const string& filename : files) {
            benchmarkGraph(filename, results);
        }
    } catch (const exception& error) {
        cerr << error.what() << '\n';
        return 1;
    }

    cout << "\nResults saved to matrix_results.csv\n";

    return 0;
}
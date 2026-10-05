#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <fstream>
#include <chrono>
#include <string>

using namespace std;

const long long INF = numeric_limits<long long>::max();

struct Edge {
    int to;
    long long weight;
};

class GraphAdjList {
public:
    int V;
    vector<vector<Edge>> adj;

    GraphAdjList(int numVertices) : V(numVertices), adj(numVertices) {}

    void addEdge(int u, int v, long long weight) {
        adj[u].push_back({v, weight});
    }

    vector<long long> dijkstra(int source) {
        vector<long long> dist(V, INF);
        dist[source] = 0;

        priority_queue<pair<long long, int>, 
                       vector<pair<long long, int>>, 
                       greater<pair<long long, int>>> minHeap;

        minHeap.push({0, source});

        while (!minHeap.empty()) {
            auto [d, u] = minHeap.top();
            minHeap.pop();

            if (d > dist[u]) {
                continue;
            }

            for (const auto& edge : adj[u]) {
                int v = edge.to;
                long long weight = edge.weight;

                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    minHeap.push({dist[v], v});
                }
            }
        }

        return dist;
    }
};

double benchmarkGraph(const string& filename, int trials = 5) {
    ifstream fin(filename);
    if (!fin.is_open()) {
        cerr << "Error opening file: " << filename << "\n";
        return -1.0;
    }

    int V, E;
    fin >> V >> E;

    GraphAdjList g(V);
    int u, v;
    long long w;
    for (int i = 0; i < E; ++i) {
        fin >> u >> v >> w;
        g.addEdge(u, v, w);
    }
    fin.close();

    double totalTimeMs = 0.0;
    for (int t = 0; t < trials; ++t) {
        auto start = chrono::high_resolution_clock::now();
        g.dijkstra(0);
        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double, milli> duration = end - start;
        totalTimeMs += duration.count();
    }

    return totalTimeMs / trials;
}

int main() {
    vector<string> files = {
        // Set 1: Varying |E| (|V| = 1000)
        "graph_V1000_E2000.txt",
        "graph_V1000_E10000.txt",
        "graph_V1000_E50000.txt",
        "graph_V1000_E200000.txt",
        "graph_V1000_E500000.txt",

        // Set 2: Dense graphs (|E| ≈ 0.5 * |V|^2)
        "graph_dense_V200.txt",
        "graph_dense_V400.txt",
        "graph_dense_V600.txt",
        "graph_dense_V800.txt",
        "graph_dense_V1000.txt",

        // Set 3: Sparse graphs
        "graph_sparse_V500.txt",
        "graph_sparse_V1000.txt",
        "graph_sparse_V2000.txt",
        "graph_sparse_V4000.txt",
        "graph_sparse_V8000.txt"
    };

    cout << "Filename | Average Time (ms)\n";
    cout << "------------------------------------\n";

    for (const auto& file : files) {
        double avgTime = benchmarkGraph(file, 5);
        if (avgTime >= 0) {
            cout << file << " : " << avgTime << " ms\n";
        }
    }

    return 0;
}
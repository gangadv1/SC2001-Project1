#include <iostream>
#include <fstream>
#include <random>
#include <set>
#include <string>
#include <vector>

using namespace std;

void generateGraph(int V, long long E, const string& filename) {
    ofstream fout(filename);
    if (!fout.is_open()) {
        cerr << "Error creating " << filename << "\n";
        return;
    }

    fout << V << " " << E << "\n";

    mt19937 rng(42); // Fixed seed ensures identical output every run
    uniform_int_distribution<int> nodeDist(0, V - 1);
    uniform_int_distribution<int> weightDist(1, 100);

    set<pair<int, int>> edges;

    // 1. Ensure the graph is connected: make a simple chain 0 -> 1 -> 2 ... -> V-1
    for (int i = 0; i < V - 1 && (long long)edges.size() < E; ++i) {
        edges.insert({i, i + 1});
        fout << i << " " << (i + 1) << " " << weightDist(rng) << "\n";
    }

    // 2. Fill remaining edges randomly without self-loops or duplicate edges
    while ((long long)edges.size() < E) {
        int u = nodeDist(rng);
        int v = nodeDist(rng);
        if (u != v && edges.find({u, v}) == edges.end()) {
            edges.insert({u, v});
            fout << u << " " << v << " " << weightDist(rng) << "\n";
        }
    }

    cout << "Generated: " << filename << " (V=" << V << ", E=" << E << ")\n";
}

int main() {
    cout << "Generating test files...\n";

    // Experiment 1: Fixed V = 1000, varying E
    int fixedV = 1000;
    vector<long long> edgesExp1 = {2000, 10000, 50000, 200000, 500000};
    for (long long E : edgesExp1) {
        generateGraph(fixedV, E, "graph_V" + to_string(fixedV) + "_E" + to_string(E) + ".txt");
    }

    // Experiment 2a: Varying V, Sparse (E = 5 * V)
    vector<int> verticesSparse = {500, 1000, 2000, 4000, 8000};
    for (int V : verticesSparse) {
        long long E = 5LL * V;
        generateGraph(V, E, "graph_sparse_V" + to_string(V) + ".txt");
    }

    // Experiment 2b: Varying V, Dense (E = 0.5 * V^2)
    vector<int> verticesDense = {200, 400, 600, 800, 1000};
    for (int V : verticesDense) {
        long long E = (1LL * V * (V - 1)) / 2; // ~0.5 * V^2 without exceeding complete graph max
        generateGraph(V, E, "graph_dense_V" + to_string(V) + ".txt");
    }

    cout << "All files generated successfully.\n";
    return 0;
}
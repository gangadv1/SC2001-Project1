#include <iostream>
#include <vector>
#include <queue>
#include <limits>

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

int main() {
    int V = 4;
    GraphAdjList g(V);

    // 0: A, 1: B, 2: C, 3: D
    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 2);
    g.addEdge(2, 3, 3);
    g.addEdge(1, 3, 1);

    int source = 0;
    vector<long long> dist = g.dijkstra(source);

    for (int i = 0; i < V; ++i) {
        cout << "Distance from " << source << " to " << i << " = " << dist[i] << "\n";
    }

    return 0;
}
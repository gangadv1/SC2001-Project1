#include <iostream>
#include <vector>
#include <limits>

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
        // dist stores the priorities in our array-based queue.
        vector<long long> dist(V, INF);
        vector<bool> visited(V, false);

        dist[source] = 0;

        for (int iteration = 0; iteration < V; ++iteration) {
            int u = -1;

            // Find the unvisited vertex with the smallest distance.
            for (int v = 0; v < V; ++v) {
                if (!visited[v] &&
                    (u == -1 || dist[v] < dist[u])) {
                    u = v;
                }
            }

            // Stop if no remaining vertex is reachable.
            if (u == -1 || dist[u] == INF) {
                break;
            }

            visited[u] = true;

            // Scan the matrix row and update neighbouring distances.
            for (int v = 0; v < V; ++v) {
                if (!visited[v] && matrix[u][v] != -1) {
                    long long weight = matrix[u][v];

                    // Prevent overflow when adding the edge weight.
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

int main() {
    // Same example graph as your teammate's Part (b).
    GraphMatrix g(4);

    g.addEdge(0, 1, 4);
    g.addEdge(0, 2, 2);
    g.addEdge(2, 3, 3);
    g.addEdge(1, 3, 1);

    vector<long long> dist = g.dijkstra(0);

    for (int v = 0; v < g.V; ++v) {
        cout << "Distance from 0 to " << v << " = ";

        if (dist[v] == INF)
            cout << "Unreachable\n";
        else
            cout << dist[v] << '\n';
    }

    return 0;
}
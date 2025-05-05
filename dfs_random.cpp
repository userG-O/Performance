#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

// Graph class representing the adjacency list
class Graph {
    int V;  // Number of vertices
    vector<vector<int>> adj;  // Adjacency list

public:
    Graph(int V) : V(V), adj(V) {}

    void addEdge(int v, int w) {
        adj[v].push_back(w);
        adj[w].push_back(v); // For undirected graph
    }

    void parallelDFS(int startVertex) {
        vector<bool> visited(V, false);

        #pragma omp parallel
        {
            #pragma omp single
            {
                parallelDFSUtil(startVertex, visited);
            }
        }
    }

    void parallelDFSUtil(int v, vector<bool>& visited) {
        #pragma omp critical
        {
            if (visited[v])
                return;
            visited[v] = true;
            cout << v << " ";
        }

        #pragma omp parallel for shared(visited)
        for (size_t i = 0; i < adj[v].size(); ++i) {
            int n = adj[v][i];
            if (!visited[n]) {
                #pragma omp task
                parallelDFSUtil(n, visited);
            }
        }
    }
};

int main() {
    srand(time(0)); // Seed RNG

    int V;
    cout << "Enter the number of vertices: ";
    cin >> V;

    Graph g(V + 1); // 1-based indexing

    // Generate a random number of edges (avoid too sparse or too dense)
    int maxEdges = V * (V - 1) / 2;
    int E = rand() % (maxEdges / 2) + V; // Ensure enough edges

    // cout << "Number of processors available: " << omp_get_num_procs() << endl;
    cout << "Randomly generating " << E << " edges..." << endl;

    vector<vector<bool>> edgeExists(V + 1, vector<bool>(V + 1, false));
    int added = 0;

    while (added < E) {
        int u = rand() % V + 1; // 1-based
        int v = rand() % V + 1;

        if (u != v && !edgeExists[u][v]) {
            g.addEdge(u, v);
            edgeExists[u][v] = edgeExists[v][u] = true;
            added++;
        }
    }

    int start = rand() % V + 1; // random starting node from 1 to V
    cout << "Randomly chosen starting vertex for DFS: " << start << endl;

    cout << "Parallel Depth-First Search (DFS): ";
    g.parallelDFS(start);
    cout << endl;

    return 0;
}

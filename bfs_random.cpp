#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int V) : V(V), adj(V) {}

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // for undirected graph
    }

    void parallelBFS(int startVertex) {
        vector<bool> visited(V, false);
        vector<int> currentFrontier;
        vector<int> nextFrontier;

        visited[startVertex] = true;
        currentFrontier.push_back(startVertex);

        while (!currentFrontier.empty()) {
            #pragma omp parallel for
            for (int i = 0; i < currentFrontier.size(); ++i) {
                int v = currentFrontier[i];

                #pragma omp critical
                {
                    cout << v << " ";
                }

                for (int u : adj[v]) {
                    bool needVisit = false;

                    #pragma omp critical
                    {
                        if (!visited[u]) {
                            visited[u] = true;
                            needVisit = true;
                        }
                    }

                    if (needVisit) {
                        #pragma omp critical
                        {
                            nextFrontier.push_back(u);
                        }
                    }
                }
            }

            currentFrontier = nextFrontier;
            nextFrontier.clear();
        }

        cout << endl;
    }
};

int main() {
    int n;
    cout << "Enter the number of vertices (random graph): ";
    cin >> n;

    srand(time(0)); // Seed the random number generator

    Graph g(n);

    // Randomly generate edges (ensure no self-loops or duplicate edges)
    int maxEdges = n * (n - 1) / 2; // Max possible edges in undirected graph
    int numEdges = rand() % maxEdges + 1; // At least one edge

    cout << "Randomly generating " << numEdges << " edges..." << endl;

    vector<vector<bool>> edgeExists(n, vector<bool>(n, false));
    int edgeCount = 0;

    while (edgeCount < numEdges) {
        int u = rand() % n;
        int v = rand() % n;

        if (u != v && !edgeExists[u][v]) {
            g.addEdge(u, v);
            edgeExists[u][v] = edgeExists[v][u] = true;
            edgeCount++;
        }
    }

    int start = rand() % n;
    cout << "Randomly chosen starting vertex for BFS: " << start << endl;

    cout << "Parallel Breadth-First Search (BFS): ";
    g.parallelBFS(start);

    return 0;
}

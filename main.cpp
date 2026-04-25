// Topological Sort on a DAG
// Created By Christopher Sparks
// Class: CS 236
// This program implements a topological sort on a directed acyclic graph (DAG) using Recursive Backtracking.
// It reads the number of vertices and edges from the user, constructs the graph, and then performs the topological sort.	
// This program can also be modified to include weighted edges, but the current implementation does not use weights.

#include <iostream>
#include "Graph.h"
#include "TopologicalSort.h"

using namespace std;

int main() {
    Graph g;

    Vertex* v0 = g.AddVertex(0);
    Vertex* v1 = g.AddVertex(1);
    Vertex* v2 = g.AddVertex(2);
    Vertex* v3 = g.AddVertex(3);

    g.AddDirectedEdge(v0, v1, 4);
    g.AddDirectedEdge(v0, v2, 1);
    g.AddDirectedEdge(v2, v1, 2);
    g.AddDirectedEdge(v1, v3, 1);
    g.AddDirectedEdge(v2, v3, 5);

    cout << "\n--- GRAPH DEMO ---\n";

    cout << "\nBFS:\n";
    g.BFS(v0);

    cout << "\nDFS:\n";
    g.DFS(v0);

    cout << "\nCycle Detection:\n";
    g.DetectCycle();

    cout << "\nDijkstra:\n";
    if (g.Dijkstra(v0)) {
        g.PrintPath(v0, v3);
    }

    cout << "\nBellman-Ford:\n";
    if (g.BellmanFord(v0)) {
        g.PrintShortestPaths(v0);
    }
    cout << "\n--- Topological Sort ---\n";
    vector<Vertex*> topo = g.TopologicalSort();

    for (Vertex* v : topo) {
        cout << v->label << " ";
    }
    cout << endl;


    return 0;
}
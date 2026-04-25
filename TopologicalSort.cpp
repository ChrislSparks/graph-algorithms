#include "TopologicalSort.h"
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>

using namespace std;

// Recursive utility for backtracking topological sorts
void TopologicalSortUtil(Graph& graph, vector<Vertex*>& result, unordered_map<Vertex*, int>& inDegree, unordered_set<Vertex*>& visited) {
    bool flag = false;

    // Look for vertices with zero in-degree that are not visited yet
    for (Vertex* v : graph.GetVertices()) {
        // If the vertex has an in-degree of 0 and has not been visited yet
        if (inDegree[v] == 0 && visited.find(v) == visited.end()) {
            // Include the vertex in the result
            result.push_back(v);
            visited.insert(v);

            // Decrease the in-degree of neighbors
            for (Edge* e : *graph.GetEdgesFrom(v)) {
                inDegree[e->toVertex]--;
            }

            // Recurse to find other possible topological sorts
            TopologicalSortUtil(graph, result, inDegree, visited);

            // Backtrack: Remove the last vertex and restore in-degrees
            result.pop_back();
            visited.erase(v);

            // Restore in-degree for neighbors
            for (Edge* e : *graph.GetEdgesFrom(v)) {
                inDegree[e->toVertex]++;
            }

            flag = true;
        }
    }

    // If no more vertices to visit, print the current result (valid topological order)
    if (!flag) {
        for (Vertex* v : result) {
            cout << v->label << " ";
        }
        cout << endl;
    }
}

// Kick-off function for topological sorts
void AllTopologicalSorts(Graph& graph) {
    unordered_map<Vertex*, int> inDegree;

    // Initialize the in-degree map (each vertex initially has an in-degree of 0)
    for (Vertex* v : graph.GetVertices()) {
        inDegree[v] = 0;
    }

    // Compute the in-degree of each vertex by iterating over all edges
    for (Vertex* v : graph.GetVertices()) {
        for (Edge* e : *graph.GetEdgesFrom(v)) {
            inDegree[e->toVertex]++;
        }
    }

    // Vector to store the current topological sort
    vector<Vertex*> result;

    // Set to track visited vertices during backtracking
    unordered_set<Vertex*> visited;

    // Start the recursive topological sort
    TopologicalSortUtil(graph, result, inDegree, visited);
}
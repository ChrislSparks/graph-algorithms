#ifndef GRAPH_H
#define GRAPH_H

#include <stack>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "Vertex.h"
#include "Edge.h"

using namespace std;

class Graph {
private:
    // Map each vertex to a dynamic list of outgoing edges (adjacency list)
    unordered_map<Vertex*, vector<Edge*>*> fromEdges;
    void DFSHelper(Vertex* currentV, unordered_map<Vertex*, bool>& visited); 
    void TopoSortHelper(Vertex* v, unordered_map<Vertex*, bool>& visited, stack<Vertex*>& topoStack);

public:
    ~Graph(); // destructor
    Vertex* AddVertex(int label); // adds vertex
    Edge* AddDirectedEdge(Vertex* from, Vertex* to, int weight); //adds direct edge
    vector<Vertex*> GetVertices() const; // get list of vertices
    const vector<Edge*>* GetEdgesFrom(Vertex* from) const; // get outgoing edges
    bool HasEdge(Vertex* from, Vertex* to) const; // check if edge exists
    bool DetectCycle(); // detect if the graph has cycles, if so, print the cycle path
    void AddUndirectedEdge(Vertex* a, Vertex* b, int weight); // adds undirected edges

    vector<Vertex*> TopologicalSort(); // Topological Sort function
    
    bool BellmanFord(Vertex* startV); // BellmanFord
    void PrintShortestPaths(Vertex* startV);

    bool Dijkstra(Vertex* startV); // Dijkstra
    void PrintPath(Vertex* startV, Vertex* endV);

    void BFS(Vertex* startV); // BFS
    void DFS(Vertex* startV); // DFS
};

#endif

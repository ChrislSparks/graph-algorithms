#include <climits>
#include <algorithm>
#include <queue> 
#include <stack>
#include "Graph.h"

// Destructor to clean up dynamic memory
Graph::~Graph() {
    unordered_set<Edge*> allEdges;
    for (auto& pair : fromEdges) {
        for (Edge* edge : *pair.second) {
            allEdges.insert(edge);
        }
        delete pair.second;
        delete pair.first;
    }
    for (Edge* edge : allEdges) {
        delete edge;
    }
}

// Add a new vertex to the graph
Vertex* Graph::AddVertex(int label) {
    Vertex* vertex = new Vertex(label);
    fromEdges[vertex] = new vector<Edge*>();
    return vertex;
}

// Add a directed edge from one vertex to another
Edge* Graph::AddDirectedEdge(Vertex* from, Vertex* to, int weight) {
    if (HasEdge(from, to)) return nullptr;
    Edge* edge = new Edge(from, to, weight);
    fromEdges[from]->push_back(edge);
    return edge;
}

// Return all vertices
vector<Vertex*> Graph::GetVertices() const {
    vector<Vertex*> vertices;
    for (auto& pair : fromEdges) {
        vertices.push_back(pair.first);
    }
    return vertices;
}

// Get outgoing edges from a vertex
const vector<Edge*>* Graph::GetEdgesFrom(Vertex* from) const {
    return fromEdges.at(from);
}

// Check if an edge exists between two vertices
bool Graph::HasEdge(Vertex* from, Vertex* to) const {
    if (fromEdges.count(from) == 0) return false;
    for (Edge* edge : *fromEdges.at(from)) {
        if (edge->toVertex == to) return true;
    }
    return false;
}

// Recursive helper to detect cycles (DFS)
bool DetectCycleUtil(Vertex* v, unordered_map<Vertex*, bool>& visited, unordered_map<Vertex*, bool>& recursionStack,
    const Graph& graph, vector<Vertex*>& cyclePath, unordered_map<Vertex*, Vertex*>& parentMap)
{
    visited[v] = true;
    recursionStack[v] = true;

    // Explore neighbors
    for (Edge* edge : *graph.GetEdgesFrom(v)) {
        Vertex* neighbor = edge->toVertex;

        // If not visited, recurse and set parent
        if (!visited[neighbor]) {
            parentMap[neighbor] = v;  //  Set parent here
            if (DetectCycleUtil(neighbor, visited, recursionStack, graph, cyclePath, parentMap)) {
                return true;
            }
        }
        // If neighbor is in recursion stack, we found a cycle
        else if (recursionStack[neighbor]) {
            // Cycle detected, reconstruct path
            cyclePath.push_back(neighbor);
            Vertex* temp = v;
            while (temp != neighbor) {
                cyclePath.push_back(temp);
                temp = parentMap[temp];
            }
            cyclePath.push_back(neighbor); // close the cycle
            reverse(cyclePath.begin(), cyclePath.end());
            return true;
        }
    }

    recursionStack[v] = false; // Backtrack
    return false;
}

// Detect if graph contains a cycle and print cycle path
bool Graph::DetectCycle() {
    unordered_map<Vertex*, bool> visited;
    unordered_map<Vertex*, bool> recursionStack;
    unordered_map<Vertex*, Vertex*> parentMap;  // To keep track of the parent nodes
    vector<Vertex*> cyclePath;

    // Initialize visited and recursionStack for all vertices
    for (Vertex* v : GetVertices()) {
        visited[v] = false;
        recursionStack[v] = false;
    }

    // Traverse each vertex
    for (Vertex* v : GetVertices()) {
        if (!visited[v]) {
            if (DetectCycleUtil(v, visited, recursionStack, *this, cyclePath, parentMap)) {
                cout << "Graph contains a cycle.\nCycle detected: ";
                for (Vertex* vertex : cyclePath) {
                    cout << vertex->label << " ";
                }
                cout << endl;
                return true;
            }
        }
    }

    // If no cycle was detected after checking all vertices
    cout << "Graph does not contain a cycle." << endl;
    return false; // Return false if no cycle is found
}
bool Graph::BellmanFord(Vertex* startV) {
    vector<Vertex*> vertices = GetVertices();

    // Step 1: Initialize all vertices
    for (Vertex* currentV : vertices) {
        currentV->distance = INT_MAX;
        currentV->predecessor = nullptr;
    }

    startV->distance = 0;

    // Step 2: Relax all edges |V| - 1 times
    for (int i = 1; i <= vertices.size() - 1; i++) {
        for (Vertex* currentV : vertices) {
            for (Edge* edge : *GetEdgesFrom(currentV)) {
                Vertex* adjV = edge->toVertex;
                int edgeWeight = edge->weight;

                if (currentV->distance != INT_MAX &&
                    currentV->distance + edgeWeight < adjV->distance) {

                    adjV->distance = currentV->distance + edgeWeight;
                    adjV->predecessor = currentV;
                }
            }
        }
    }

    // Step 3: Check for negative weight cycle
    for (Vertex* currentV : vertices) {
        for (Edge* edge : *GetEdgesFrom(currentV)) {
            Vertex* adjV = edge->toVertex;
            int edgeWeight = edge->weight;

            if (currentV->distance != INT_MAX &&
                currentV->distance + edgeWeight < adjV->distance) {
                return false;
            }
        }
    }

    return true;
}
void Graph::PrintShortestPaths(Vertex* startV) {
    cout << "Shortest paths from vertex " << startV->label << ":\n";

    for (Vertex* v : GetVertices()) {
        cout << "Vertex " << v->label << ": ";

        if (v->distance == INT_MAX) {
            cout << "unreachable";
        }
        else {
            cout << "distance = " << v->distance;

            if (v->predecessor != nullptr) {
                cout << ", predecessor = " << v->predecessor->label;
            }
            else {
                cout << ", predecessor = none";
            }
        }

        cout << endl;
    }
}
 // Adds Undirected Edges
void Graph::AddUndirectedEdge(Vertex* a, Vertex* b, int weight) { 
    AddDirectedEdge(a, b, weight);
    AddDirectedEdge(b, a, weight);
}

///////// TOPOLOGICAL SORT FUNCTIONS ///////////
void Graph::TopoSortHelper(Vertex* v, unordered_map<Vertex*, bool>& visited, stack<Vertex*>& topoStack)
{
    visited[v] = true;

    for (Edge* edge : *GetEdgesFrom(v)) {
        Vertex* neighbor = edge->toVertex;

        if (!visited[neighbor]) {
            TopoSortHelper(neighbor, visited, topoStack);
        }
    }

    topoStack.push(v); // push AFTER exploring
}
vector<Vertex*> Graph::TopologicalSort() {
    vector<Vertex*> result;

    if (DetectCycle()) {
        cout << "Topological sort not possible (graph has a cycle)\n";
        return result;
    }

    unordered_map<Vertex*, bool> visited;
    stack<Vertex*> topoStack;

    for (Vertex* v : GetVertices()) {
        visited[v] = false;
    }

    for (Vertex* v : GetVertices()) {
        if (!visited[v]) {
            TopoSortHelper(v, visited, topoStack);
        }
    }

    while (!topoStack.empty()) {
        result.push_back(topoStack.top());
        topoStack.pop();
    }

    return result;
}

//////////////Dijkstra/////////////////
bool Graph::Dijkstra(Vertex* startV) {
    vector<Vertex*> vertices = GetVertices();

    for (Vertex* v : vertices) {
        v->distance = INT_MAX;
        v->predecessor = nullptr;
    }

    startV->distance = 0;

    vector<Vertex*> unvisited = vertices;

    while (!unvisited.empty()) {
        int minIndex = 0;

        for (int i = 1; i < unvisited.size(); i++) {
            if (unvisited[i]->distance < unvisited[minIndex]->distance) {
                minIndex = i;
            }
        }

        Vertex* currentV = unvisited[minIndex];
        unvisited.erase(unvisited.begin() + minIndex);

        if (currentV->distance == INT_MAX) {
            continue;
        }

        for (Edge* edge : *GetEdgesFrom(currentV)) {
            Vertex* adjV = edge->toVertex;
            int weight = edge->weight;

            if (weight < 0) {
                cout << "Dijkstra cannot be used with negative edge weights." << endl;
                return false;
            }

            int altDistance = currentV->distance + weight;

            if (altDistance < adjV->distance) {
                adjV->distance = altDistance;
                adjV->predecessor = currentV;
            }
        }
    }

    return true;
}
void Graph::PrintPath(Vertex* startV, Vertex* endV) {
    if (endV->distance == INT_MAX) {
        cout << "No path from " << startV->label
            << " to " << endV->label << endl;
        return;
    }

    vector<Vertex*> path;
    Vertex* currentV = endV;

    while (currentV != nullptr) {
        path.push_back(currentV);
        currentV = currentV->predecessor;
    }

    reverse(path.begin(), path.end());

    cout << "Shortest path from " << startV->label
        << " to " << endV->label << ": ";

    for (int i = 0; i < path.size(); i++) {
        cout << path[i]->label;
        if (i < path.size() - 1) {
            cout << " -> ";
        }
    }

    cout << " | Distance: " << endV->distance << endl;
}
////////////////BFS//////////////////////////
void Graph::BFS(Vertex* startV) {
    unordered_map<Vertex*, bool> visited;
    queue<Vertex*> q;

    for (Vertex* v : GetVertices()) {
        visited[v] = false;
    }

    visited[startV] = true;
    q.push(startV);

    cout << "BFS starting from vertex " << startV->label << ": ";

    while (!q.empty()) {
        Vertex* currentV = q.front();
        q.pop();

        cout << currentV->label << " ";

        for (Edge* edge : *GetEdgesFrom(currentV)) {
            Vertex* neighbor = edge->toVertex;

            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    cout << endl;
}
/////////////////DFS///////////////////
void Graph::DFS(Vertex* startV) {
    unordered_map<Vertex*, bool> visited;

    for (Vertex* v : GetVertices()) {
        visited[v] = false;
    }

    cout << "DFS starting from vertex " << startV->label << ": ";
    DFSHelper(startV, visited);
    cout << endl;
}

void Graph::DFSHelper(Vertex* currentV, unordered_map<Vertex*, bool>& visited) {
    visited[currentV] = true;
    cout << currentV->label << " ";

    for (Edge* edge : *GetEdgesFrom(currentV)) {
        Vertex* neighbor = edge->toVertex;

        if (!visited[neighbor]) {
            DFSHelper(neighbor, visited);
        }
    }
}
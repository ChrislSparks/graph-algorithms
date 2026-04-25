#ifndef EDGE_H
#define EDGE_H

#include "Vertex.h"

class Edge {
public:
    Vertex* fromVertex;
    Vertex* toVertex;
    int weight;

    Edge(Vertex* from, Vertex* to, int w) {
        fromVertex = from;
        toVertex = to;
        weight = w;
    }
};
#endif

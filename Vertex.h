#ifndef VERTEX_H
#define VERTEX_H

#include <string>
#include <iostream>
#include <climits>

class Vertex {
public:
    int label;
    int distance;
    Vertex* predecessor;

    Vertex(const int& vertexLabel) {
        label = vertexLabel;
        distance = INT_MAX;
        predecessor = nullptr;
    }
};

#endif

#pragma once

#include "SharedByAllGreedysUtils.hpp"

class VertexWithOutDegree {
    public:
    Vertex vertex;
    int outdegree;

    VertexWithOutDegree(int x, int y, int outdegree) {
        this->vertex = Vertex(x, y);
        this->outdegree = outdegree;
    }

    bool operator<(const VertexWithOutDegree &other) const {
        if (this->outdegree == other.outdegree) {
            if (this->vertex.x == other.vertex.x) {
                return this->vertex.y > other.vertex.y;
            }
            return this->vertex.x < other.vertex.x;
        }
        return this->outdegree < other.outdegree;
    }
};

#pragma once

#include "SharedByAllUtils.h"

// only used by greedy3.cpp, greedy3v2.cpp and greedy4.cpp but unnecessary to create a file just for this
class PairRectangleDegree {
    public:
    int rectangleID;
    int rectangleDegree;

    PairRectangleDegree(int rectangleID, int rectangleDegree) {
        this->rectangleID = rectangleID;
        this->rectangleDegree = rectangleDegree;
    }

    bool operator <(const PairRectangleDegree &other) const {
        if (this->rectangleDegree == other.rectangleDegree) {
            return this->rectangleID > other.rectangleID;
        }

        return this->rectangleDegree > other.rectangleDegree;
    }
};

void printAllVerticesOutDegrees(const map<Vertex, int>& vertexOutDegree);
void printAllRectanglesBoundaryVertices(const map<int, set<Vertex>>& rectangleBoundVertices);
void printAllRectanglesAtVertices(const map<Vertex, set<int>>& rectanglesAtVertex);
void printAllRectanglesCoveredByCurrentBestVertex(const set<int>& rectanglesCoveredByVertex);
void printAllRectanglesToUpdate(const set<int>& rectanglesToUpdate);
void processCurrentInstanceInputs(istream &inputFile, int& numRectangles, int& numRectanglesToBeCovered, map<int, set<Vertex>>& rectangleBoundaryVertices, map<Vertex, int>& vertexOutDegree, map<Vertex, set<int>>& rectanglesAtVertex);
Vertex rectanglesVertexWithHighestDegree(int rectangleID, const map<int, set<Vertex>>& rectangleBoundaryVertices, const map<Vertex, int>& vertexOutDegree);

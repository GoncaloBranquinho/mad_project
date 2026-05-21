#pragma once

#include "SharedByAllGreedysUtils.h"

// also used by greedy3v2.cpp - unnecessary to create a single file for this
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

void printAllRectanglesDegrees(const map<int, set<int>>& rectangleDegree);
void initializeRectanglesDegreesAndPQ(map<int, set<Vertex>>& rectangleBoundaryVertices, const map<Vertex, set<int>>& rectanglesAtVertex,  const map<Vertex, int>& vertexOutDegree, map<int, set<int>>& rectangleDegree, priority_queue<PairRectangleDegree>& rectanglePriorityQueue);
void calculateWhichRectanglesToUpdate(set<int>& rectanglesToUpdate, const set<int>& rectanglesCoveredByVertex, const map<int, set<Vertex>>& rectangleBoundaryVertices, const map<Vertex, set<int>>& rectanglesAtVertex, const vector<bool>& rectangleCovered);
void updateNecessaryRectanglesDegreesAndPQ(const set<int>& rectanglesToUpdate, const set<int>& rectanglesCoveredByVertex, map<int, set<int>>& rectangleDegree, priority_queue<PairRectangleDegree>& rectanglePriorityQueue);
void printAllRectanglesCoveredByCurrentBestVertex(const set<int>& rectanglesCoveredByVertex);
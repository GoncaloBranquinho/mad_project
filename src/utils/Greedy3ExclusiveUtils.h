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

void printAllRectanglesDegrees(const map<int, int>& rectangleDegree);
void calculateWhichRectanglesToUpdate(set<int>& rectanglesToUpdate, const set<Vertex>& affectedVertices, const map<Vertex, set<int>>& rectanglesAtVertex, const vector<bool>& rectangleCovered);
void initializeRectanglesDegreesAndPQ(map<int, set<Vertex>>& rectangleBoundaryVertices, const map<Vertex, int>& vertexOutDegree, map<int, int>& rectangleDegree, priority_queue<PairRectangleDegree>& rectanglePriorityQueue);
void updateNecessaryRectanglesDegreesAndPQ(const set<int>& rectanglesToUpdate, const map<int, set<Vertex>>& rectangleBoundaryVertices, const map<Vertex, int>& vertexOutDegree, map<int, int>& rectangleDegree, priority_queue<PairRectangleDegree>& rectanglePriorityQueue);
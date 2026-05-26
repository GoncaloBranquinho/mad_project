#pragma once

#include "SharedByAllGreedysUtils.h"

void printAllRectanglesDegrees(const map<int, int>& rectangleDegree);
void calculateWhichRectanglesToUpdate(set<int>& rectanglesToUpdate, const set<Vertex>& affectedVertices, const map<Vertex, set<int>>& rectanglesAtVertex, const map<int, bool>& rectangleCovered);
void initializeRectanglesDegreesAndPQ(map<int, bool>& rectangleCovered, map<int, set<Vertex>>& rectangleBoundaryVertices, const map<Vertex, int>& vertexOutDegree, map<int, int>& rectangleDegree, priority_queue<PairRectangleDegree>& rectanglePriorityQueue);
void updateNecessaryRectanglesDegreesAndPQ(const set<int>& rectanglesToUpdate, const map<int, set<Vertex>>& rectangleBoundaryVertices, const map<Vertex, int>& vertexOutDegree, map<int, int>& rectangleDegree, priority_queue<PairRectangleDegree>& rectanglePriorityQueue);
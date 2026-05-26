#pragma once

#include "SharedByAllGreedysUtils.hpp"

void printAllRectanglesDegrees(const map<int, set<int>>& rectangleDegree);
void initializeRectanglesDegreesAndPQ(map<int, bool>& rectangleCovered, map<int, set<Vertex>>& rectangleBoundaryVertices, const map<Vertex, set<int>>& rectanglesAtVertex,  const map<Vertex, int>& vertexOutDegree, map<int, set<int>>& rectangleDegree, priority_queue<PairRectangleDegree>& rectanglePriorityQueue);
void calculateWhichRectanglesToUpdate(set<int>& rectanglesToUpdate, const set<int>& rectanglesCoveredByVertex, const map<int, set<Vertex>>& rectangleBoundaryVertices, const map<Vertex, set<int>>& rectanglesAtVertex, const map<int, bool>& rectangleCovered);
void updateNecessaryRectanglesDegreesAndPQ(const set<int>& rectanglesToUpdate, const set<int>& rectanglesCoveredByVertex, map<int, set<int>>& rectangleDegree, priority_queue<PairRectangleDegree>& rectanglePriorityQueue);
void printAllRectanglesCoveredByCurrentBestVertex(const set<int>& rectanglesCoveredByVertex);
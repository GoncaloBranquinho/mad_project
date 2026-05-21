#pragma once

#include "SharedByAllUtils.h"

void printAllVerticesOutDegrees(const map<Vertex, int>& vertexOutDegree);
void printAllRectanglesBoundaryVertices(const map<int, set<Vertex>>& rectangleBoundVertices);
void printAllRectanglesAtVertices(const map<Vertex, set<int>>& rectanglesAtVertex);
void printAllRectanglesCoveredByCurrentBestVertex(const set<int>& rectanglesCoveredByVertex);
void printAllRectanglesToUpdate(const set<int>& rectanglesToUpdate);
void processCurrentInstanceInputs(istream &inputFile, int& numRectangles, int& numRectanglesToBeCovered, map<int, set<Vertex>>& rectangleBoundaryVertices, map<Vertex, int>& vertexOutDegree, map<Vertex, set<int>>& rectanglesAtVertex);
Vertex rectanglesVertexWithHighestDegree(int rectangleID, const map<int, set<Vertex>>& rectangleBoundaryVertices, const map<Vertex, int>& vertexOutDegree);

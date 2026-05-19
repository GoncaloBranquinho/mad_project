#pragma once

#include <set>
#include <map>
#include <queue>

using namespace std;

class Vertex {
    public:
    int x;
    int y;

    Vertex() {
        this->x = -1;
        this->y = -1;
    }

    Vertex(int x, int y) {
        this->x = x;
        this->y = y;
    }

    bool operator <(const Vertex &other) const {
        if (this->x != other.x) {
            return this->x < other.x;
        }

        return this->y < other.y;
    }   
};

// shared by more than one .cpp file
void printNumInstancesToConsider(int numInstances);
void printCurrentInstanceNumber(int currentInstance);
void printPercentageOfRectanglesInConsideration(float percentageToCover, int numRectanglesToBeCovered, int numRectangles);
void printPercentageToCoverInputMessage();
float getInputPercentageIfValidOrDefault100();
void printMinimumNumberOfGuardsRequired(int minNumGuardsRequired);
void printAllVerticesOutDegrees(const map<Vertex, int>& vertexOutDegree);
void printAllRectanglesBoundaryVertices(const map<int, set<Vertex>>& rectangleBoundaryVertices);
void printAllRectanglesAtVertices(const map<Vertex, set<int>>& rectanglesAtVertex);
void printAllRandomRectanglesChosenFromPartition(const set<int>& randomlyChosenRectangleIDs);
void printAllRectanglesCoveredByCurrentBestVertex(const set<int>& rectanglesCoveredByVertex);
void printAllRectanglesToUpdate(set<int>& rectanglesToUpdate);
void processInput(istream &inputFile, int numRectangles, const set<int>& randomlyChosenRectangleIDs, map<int, set<Vertex>>& rectangleBoundaryVertices, map<Vertex, int>& vertexOutDegree, map<Vertex, set<int>>& rectanglesAtVertex);
void chooseRandomRectanglesFromPartition(set<int>& randomlyChosenRectangleIDs, int numRectanglesToBeCovered, int numRectangles);

// greedy3.cpp exclusive
void printAllRectanglesDegrees(const map<int, int>& rectangleDegree);

// greedy3v2.cpp exclusive
void printAllRectanglesDegreesV2(const map<int, set<int>>& rectangleDegree);

// or-tools-solver.cpp exclusive
void printVerticesSet(set<Vertex>& verticesSet);
void printIDsToVertices(map<int, Vertex>& idToVertex);
void printVerticesToIds(map<Vertex, int>& vertexToId);
void processInput(istream &inputFile, int numRectangles, const set<int>& randomlyChosenRectangleIDs, map<int, set<Vertex>>& rectangleBoundaryVertices, set<Vertex>& verticesSet);
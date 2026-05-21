#include "Greedy3v2ExclusiveUtils.h"

void printAllRectanglesDegrees(const map<int, set<int>>& rectangleDegree) {
    print("\n\tDegree associated to each rectangle (number of different rectangles it is in contact with / shares vertices with)\n");
    print("\trectangleDegree = {{\n");

    for (const auto& entry : rectangleDegree) {
        const auto& rectangleID = entry.first;
        const auto& adjacentRectanglesSet = entry.second;

        bool printComma = false;
        print("\t   {}: ({}) {{", rectangleID, adjacentRectanglesSet.size());

        for (const auto& rectangleID : adjacentRectanglesSet) {
            if (printComma) {
                print(", ");
            } else {
                printComma = true;
            }

            print("{}", rectangleID);
        }

        print("}},\n");
    }
    
    print("\t}}\n\n");
}

void initializeRectanglesDegreesAndPQ(map<int, set<Vertex>>& rectangleBoundaryVertices, const map<Vertex, set<int>>& rectanglesAtVertex,  const map<Vertex, int>& vertexOutDegree, map<int, set<int>>& rectangleDegree, priority_queue<PairRectangleDegree>& rectanglePriorityQueue) {
    for (const auto& entry : rectangleBoundaryVertices) {
        const auto& rectangleID = entry.first;
        const auto& verticesSet = entry.second;
        rectangleDegree[rectangleID]; // empty entry so that isolated rectangles (degree = 0) enter the priority queue

        for (const auto& vertex : verticesSet) {
            for (const auto& rectID : rectanglesAtVertex.at(vertex)) {
                if (rectID != rectangleID) {
                    rectangleDegree[rectID].insert(rectangleID);
                    rectangleDegree[rectangleID].insert(rectID);
                }
            }
        }
    }

    for (const auto& entry: rectangleDegree) {
        const auto& rectangleID = entry.first;
        const auto& numAdjacentRectangles = entry.second.size();
        rectanglePriorityQueue.push(PairRectangleDegree(rectangleID, numAdjacentRectangles));
        // print("\tInitiliazlized rectangle {}'s degree to {}\n", rectangleID, numAdjacentRectangles);
    }
}

void calculateWhichRectanglesToUpdate(set<int>& rectanglesToUpdate, const set<int>& rectanglesCoveredByVertex, const map<int, set<Vertex>>& rectangleBoundaryVertices, const map<Vertex, set<int>>& rectanglesAtVertex, const vector<bool>& rectangleCovered) {
    for (const auto& rectangleID: rectanglesCoveredByVertex) {
        for (const auto& vertex: rectangleBoundaryVertices.at(rectangleID)) {
            for (const auto& rectID : rectanglesAtVertex.at(vertex)) {
                if (!rectangleCovered.at(rectID)) {
                    rectanglesToUpdate.insert(rectID);
                }
            }
        }   
    }
}
   
void updateNecessaryRectanglesDegreesAndPQ(const set<int>& rectanglesToUpdate, const set<int>& rectanglesCoveredByVertex, map<int, set<int>>& rectangleDegree, priority_queue<PairRectangleDegree>& rectanglePriorityQueue) {
    if (rectanglesToUpdate.size() != 0) {
        for (const auto& rectangleIDCoveredByVertex : rectanglesCoveredByVertex) {
            for (const auto& rectangleID : rectanglesToUpdate) {
                if (rectangleDegree.contains(rectangleID)) {
                    rectangleDegree[rectangleID].erase(rectangleIDCoveredByVertex);
                }
            }
        }

        for (const auto& rectangleID : rectanglesToUpdate) {
            int newDegree = rectangleDegree[rectangleID].size();
            rectanglePriorityQueue.push(PairRectangleDegree(rectangleID, newDegree));
        }
    }
}
#include "Greedy3NecessaryUtils.h"

void printAllRectanglesDegrees(const map<int, int>& rectangleDegree) { 
    print("\n\tDegree associated to each rectangle (sum of the degrees of the vertices it SHARES i.e. ≠ 1)\n");
    print("\trectangleDegree = {{\n");
    bool printComma = false;

    for (const auto& entry : rectangleDegree) {
        const auto& rectangleID = entry.first;
        const auto& degree = entry.second;

        if (printComma) {
            print(",\t\n");
        } else {
            printComma = true;
        }

        print("\t   {}: {}", rectangleID, degree);
    }
    
    print("\n\t}}\n\n");
}

void calculateWhichRectanglesToUpdate(set<int>& rectanglesToUpdate, const set<Vertex>& affectedVertices, const map<Vertex, set<int>>& rectanglesAtVertex, const map<int, bool>& rectangleCovered) {
    for (const auto& vertex: affectedVertices) {
        for (const auto& rectangleID : rectanglesAtVertex.at(vertex)) {
            if (!rectangleCovered.at(rectangleID)) {
                rectanglesToUpdate.insert(rectangleID);
            }
        }
    }
}

void initializeRectanglesDegreesAndPQ(map<int, bool>& rectangleCovered, map<int, set<Vertex>>& rectangleBoundaryVertices, const map<Vertex, int>& vertexOutDegree, map<int, int>& rectangleDegree, priority_queue<PairRectangleDegree>& rectanglePriorityQueue) {
    for (const auto& entry : rectangleBoundaryVertices) {
        const auto& rectangleID = entry.first;
        const auto& verticesSet = entry.second;

        int degree = 0;

        for (const auto& vertexCoords : verticesSet) {
            if (vertexOutDegree.at(vertexCoords) > 1) {
                degree += vertexOutDegree.at(vertexCoords);
            }
        }

        rectangleDegree[rectangleID] = degree;
        rectanglePriorityQueue.push(PairRectangleDegree(rectangleID, degree));
        rectangleCovered[rectangleID] = false;
        // print("\tInitiliazlized rectangle {}'s degree to {}\n", rectangleID, degree);
    }
}

void updateNecessaryRectanglesDegreesAndPQ(const set<int>& rectanglesToUpdate, const map<int, set<Vertex>>& rectangleBoundaryVertices, const map<Vertex, int>& vertexOutDegree, map<int, int>& rectangleDegree, priority_queue<PairRectangleDegree>& rectanglePriorityQueue) {

    if (rectanglesToUpdate.size() != 0) {
        for (int rectangleID : rectanglesToUpdate) {
            int newDegree = 0;
            const auto& verticesSet = rectangleBoundaryVertices.at(rectangleID);

            for (const auto& vertex: verticesSet) {
                if (vertexOutDegree.at(vertex) > 1) {
                    newDegree += vertexOutDegree.at(vertex);
                }
            }

            const auto& oldDegree = rectangleDegree.at(rectangleID);
            // print("\n    Updated rectangle {}'s degree from {} to {}", rectangleID, oldDegree, newDegree);
            rectangleDegree.at(rectangleID) = newDegree;
            rectanglePriorityQueue.push(PairRectangleDegree(rectangleID, newDegree));
        }
    }
    // print("\n");
}
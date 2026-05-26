#include "Greedy4NecessaryUtils.h"

void printPQ(priority_queue<PairRectangleDegree> rectanglePriorityQueue) {
    int counter = 1;
    print("\n\tCurrent PQ state\n");
    while (!rectanglePriorityQueue.empty()) {
        const auto pairRectangleDegree = rectanglePriorityQueue.top(); rectanglePriorityQueue.pop();
        const auto rectangleID = pairRectangleDegree.rectangleID;
        const auto rectangleDegree = pairRectangleDegree.rectangleDegree;

        print("\t\t{}. ({}, {})\n", counter++, rectangleID, rectangleDegree);
    }
    print("\n");
}

void initializePQ(const Graph& graph, priority_queue<PairRectangleDegree>& rectanglePriorityQueue) {
    for (const auto& entry: graph.nodes) {
        const auto& node = entry.second;
        const auto& nodeID = entry.first;
        const auto& nodeOutDegree = node.outDegree;

        if (nodeID != -1) {
            rectanglePriorityQueue.push(PairRectangleDegree(nodeID, nodeOutDegree));
        }
    }
}

void updateNecessaryRectanglesDegreesAndPQ(Graph& graph, const set<int>& rectanglesCoveredByVertex, priority_queue<PairRectangleDegree>& rectanglePriorityQueue) {
    if (rectanglesCoveredByVertex.size() != 0) {

        map<int, int> rectangleIDsNotCoveredByVertexDegrees;

        for (const auto& rectangleIDCoveredByVertex : rectanglesCoveredByVertex) {
            const auto rectangleIDCoveredByVertexAdjacentRectanglesSet = graph.nodes.at(rectangleIDCoveredByVertex).adjacentRectangleIDs;

            for (const auto& rectangleID : rectangleIDCoveredByVertexAdjacentRectanglesSet) {
                if (!rectanglesCoveredByVertex.contains(rectangleID)) { // do not remove edges between nodes that are covered by the vertex in question
                    rectangleIDsNotCoveredByVertexDegrees[rectangleID] = graph.nodes.at(rectangleID).outDegree;
                    graph.removeBidirectonalEdge(rectangleIDCoveredByVertex, rectangleID);
                }
            }
        }

        for (const auto& entry : rectangleIDsNotCoveredByVertexDegrees) {
            const auto& updatedRectangleID = entry.first;
            const auto& updatedRectangleIDNewegree = graph.nodes.at(updatedRectangleID).outDegree;
            rectanglePriorityQueue.push(PairRectangleDegree(updatedRectangleID, updatedRectangleIDNewegree));
        }
    }
}

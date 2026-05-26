#pragma once

#include "SharedByAllGreedysUtils.hpp"

class Node {
    public:
    int rectangleID;
    int outDegree = 0;
    bool visited = false;
    set<int> adjacentRectangleIDs;

    Node() {
        this->rectangleID = -1;
    }

    Node(int rectangleID) {
        this->rectangleID = rectangleID;
    }

    void addUnidirectionalEdge(int otherRectangleID) {
        if (!this->adjacentRectangleIDs.contains(otherRectangleID)) {
            this->adjacentRectangleIDs.insert(otherRectangleID);
            this->outDegree++;
        }
    }

    void removeUnidirectionalEdge(int otherRectangleID) {
        if (this->adjacentRectangleIDs.contains(otherRectangleID)) {
            this->adjacentRectangleIDs.erase(otherRectangleID);
            this->outDegree--;
        }
    }
};

// class Graph {
// public:
//     vector<Node> nodes;

//     Graph(int size) {
//         nodes.resize(size + 1); // 1 indexed
//     }

//     void createNodeIfNecessary(int rectangleID) {
//         if (nodes.at(rectangleID).rectangleID == -1) {
//             nodes.at(rectangleID) = Node(rectangleID);
//         }
//     }

//     void addBidirectonalEdge(int rectangleID, int otherRectangleID) {
//         this->createNodeIfNecessary(rectangleID);
//         this->createNodeIfNecessary(otherRectangleID);
        
//         if (rectangleID != otherRectangleID) {
//             nodes.at(rectangleID).addUnidirectionalEdge(otherRectangleID);
//             nodes.at(otherRectangleID).addUnidirectionalEdge(rectangleID);
//         }
//     }

//     void removeBidirectonalEdge(int rectangleID, int otherRectangleID) {
//         nodes.at(rectangleID).removeUnidirectionalEdge(otherRectangleID);
//         nodes.at(otherRectangleID).removeUnidirectionalEdge(rectangleID);
//     }

//     void initialize(const map<int, set<Vertex>>& rectangleBoundaryVertices, const map<Vertex, set<int>>& rectanglesAtVertex) {
//         for (const auto& entry : rectangleBoundaryVertices) {
//             const auto& rectangleID = entry.first;
//             const auto& verticesSet = entry.second;
//             for (const auto& vertex : verticesSet) {
//                 for (const auto& rectID : rectanglesAtVertex.at(vertex)) {
//                     this->addBidirectonalEdge(rectangleID, rectID);
//                 }
//             }
//         }
//     }

//    void printGraph() {
//         print("\n\tCurrent graph state\n");

//         for (const auto& node : nodes) {
//             const auto& nodeID = node.rectangleID;
//             const auto& nodeOutDegree = node.outDegree;

//             print("\t{}: ({}) {{", nodeID, nodeOutDegree);
//             bool printComma = false;

//             for (const auto& otherRectangleID : node.adjacentRectangleIDs) {
//                 if (printComma) {
//                     print(", ");
//                 } else {
//                     printComma = true;
//                 }

//                 print("{}", otherRectangleID);
//             }

//             print("}}\n");
//         }
//     }

// };

class Graph {
public:
    map<int, Node> nodes;

    Graph() { }

    void createNodeIfNecessary(int rectangleID) {
        if (nodes[rectangleID].rectangleID == -1) {
            nodes[rectangleID] = Node(rectangleID);
        }
    }

    void addBidirectonalEdge(int rectangleID, int otherRectangleID) {
        this->createNodeIfNecessary(rectangleID);
        this->createNodeIfNecessary(otherRectangleID);
        
        if (rectangleID != otherRectangleID) {
            nodes.at(rectangleID).addUnidirectionalEdge(otherRectangleID);
            nodes.at(otherRectangleID).addUnidirectionalEdge(rectangleID);
        }
    }

    void removeBidirectonalEdge(int rectangleID, int otherRectangleID) {
        nodes.at(rectangleID).removeUnidirectionalEdge(otherRectangleID);
        nodes.at(otherRectangleID).removeUnidirectionalEdge(rectangleID);
    }

    void initialize(const map<int, set<Vertex>>& rectangleBoundaryVertices, const map<Vertex, set<int>>& rectanglesAtVertex) {
        for (const auto& entry : rectangleBoundaryVertices) {
            const auto& rectangleID = entry.first;
            const auto& verticesSet = entry.second;
            for (const auto& vertex : verticesSet) {
                for (const auto& rectID : rectanglesAtVertex.at(vertex)) {
                    this->addBidirectonalEdge(rectangleID, rectID);
                }
            }
        }
    }

   void printGraph() {
        print("\n\tCurrent graph state\n");

        for (const auto& entry : nodes) {
            const auto& node = entry.second;
            const auto& nodeID = entry.first;
            const auto& rectangleID = node.rectangleID;
            const auto& nodeOutDegree = node.outDegree;

            print("\t{}: ({}) {{", nodeID, nodeOutDegree);
            bool printComma = false;

            for (const auto& otherRectangleID : node.adjacentRectangleIDs) {
                if (printComma) {
                    print(", ");
                } else {
                    printComma = true;
                }

                print("{}", otherRectangleID);
            }

            print("}}\n");
        }
    }

};

void printPQ(priority_queue<PairRectangleDegree> rectanglePriorityQueue);
void initializePQ(const Graph& graph, priority_queue<PairRectangleDegree>& rectanglePriorityQueue);
void updateNecessaryRectanglesDegreesAndPQ(Graph& graph, const set<int>& rectanglesCoveredByVertex, priority_queue<PairRectangleDegree>& rectanglePriorityQueue);

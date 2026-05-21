#include "../utils/SharedByAllGreedysUtils.h"

class Node {
    public:
    int ID;
    int outDegree = 0;
    bool visited = false;
    set<int> adjacentRectangleIDs;

    Node() {
        this->ID = -1;
    }

    Node(int ID) {
        this->ID = ID;
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

class Graph {
public:
    map<int, Node> nodes;

    void createNodeIfNecessary(int rectangleID) {
        if (!nodes.contains(rectangleID)) {
            nodes[rectangleID] = Node(rectangleID);
        }
    }

    void addBidirectonalEdge(int rectangleID, int otherRectangleID) {
        this->createNodeIfNecessary(rectangleID);
        this->createNodeIfNecessary(otherRectangleID);
        
        if (rectangleID != otherRectangleID) {
            nodes[rectangleID].addUnidirectionalEdge(otherRectangleID);
            nodes[otherRectangleID].addUnidirectionalEdge(rectangleID);
        }
    }

    void removeBidirectonalEdge(int rectangleID, int otherRectangleID) {
        // print("\t\t\tRemoving edge {} <-> {}\n", rectangleID, otherRectangleID);
        nodes[rectangleID].removeUnidirectionalEdge(otherRectangleID);
        nodes[otherRectangleID].removeUnidirectionalEdge(rectangleID);
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
            const auto& rectangleID = entry.first;
            const auto& node = entry.second;

            print("\t{}: ({}) {{", rectangleID, node.outDegree);
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
        const auto& rectangleID = entry.first;
        const auto& node = entry.second;

        rectanglePriorityQueue.push(PairRectangleDegree(rectangleID, node.outDegree));
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

            const auto& updatedRectangleIDOldegree = entry.second;
        }
    }
}

void solveAllInstances(istream &inputFile) {
    int numInstances;
    inputFile >> numInstances;
    printNumInstancesToConsider(numInstances);

    string fileteredPartitionsOutputFileName;
    int numInstancesAddedToOutputFile = 0;

    for (int currentInstance = 1; currentInstance <= numInstances; currentInstance++) {
        printCurrentInstanceNumber(currentInstance);

        int numRectangles;
        int numRectanglesToBeCovered;
        map<Vertex, int> vertexOutDegree;
        map<int, set<Vertex>> rectangleBoundaryVertices;
        map<Vertex, set<int>> rectanglesAtVertex;
        priority_queue<PairRectangleDegree> rectanglePriorityQueue;

        processCurrentInstanceInputs(inputFile, numRectangles, numRectanglesToBeCovered, rectangleBoundaryVertices, vertexOutDegree, rectanglesAtVertex);

        int coveredRectangles = 0;
        int minNumGuardsRequired = 0;
        vector<bool> rectangleCovered(numRectangles + 1);   

        Graph graph = Graph();
        graph.initialize(rectangleBoundaryVertices, rectanglesAtVertex);
        initializePQ(graph, rectanglePriorityQueue);

        while (coveredRectangles != numRectanglesToBeCovered) {            
            auto pairRectangleDegree = rectanglePriorityQueue.top(); rectanglePriorityQueue.pop();
            const auto& currRectangleID = pairRectangleDegree.rectangleID;
            // print("\tMost isolated rectangle: {}\n", currRectangleID);

            if (!rectangleCovered.at(currRectangleID)) {            
                // 0. Given the "most isolated" rectangle of the uncovered ones
                // 1. Find its best vertex (highest outDegree of all)
                auto bestVertex = rectanglesVertexWithHighestDegree(currRectangleID, rectangleBoundaryVertices, vertexOutDegree);

                // 2. Set surrounding uncovered rectangles as covered and update remaining vertices' outDegree
                set<int> rectanglesCoveredByVertex;

                for (const auto& rectangleID : rectanglesAtVertex.at(bestVertex)) {
                    if (!rectangleCovered.at(rectangleID)) {
                        rectangleCovered.at(rectangleID) = true;
                        rectanglesCoveredByVertex.insert(rectangleID);
                        coveredRectangles++;

                        for (const auto& vertex : rectangleBoundaryVertices.at(rectangleID)) {
                            vertexOutDegree.at(vertex) -= 1;
                        }
                    }
                }

                // 3. Update remaining necessary uncovered rectangle's degrees
                updateNecessaryRectanglesDegreesAndPQ(graph, rectanglesCoveredByVertex, rectanglePriorityQueue);

                // 4. Officially place a guard in that vertex
                minNumGuardsRequired++;
                // print("\tPlacing guard in ({}, {})\n", bestVertex.first, bestVertex.second);
            }
        }
        printMinimumNumberOfGuardsRequired(minNumGuardsRequired);
        savePartitionToOutputFile(numInstancesAddedToOutputFile, fileteredPartitionsOutputFileName, rectangleBoundaryVertices);
    }
    insertAtBegginingNumInstancesAddedToOutputFile(numInstancesAddedToOutputFile, fileteredPartitionsOutputFileName);
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        print("Insufficient arguments\n");
        return 1;
    }

    ifstream inputFile(argv[1]);

    if (!inputFile.is_open()) {
        print("Unable to open file {}\n", argv[1]);
        return 1;
    }

    solveAllInstances(inputFile);

    inputFile.close();
}

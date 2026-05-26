#include "../utils/Greedy4NecessaryUtils.hpp"

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

        Graph graph = Graph();
        graph.initialize(rectangleBoundaryVertices, rectanglesAtVertex);
        initializePQ(graph, rectanglePriorityQueue);

        while (coveredRectangles != numRectanglesToBeCovered) {            
            auto pairRectangleDegree = rectanglePriorityQueue.top(); rectanglePriorityQueue.pop();
            const auto& currRectangleID = pairRectangleDegree.rectangleID;
            // print("\tMost isolated rectangle: {}\n", currRectangleID);

            if (!graph.nodes.at(currRectangleID).visited) {            
                // 0. Given the "most isolated" rectangle of the uncovered ones
                // 1. Find its best vertex (highest outDegree of all)
                auto bestVertex = rectanglesVertexWithHighestDegree(currRectangleID, rectangleBoundaryVertices, vertexOutDegree);

                // 2. Set surrounding uncovered rectangles as covered and update remaining vertices' outDegree
                set<int> rectanglesCoveredByVertex;

                for (const auto& rectangleID : rectanglesAtVertex.at(bestVertex)) {
                    if (!graph.nodes.at(rectangleID).visited) {
                        graph.nodes.at(rectangleID).visited = true;
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
                // print("\tPlacing guard in ({}, {})\n", bestVertex.x, bestVertex.y);
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

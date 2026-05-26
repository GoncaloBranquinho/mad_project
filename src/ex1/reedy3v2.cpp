#include "../utils/Greedy3v2NecessaryUtils.hpp"

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
        map<int, set<int>> rectangleDegree;
        map<int, bool> rectangleCovered;
        priority_queue<PairRectangleDegree> rectanglePriorityQueue;

        processCurrentInstanceInputs(inputFile, numRectangles, numRectanglesToBeCovered, rectangleBoundaryVertices, vertexOutDegree, rectanglesAtVertex);
        initializeRectanglesDegreesAndPQ(rectangleCovered, rectangleBoundaryVertices, rectanglesAtVertex,  vertexOutDegree, rectangleDegree, rectanglePriorityQueue);

        int coveredRectangles = 0;
        int minNumGuardsRequired = 0;

        while (coveredRectangles != numRectanglesToBeCovered) {
            auto pairRectangleDegree = rectanglePriorityQueue.top(); rectanglePriorityQueue.pop();
            const auto& currRectangleID = pairRectangleDegree.rectangleID;

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

                        for (const auto& vertex: rectangleBoundaryVertices.at(rectangleID)) {
                            vertexOutDegree.at(vertex) -= 1;
                        }

                        rectangleDegree[rectangleID].clear();
                    }
                }

                // 3. Only consider the rectangles whose degree is affected (are bound to the vertices whose degree was decremented)
                set<int> rectanglesToUpdate;
                calculateWhichRectanglesToUpdate(rectanglesToUpdate, rectanglesCoveredByVertex, rectangleBoundaryVertices, rectanglesAtVertex, rectangleCovered);

                
                // 4. Update remaining necessary uncovered rectangle's degrees
                updateNecessaryRectanglesDegreesAndPQ(rectanglesToUpdate, rectanglesCoveredByVertex, rectangleDegree, rectanglePriorityQueue);


                // 5. Officially place a guard in that vertex
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

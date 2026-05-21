#include "../utils/utils.h"

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
        map<int, int> rectangleDegree;
        priority_queue<PairRectangleDegree> rectanglePriorityQueue;
        
        processCurrentInstanceInputs(inputFile, numRectangles, numRectanglesToBeCovered, rectangleBoundaryVertices, vertexOutDegree, rectanglesAtVertex);
        initializeRectanglesDegreesAndPQ(rectangleBoundaryVertices, vertexOutDegree, rectangleDegree, rectanglePriorityQueue);
        
        int coveredRectangles = 0;
        int minNumGuardsRequired = 0;
        vector<bool> rectangleCovered(numRectangles + 1);

        while (coveredRectangles != numRectanglesToBeCovered) {
            auto pairRectangleDegree = rectanglePriorityQueue.top(); rectanglePriorityQueue.pop();
            const auto& currRectangleID = pairRectangleDegree.rectangleID;

            if (!rectangleCovered.at(currRectangleID)) {
                // 0. Given the "most isolated" rectangle of the uncovered ones
                // 1. Find its best vertex (highest outDegree of all)
                auto bestVertex = rectanglesVertexWithHighestDegree(currRectangleID, rectangleBoundaryVertices, vertexOutDegree);

                // 2. Set surrounding uncovered rectangles as covered and update remaining vertices' outDegree
                set<Vertex> affectedVertices;

                for (const auto& rectangleID : rectanglesAtVertex.at(bestVertex)) {
                    if (!rectangleCovered.at(rectangleID)) {
                        rectangleCovered.at(rectangleID) = true;
                        coveredRectangles++;

                        for (const auto& vertex: rectangleBoundaryVertices.at(rectangleID)) {
                            vertexOutDegree.at(vertex) -= 1;
                            affectedVertices.insert(vertex);                   
                        }   
                    }
                }
                
                // 3. Only consider the rectangles whose degree is affected (are bound to the vertices whose degree was decremented)
                set<int> rectanglesToUpdate;
                calculateWhichRectanglesToUpdate(rectanglesToUpdate, affectedVertices, rectanglesAtVertex, rectangleCovered);

                
                // 4. Update remaining necessary uncovered rectangle's degrees
                updateNecessaryRectanglesDegreesAndPQ(rectanglesToUpdate, rectangleBoundaryVertices, vertexOutDegree, rectangleDegree, rectanglePriorityQueue);
                

                // 5. Officially place a guard in that vertex
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
#include "../utils/utils.h"

class PairRectangleDegree {
    public:
    int rectangleID;
    int rectangleDegree;

    PairRectangleDegree(int rectangleID, int rectangleDegree) {
        this->rectangleID = rectangleID;
        this->rectangleDegree = rectangleDegree;
    }

    bool operator <(const PairRectangleDegree &other) const {
        if (this->rectangleDegree == other.rectangleDegree) {
            return this->rectangleID > other.rectangleID;
        }

        return this->rectangleDegree > other.rectangleDegree;
    }
};

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

Vertex rectanglesVertexWithHighestDegree(int rectangleID, const map<int, set<Vertex>>& rectangleBoundaryVertices, const map<Vertex, int>& vertexOutDegree) {

    // print("\n    Most \"isolated\" rectangle: {} -> {{", rectangleID);
    Vertex bestVertex = Vertex(-1, -1);
    int bestVertexDegree = 0;
    bool printComma = false;

    for (const auto& vertex : rectangleBoundaryVertices.at(rectangleID)) {
        // if (printComma) {
        //     print(", ");
        // } else {
        //     printComma = true;
        // }

        // print("(({}, {}), {})", vertex.first, vertex.second, vertexOutDegree.at(vertex));

        if (vertexOutDegree.at(vertex) > bestVertexDegree) {
            bestVertexDegree = vertexOutDegree.at(vertex);
            bestVertex = vertex;
        }
    }

    // print("}}\n");
    // print("\tThe rectangle's vertex with highest outDegree is ({}, {}) with outDegree {}\n", bestVertex.first, bestVertex.second, bestVertexDegree);

    return bestVertex;
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


void solve(istream &inputFile, ofstream& outputFile, float percentageToCover) {
    set<int> randomlyChosenRectangleIDs;
    map<Vertex, int> vertexOutDegree;
    map<int, set<Vertex>> rectangleBoundaryVertices;
    map<Vertex, set<int>> rectanglesAtVertex;
    map<int, set<int>> rectangleDegree;
    priority_queue<PairRectangleDegree> rectanglePriorityQueue;

    int numRectangles;
    inputFile >> numRectangles;

    int numRectanglesToBeCovered = lround(numRectangles * percentageToCover / 100.0);
    printPercentageOfRectanglesInConsideration(percentageToCover, numRectanglesToBeCovered, numRectangles);
    outputFile << numRectanglesToBeCovered << "\n";
    
    chooseRandomRectanglesFromPartition(randomlyChosenRectangleIDs, numRectanglesToBeCovered, numRectangles);
    if (numRectanglesToBeCovered != numRectangles) {
        printAllRandomRectanglesChosenFromPartition(randomlyChosenRectangleIDs);
    }
    
    vector<bool> rectangleCovered(numRectangles + 1);
    processInputAndAddToOutputFile(inputFile, outputFile, numRectangles, randomlyChosenRectangleIDs, rectangleBoundaryVertices, vertexOutDegree, rectanglesAtVertex);
    initializeRectanglesDegreesAndPQ(rectangleBoundaryVertices, rectanglesAtVertex, vertexOutDegree, rectangleDegree, rectanglePriorityQueue);

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
            // print("\tPlacing guard in ({}, {})\n", bestVertex.first, bestVertex.second);
        }
    }

    printMinimumNumberOfGuardsRequired(minNumGuardsRequired);
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

    ofstream outputFile("../../PartsRectangulares/testingFilteredOutput.txt");

    int numInstances;
    inputFile >> numInstances;
    printNumInstancesToConsider(numInstances);
    outputFile << numInstances << "\n";

    for (int currentInstance = 1; currentInstance <= numInstances; currentInstance++) {
        printCurrentInstanceNumber(currentInstance);
        printPercentageToCoverInputMessage();

        float percentageToCover = getInputPercentageIfValidOrDefault100();

        solve(inputFile, outputFile, percentageToCover);
        print("\n");
    }

    inputFile.close();
}

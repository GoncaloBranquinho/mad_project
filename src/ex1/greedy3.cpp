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

void calculateWhichRectanglesToUpdate(set<int>& rectanglesToUpdate, const set<Vertex>& affectedVertices, const map<Vertex, set<int>>& rectanglesAtVertex, const vector<bool>& rectangleCovered) {
    for (const auto& vertex: affectedVertices) {
        for (const auto& rectangleID : rectanglesAtVertex.at(vertex)) {
            if (!rectangleCovered.at(rectangleID)) {
                rectanglesToUpdate.insert(rectangleID);
            }
        }
    }
}

void initializeRectanglesDegreesAndPQ(map<int, set<Vertex>>& rectangleBoundaryVertices, const map<Vertex, int>& vertexOutDegree, map<int, int>& rectangleDegree, priority_queue<PairRectangleDegree>& rectanglePriorityQueue) {
    
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
        // print("\tInitiliazlized rectangle {}'s degree to {}\n", rectangleID, degree);
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


void solve(istream &inputFile, ofstream& outputFile, float percentageToCover) {
    map<Vertex, int> vertexOutDegree;
    map<int, set<Vertex>> rectangleBoundaryVertices;
    map<Vertex, set<int>> rectanglesAtVertex;
    map<int, int> rectangleDegree;
    priority_queue<PairRectangleDegree> rectanglePriorityQueue;
    
    int numRectangles;
    processInputsAndAddToOutputFile(inputFile, outputFile, numRectangles, percentageToCover, rectangleBoundaryVertices, vertexOutDegree, rectanglesAtVertex);
    int numRectanglesToBeCovered = lround(numRectangles * percentageToCover / 100.0);
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

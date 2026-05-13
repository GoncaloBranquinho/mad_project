#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>

using namespace std;

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

void printAllVerticesOutDegrees(const map<pair<int, int>, int>& vertexOutDegree) {

    cout << "\nDegree associated to each vertex (number of rectangles it is in conctact with)\n";
    cout << "vertexOutDegree = {\n";
    bool printComma = false;

    for (const auto& entry : vertexOutDegree) {
        const auto& vertexCoords = entry.first;
        const auto& vertexDegree = entry.second;

        if (printComma) {
            cout << ",\n";
        } else {
            printComma = true;
        }

        cout << "   (" << vertexCoords.first << ", " << vertexCoords.second << "): " << vertexDegree;
    }

    cout << "\n}\n\n";
}

void printAllRectanglesBoundaryVertices(const map<int, set<pair<int, int>>>& rectangleBoundVertices) {

    cout << "\nBoundary vertices associated to each rectangle\n";
    cout << "rectangleBoundVertices = {\n";

    for (const auto& entry : rectangleBoundVertices) {
        const int rectangleID = entry.first;
        const auto& verticesSet = entry.second;

        cout << "   " << rectangleID << ": = {";
        bool printComma = false;

        for (const auto& vertex : verticesSet) {
            if (printComma) {
                cout << ", ";
            }

            cout << "(" << vertex.first << ", " << vertex.second << ")";
            printComma = true;
        }

        cout << "},\n";
    }

    cout << "}\n\n";
}

void printAllRectanglesAtVertices(const map<pair<int, int>, set<int>>& rectanglesAtVertex) {

    cout << "\nRectangles which each vertex is in contact with\n";
    cout << "rectanglesAtVertex = {\n";

    for (const auto& entry : rectanglesAtVertex) {
        const auto& vertexCoords = entry.first;
        const auto& rectanglesIDsSet = entry.second;

        cout << "   (" << vertexCoords.first << ", " << vertexCoords.second << "): {";
        bool printComma = false;

        for (const auto& rectangleID : rectanglesIDsSet) {
            if (printComma) {
                cout << ", ";
            } else {
                printComma = true;
            }

            cout << rectangleID;
        }  

        cout << "},\n";
    }

    cout << "}\n\n";
}

void printAllRectanglesDegrees(const map<int, int>& rectangleDegree) {

    cout << "\nDegree associated to each rectangle (sum of the degrees of the vertices it SHARES i.e. ≠ 1)\n";
    cout << "rectangleDegree = {\n";
    bool printComma = false;

    for (const auto& entry : rectangleDegree) {
        if (printComma) {
            cout << ",\n";
        } else {
            printComma = true;
        }

        cout << "   " << entry.first << ": " << entry.second ;
    }
    
    cout << "\n}\n\n";
}

void printRectanglePriorityQueue(priority_queue<PairRectangleDegree> rectanglePriorityQueue) {
    
    cout << "\nPQ - Degree associated to each rectangle (sum of the degrees of the vertices it SHARES i.e. ≠ 1)\n";
    cout << "rectanglePriorityQueue = {\n";
    bool printComma = false;

    while (!rectanglePriorityQueue.empty()) {
        auto pairRectangleDegree = rectanglePriorityQueue.top();
        rectanglePriorityQueue.pop();

        if (printComma) {
            cout << ",\n";
        } else {
            printComma = true;
        }

        cout << "   (" << pairRectangleDegree.rectangleID << ", " << pairRectangleDegree.rectangleDegree << ")";
    }

    cout << "\n}\n\n";
}

pair<int, int> bestVertexOfRectangle(int rectangleID, const map<int, set<pair<int, int>>>& rectangleBoundVertices, const map<pair<int, int>, int>& vertexOutDegree) {

    // cout << "Most \"isolated\" rectangle: " << rectangleID << " -> {";
    pair<int, int> bestVertex;
    int bestVertexDegree = 0;
    bool printComma = false;

    for (const auto& vertex : rectangleBoundVertices.at(rectangleID)) {

        // if (printComma) {
        //     cout << ", ";
        // } else {
        //     printComma = true;
        // }

        // cout << "((" << vertex.first << ", " << vertex.second << "), " << vertexOutDegree[vertex] << ")";

        if (vertexOutDegree.at(vertex) > bestVertexDegree) {
            bestVertexDegree = vertexOutDegree.at(vertex);
            bestVertex = vertex;
        }
    }

    // cout << "}\n";
    // cout << "The best vertex is (" << bestVertex.first << ", " << bestVertex.second << ") with outDegree " << bestVertexDegree << "\n";

    return bestVertex;
}

void updateRectangleDegrees(const map<int, set<pair<int,int>>>& rectangleBoundVertices, const vector<bool>& rectangleCovered, const map<pair<int,int>, int>& vertexOutDegree, map<int, int>& rectangleDegree, priority_queue<PairRectangleDegree>& rectanglePriorityQueue) {
    
    for (const auto& entry : rectangleBoundVertices) {
        const auto& rectangleID = entry.first;
        const auto& pairSet = entry.second;

        if (!rectangleCovered[rectangleID]) {
            // cout << "Visiting " << rectangleID;
            int degree = 0;

            for (const auto& vertexCoords : pairSet) {
                if (vertexOutDegree.at(vertexCoords) > 1) {
                    degree += vertexOutDegree.at(vertexCoords);
                }
            }

            if (rectangleDegree[rectangleID] != degree) {
                // cout << " Updated!";
                rectangleDegree[rectangleID] = degree;
                rectanglePriorityQueue.push(PairRectangleDegree(rectangleID, degree));
            }

            // cout << "\n";
        }
    }
}

void solve(istream &inputFile) {
    map<pair<int, int>, int> vertexOutDegree;
    map<int, set<pair<int, int>>> rectangleBoundVertices;
    map<pair<int, int>, set<int>> rectanglesAtVertex;
    map<int, int> rectangleDegree;
    priority_queue<PairRectangleDegree> rectanglePriorityQueue;
    
    int numRectangles;
    inputFile >> numRectangles;

    vector<bool> rectangleCovered(numRectangles + 1);

    for (int i = 0; i < numRectangles; i++) {
        int rectangleID;
        int numVertices;
        inputFile >> rectangleID >> numVertices;

        for (int j = 0; j < numVertices; j++) {
            int x, y;
            inputFile >> x >> y;
            auto vertex = make_pair(x, y);
            vertexOutDegree[vertex] += 1;
            rectangleBoundVertices[rectangleID].insert(vertex);
            rectanglesAtVertex[vertex].insert(rectangleID);
        }
    }

    updateRectangleDegrees(rectangleBoundVertices, rectangleCovered, vertexOutDegree, rectangleDegree, rectanglePriorityQueue);
    
    printAllVerticesOutDegrees(vertexOutDegree);
    printAllRectanglesBoundaryVertices(rectangleBoundVertices);
    printAllRectanglesAtVertices(rectanglesAtVertex);
    printAllRectanglesDegrees(rectangleDegree);
    printRectanglePriorityQueue(rectanglePriorityQueue);

    int uncoveredRectangles = numRectangles;
    int numGuardsPlaced = 0;
    // int iterations = 0;

    while (uncoveredRectangles > 0) {
        // iterations++;
        auto pairRectangleDegree = rectanglePriorityQueue.top();
        rectanglePriorityQueue.pop();
        int currRectangleID = pairRectangleDegree.rectangleID;

        if (!rectangleCovered[currRectangleID]) {
            // Given the "most isolated" rectangle of the uncovered ones
            // Find its best vertex (highest outDegree of all)
            auto bestVertex = bestVertexOfRectangle(currRectangleID, rectangleBoundVertices, vertexOutDegree);

            // Set surrounding rectangles as covered and Update remaining vertices' outDegree
            for (auto rectangleID : rectanglesAtVertex[bestVertex]) {
                rectangleCovered[rectangleID] = true;
                uncoveredRectangles--;
                // cout << "Setting " << rectangleID << " as covered\n";
                
                for (auto vertex: rectangleBoundVertices[rectangleID]) {
                    vertexOutDegree[vertex] -= 1;
                }
            }

            // Officially place a guard in that vertex
            numGuardsPlaced++;
            // cout << "Placing guard in: (" << bestVertex.first << ", " << bestVertex.second << ")\n";
            
            // Update remaining uncovered rectangle's degrees
            updateRectangleDegrees(rectangleBoundVertices, rectangleCovered, vertexOutDegree, rectangleDegree, rectanglePriorityQueue);
        }
    }

    cout << "Total number of guards: " << numGuardsPlaced << "\n";
    // cout << "Total iterations: " << iterations << "\n";
}

int main(int argc, char *argv[]) {
    ifstream inputFile(argv[1]);

    if (!inputFile.is_open()) {
        return 1;
    }

    int numInstances;
    inputFile >> numInstances;

    for (int i = 0; i < numInstances; i++) {
        solve(inputFile);
    }

    inputFile.close();
}

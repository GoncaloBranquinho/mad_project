#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <numeric>
#include <random>
#include <algorithm>

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

void printAllRectanglesDegrees(const map<int, set<int>>& rectangleDegree) {

    cout << "\nDegree associated to each rectangle (number of uncovered rectangles it shares vertices with)\n";
    cout << "rectangleDegree = {\n";

    for (const auto& entry : rectangleDegree) {
        const auto& adjacentRectanglesSet = entry.second;

        bool printComma = false;
        cout << "   " << entry.first << ": " << adjacentRectanglesSet.size() << " ⟶ {";

        for (const auto& rectangleID : adjacentRectanglesSet) {
            if (printComma) {
                cout << ", ";
            } else {
                printComma = true;
            }

            cout << rectangleID;
        }

        cout << "},\n";
    }
    
    cout << "}\n";
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

void printAllRectanglesChosenFromPartition(const set<int>& randomlyChosenRectangleIDs) {

    cout << "Randomly chosen subset of rectangles of the partition: {";

    bool printComma = false;
    for (const auto& rectangleID : randomlyChosenRectangleIDs) {
        
        if (printComma) {
            cout << ", ";
        } else {
            printComma = true;
        }

        cout << rectangleID;
    }

    cout << "}\n";
}

void calculateWhichRectanglesToUpdate(set<int>& rectanglesToUpdate, const set<int>& rectanglesCoveredByVertex, const map<int, set<pair<int, int>>>& rectangleBoundVertices, const map<pair<int, int>, set<int>>& rectanglesAtVertex, const vector<bool>& rectangleCovered) {
    for (auto rectangleID: rectanglesCoveredByVertex) {
        for (auto vertex: rectangleBoundVertices.at(rectangleID)) {
            for (auto rectID : rectanglesAtVertex.at(vertex)) {
                if (!rectangleCovered.at(rectID)) {
                    rectanglesToUpdate.insert(rectID);
                }
            }
        }   
    }
}

void printAllRectanglesCoveredByCurrentBestVertex(const set<int>& rectanglesCoveredByVertex) {
    
    cout << "Rectangles to covered by vertex: {";

    bool printComma = false;
    
    for (auto rectangleID : rectanglesCoveredByVertex) {
        if (printComma) {
            cout << ", ";
        } else {
            printComma = true;
        }
        
        cout << rectangleID;
    }
    
    cout << "}\n";
}

void printAllRectanglesToUpdate(set<int>& rectanglesToUpdate) {
    
    cout << "Rectangles to Update: {";

    bool printComma = false;
    
    for (auto rectangleID : rectanglesToUpdate) {
        if (printComma) {
            cout << ", ";
        } else {
            printComma = true;
        }
        
        cout << rectangleID;
    }
    
    cout << "}\n";
}

void chooseRandomRectanglesFromPartition(set<int>& randomlyChosenRectangleIDs, int numRectanglesToBeCovered, int numRectangles) {
    vector<int> rectangleIDs(numRectangles);
    iota(rectangleIDs.begin(), rectangleIDs.end(), 1);
    shuffle(rectangleIDs.begin(), rectangleIDs.end(), mt19937(random_device{}()));
    randomlyChosenRectangleIDs.insert(rectangleIDs.begin(), rectangleIDs.begin() + numRectanglesToBeCovered);
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

        // cout << "((" << vertex.first << ", " << vertex.second << "), " << vertexOutDegree.at(vertex) << ")";

        if (vertexOutDegree.at(vertex) > bestVertexDegree) {
            bestVertexDegree = vertexOutDegree.at(vertex);
            bestVertex = vertex;
        }
    }

    // cout << "}\n";
    // cout << "The best vertex is (" << bestVertex.first << ", " << bestVertex.second << ") with outDegree " << bestVertexDegree << "\n";

    return bestVertex;
}
    
void initializeRectanglesDegreesAndPQ(map<int, set<pair<int,int>>>& rectangleBoundVertices, const map<pair<int, int>, set<int>>& rectanglesAtVertex,  const map<pair<int,int>, int>& vertexOutDegree, map<int, set<int>>& rectangleDegree, priority_queue<PairRectangleDegree>& rectanglePriorityQueue) {

    for (const auto& entry : rectangleBoundVertices) {
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
    }
}

void updateNecessaryRectanglesDegreesAndPQ(const set<int>& rectanglesToUpdate, const set<int>& rectanglesCoveredByVertex, map<int, set<int>>& rectangleDegree, priority_queue<PairRectangleDegree>& rectanglePriorityQueue) {

    if (rectanglesToUpdate.size() != 0) {
        for (const auto& rectangleIDCoveredByVertex : rectanglesCoveredByVertex) {
            for (const auto& rectangleID : rectanglesToUpdate) {
                if (rectangleDegree.contains(rectangleIDCoveredByVertex)) {
                    rectangleDegree[rectangleID].erase(rectangleIDCoveredByVertex);
                }
            }
        }

        for (const auto& rectangleID : rectanglesToUpdate) {
            rectanglePriorityQueue.push(PairRectangleDegree(rectangleID, rectangleDegree[rectangleID].size()));
        }
    }
}


void solve(istream &inputFile, int percentageToCover) {
    set<int> randomlyChosenRectangleIDs;
    map<pair<int, int>, int> vertexOutDegree;
    map<int, set<pair<int, int>>> rectangleBoundVertices;
    map<pair<int, int>, set<int>> rectanglesAtVertex;
    map<int, set<int>> rectangleDegree;
    priority_queue<PairRectangleDegree> rectanglePriorityQueue;
    
    int numRectangles;
    inputFile >> numRectangles;

    int numRectanglesToBeCovered = lround(numRectangles * percentageToCover / 100.0);
    cout << "Considering " << numRectanglesToBeCovered << " / " << numRectangles << " of the partition's rectangles to be covered\n";
    chooseRandomRectanglesFromPartition(randomlyChosenRectangleIDs, numRectanglesToBeCovered, numRectangles);
    
    if (numRectanglesToBeCovered == numRectangles) {
        cout << "All rectangles of the partition included\n";
    } else {
        printAllRectanglesChosenFromPartition(randomlyChosenRectangleIDs);
    }

    vector<bool> rectangleCovered(numRectangles + 1);

    for (int i = 0; i < numRectangles; i++) {
        int rectangleID;
        int numVertices;
        inputFile >> rectangleID >> numVertices;

        bool rectangleIsInPartitionSubset = randomlyChosenRectangleIDs.contains(rectangleID);

        for (int j = 0; j < numVertices; j++) {
            int x, y;
            inputFile >> x >> y;

            if (rectangleIsInPartitionSubset) {
                auto vertex = make_pair(x, y);
                vertexOutDegree[vertex] += 1;
                rectangleBoundVertices[rectangleID].insert(vertex);
                rectanglesAtVertex[vertex].insert(rectangleID);
            }
        }
    }

    initializeRectanglesDegreesAndPQ(rectangleBoundVertices, rectanglesAtVertex, vertexOutDegree, rectangleDegree, rectanglePriorityQueue);

    // printAllVerticesOutDegrees(vertexOutDegree);
    // printAllRectanglesBoundaryVertices(rectangleBoundVertices);
    // printAllRectanglesAtVertices(rectanglesAtVertex);
    // printAllRectanglesDegrees(rectangleDegree);
    // printRectanglePriorityQueue(rectanglePriorityQueue);

    int coveredRectangles = 0;
    int numGuardsPlaced = 0;

    while (coveredRectangles != numRectanglesToBeCovered) {
        auto pairRectangleDegree = rectanglePriorityQueue.top(); rectanglePriorityQueue.pop();
        int currRectangleID = pairRectangleDegree.rectangleID;

        if (!rectangleCovered.at(currRectangleID)) {

            // printAllRectanglesDegrees(rectangleDegree);

            // 0. Given the "most isolated" rectangle of the uncovered ones
            // 1. Find its best vertex (highest outDegree of all)
            auto bestVertex = bestVertexOfRectangle(currRectangleID, rectangleBoundVertices, vertexOutDegree);

            // 2. Set surrounding uncovered rectangles as covered and update remaining vertices' outDegree
            for (auto rectangleID : rectanglesAtVertex.at(bestVertex)) {
                if (!rectangleCovered.at(rectangleID)) {
                    rectangleCovered.at(rectangleID) = true;
                    coveredRectangles++;

                    for (auto vertex: rectangleBoundVertices.at(rectangleID)) {
                        vertexOutDegree.at(vertex) -= 1;
                    }   
                }
            }

            // 3. Only consider the rectangles whose degree is affected (are bound to the vertices whose degree was decremented)
            set<int> rectanglesCoveredByVertex = rectanglesAtVertex.at(bestVertex);
            // printAllRectanglesCoveredByCurrentBestVertex(rectanglesCoveredByVertex);

            set<int> rectanglesToUpdate;
            calculateWhichRectanglesToUpdate(rectanglesToUpdate, rectanglesCoveredByVertex, rectangleBoundVertices, rectanglesAtVertex, rectangleCovered);

            // printAllRectanglesToUpdate(rectanglesToUpdate);

            
            // 4. Update remaining necessary uncovered rectangle's degrees
            updateNecessaryRectanglesDegreesAndPQ(rectanglesToUpdate, rectanglesCoveredByVertex, rectangleDegree, rectanglePriorityQueue);


            // 5. Officially place a guard in that vertex
            numGuardsPlaced++;
            // cout << "Placing guard in: (" << bestVertex.first << ", " << bestVertex.second << ")\n";
        }
    }

    cout << "Total number of guards: " << numGuardsPlaced << "\n";
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        cout << "Insufficient arguments\n";
        return 1;
    }

    ifstream inputFile(argv[1]);

    if (!inputFile.is_open()) {
        cout << "Unable to open file " << argv[1] << "\n";
        return 1;
    }

    int percentageToCover = 100;

    if (argc >= 3) {
        percentageToCover = stoi(argv[2]);

        if (percentageToCover < 0 || percentageToCover > 100) {
            cout << "Invalid percentage\n";
            return 1;
        }
    }

    int numInstances;
    inputFile >> numInstances;

    for (int i = 0; i < numInstances; i++) {
        solve(inputFile, percentageToCover);
    }

    inputFile.close();
}

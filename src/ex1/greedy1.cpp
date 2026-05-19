#include "utils.h"

#include <fstream>
#include <iostream>

using namespace std;

class VertexWithOutDegree {
    public:
    Vertex vertex;
    int outdegree;

    VertexWithOutDegree(int x, int y, int outdegree) {
        this->vertex = Vertex(x, y);
        this->outdegree = outdegree;
    }

    bool operator<(const VertexWithOutDegree &other) const {
        if (this->outdegree == other.outdegree) {
            if (this->vertex.x == other.vertex.x) {
                return this->vertex.y > other.vertex.y;
            }
            return this->vertex.x < other.vertex.x;
        }
        return this->outdegree < other.outdegree;
    }
};

void solve(istream &inputFile, float percentageToCover) {
    set<int> randomlyChosenRectangleIDs;
    map<Vertex, int> vertexOutDegree;
    map<Vertex, int> currOutDegree;
    map<Vertex, set<int>> rectanglesAtVertex;
    map<int, set<Vertex>> rectangleBoundaryVertices;
    priority_queue<VertexWithOutDegree> queue;

    int numRectangles;
    inputFile >> numRectangles;

    int numRectanglesToBeCovered = lround(numRectangles * percentageToCover / 100.0);
    print("\tConsidering {}% coverage of the partition's rectangles ({} / {})\n", percentageToCover, numRectanglesToBeCovered, numRectangles);
    chooseRandomRectanglesFromPartition(randomlyChosenRectangleIDs, numRectanglesToBeCovered, numRectangles);
    
    if (numRectanglesToBeCovered != numRectangles) {
        printAllRandomRectanglesChosenFromPartition(randomlyChosenRectangleIDs);
    }

    processInput(inputFile, numRectangles, randomlyChosenRectangleIDs, rectangleBoundaryVertices, vertexOutDegree, rectanglesAtVertex);

    for (const auto& vertex : vertexOutDegree) {
        auto vertexCoords = vertex.first;
        int outDegree = vertex.second;
        int x = vertexCoords.x;
        int y = vertexCoords.y;
        currOutDegree[vertexCoords] = outDegree;
        queue.push(VertexWithOutDegree(x, y, outDegree));
    }

    int minNumGuardsRequired = 0;

    while (!queue.empty()) {
        const auto vertexWithOutDegree = queue.top(); queue.pop();
        const auto& vertexCoords = vertexWithOutDegree.vertex;
        int outDegree = vertexWithOutDegree.outdegree;

        if (currOutDegree[vertexCoords] == outDegree) { 
            minNumGuardsRequired++;

            for (const auto& rectangleID : rectanglesAtVertex[vertexCoords]) {
                rectangleBoundaryVertices[rectangleID].erase(vertexCoords);

                for (const auto& otherVertex : rectangleBoundaryVertices[rectangleID]) {
                    int x = otherVertex.x;
                    int y = otherVertex.y;

                    auto otherVertexCoords = Vertex(x, y);
                    vertexOutDegree[otherVertexCoords] -= 1;
                    rectanglesAtVertex[otherVertexCoords].erase(rectangleID);
                    currOutDegree[otherVertexCoords] = vertexOutDegree[otherVertexCoords];

                    if (vertexOutDegree[otherVertexCoords] > 0) {
                        queue.push(VertexWithOutDegree(x, y, vertexOutDegree[otherVertexCoords]));
                    }
                }
            }
        }
    }
    print("\tMinimum number of guards required: {}\n", minNumGuardsRequired);
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

    int numInstances;
    inputFile >> numInstances;
    print("\nTotal instances to consider: {}\n\n", numInstances);

    for (int currentInstance = 1; currentInstance <= numInstances; currentInstance++) {
        float percentageToCover = 100.0;
        print("Instance {}:\n", currentInstance);
        print("\tPercentage (%) of the partition to cover (rounded, invalid inputs default to 100%): ");

        string input;
        getline(cin ,input);

        if (!input.empty() && input.back() == '%') {
            input.pop_back();
        }

        try {
            float percentageToCoverInput = stof(input);
            percentageToCover = round(percentageToCover * 10.0) / 10.0;
            if (0.0 <= percentageToCoverInput && percentageToCoverInput <= 100.0) {
                percentageToCover = percentageToCoverInput;
            }
        } catch (...) {}

        solve(inputFile, percentageToCover);
        print("\n");
    }

    inputFile.close();
}

#include "utils.h"


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


void solve(istream &inputFile, ofstream& outputFile, float percentageToCover) {
    set<int> randomlyChosenRectangleIDs;
    map<Vertex, int> vertexOutDegree;
    map<Vertex, int> currOutDegree;
    map<Vertex, set<int>> rectanglesAtVertex;
    map<int, set<Vertex>> rectangleBoundaryVertices;
    priority_queue<VertexWithOutDegree> queue;

    int numRectangles;
    inputFile >> numRectangles;

    int numRectanglesToBeCovered = lround(numRectangles * percentageToCover / 100.0);
    printPercentageOfRectanglesInConsideration(percentageToCover, numRectanglesToBeCovered, numRectangles);
    outputFile << numRectanglesToBeCovered << "\n";

    chooseRandomRectanglesFromPartition(randomlyChosenRectangleIDs, numRectanglesToBeCovered, numRectangles);
    if (numRectanglesToBeCovered != numRectangles) {
        printAllRandomRectanglesChosenFromPartition(randomlyChosenRectangleIDs);
    }

    processInputAndAddToOutputFile(inputFile, outputFile, numRectangles, randomlyChosenRectangleIDs, rectangleBoundaryVertices, vertexOutDegree, rectanglesAtVertex);

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

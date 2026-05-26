#include "../utils/SharedByAllGreedysUtils.hpp"

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
        map<Vertex, int> currOutDegree;
        priority_queue<VertexWithOutDegree> vertexPriorityQueue;

        processCurrentInstanceInputs(inputFile, numRectangles, numRectanglesToBeCovered, rectangleBoundaryVertices, vertexOutDegree, rectanglesAtVertex);

        for (const auto& vertex : vertexOutDegree) {
            auto vertexCoords = vertex.first;
            int outDegree = vertex.second;
            int x = vertexCoords.x;
            int y = vertexCoords.y;
            currOutDegree[vertexCoords] = outDegree;
            vertexPriorityQueue.push(VertexWithOutDegree(x, y, outDegree));
        }

        int minNumGuardsRequired = 0;

        while (!vertexPriorityQueue.empty()) {
            const auto vertexWithOutDegree = vertexPriorityQueue.top(); vertexPriorityQueue.pop();
            const auto& vertexCoords = vertexWithOutDegree.vertex;
            int outDegree = vertexWithOutDegree.outdegree;

            if (currOutDegree[vertexCoords] == outDegree) { 
                minNumGuardsRequired++;
                // print("\tPlacing guard in ({}, {})\n", vertexCoords.x, vertexCoords.y);

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
                            vertexPriorityQueue.push(VertexWithOutDegree(x, y, vertexOutDegree[otherVertexCoords]));
                        }
                    }
                }
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

#include "SharedByAllGreedysUtils.h"

void printAllVerticesOutDegrees(const map<Vertex, int>& vertexOutDegree) {
    print("\n\tDegree associated to each vertex (number of rectangles it is in conctact with)\n");
    print("\tvertexOutDegree = {{\n");
    bool printComma = false;

    for (const auto& entry : vertexOutDegree) {
        const auto& vertexCoords = entry.first;
        const auto& vertexDegree = entry.second;

        if (printComma) {
            print(",\n");
        } else {
            printComma = true;
        }

        print("\t\t({}, {}): {}", vertexCoords.x, vertexCoords.y, vertexDegree);
    }

    print("\n\t}}\n\n");
}

void printAllRectanglesBoundaryVertices(const map<int, set<Vertex>>& rectangleBoundVertices) {
    print("\n\tBoundary vertices associated to each rectangle\n");
    print("\trectangleBoundVertices = {{\n");

    for (const auto& entry : rectangleBoundVertices) {
        const auto& rectangleID = entry.first;
        const auto& verticesSet = entry.second;

        print("\t   {}: {{", rectangleID);
        bool printComma = false;

        for (const auto& vertex : verticesSet) {
            if (printComma) {
                cout << ", ";
            } else {
                printComma = true;
            }

            print("({}, {})", vertex.x, vertex.y);
        }

        print("}},\n");
    }

    print("\t}}\n\n");
}

void printAllRectanglesAtVertices(const map<Vertex, set<int>>& rectanglesAtVertex) {
    print("\n\tRectangles which each vertex is in contact with\n");
    print("\trectanglesAtVertex = {{\n");

    for (const auto& entry : rectanglesAtVertex) {
        const auto& vertex = entry.first;
        const auto& rectanglesIDsSet = entry.second;

        cout << "\t  (" << vertex.x << ", " << vertex.y << "): {";
        bool printComma = false;

        for (const auto& rectangleID : rectanglesIDsSet) {
            if (printComma) {
                print(", ");
            } else {
                printComma = true;
            }

            cout << rectangleID;
        }  

        print("}},\n");
    }

    print("\t}}\n\n");
}

void printAllRectanglesCoveredByCurrentBestVertex(const set<int>& rectanglesCoveredByVertex) {    
    print("\n\tRectangles to covered by vertex: {{");
    bool printComma = false;
    
    for (const auto& rectangleID : rectanglesCoveredByVertex) {
        if (printComma) {
            print(", ");
        } else {
            printComma = true;
        }
        
        print("{}", rectangleID);
    }
    
    print("}}\n");
}

void printAllRectanglesToUpdate(const set<int>& rectanglesToUpdate) {  
    print("\n\tRectangles to Update: {{");
    bool printComma = false;
    
    for (const auto& rectangleID : rectanglesToUpdate) {
        if (printComma) {
            print(", ");
        } else {
            printComma = true;
        }
        
        print("{}", rectangleID);
    }
    
    print("}}\n");
}
void processCurrentInstanceInputs(istream &inputFile, int& numRectangles, int& numRectanglesToBeCovered, map<int, set<Vertex>>& rectangleBoundaryVertices, map<Vertex, int>& vertexOutDegree, map<Vertex, set<int>>& rectanglesAtVertex) {
    printPercentageToCoverInputMessage();
    float percentageToCover = getInputPercentageIfValidOrDefault100();
    inputFile >> numRectangles;

    numRectanglesToBeCovered = lround(numRectangles * percentageToCover / 100.0);
    printPercentageOfRectanglesInConsideration(percentageToCover, numRectanglesToBeCovered, numRectangles);
    
    set<int> randomlyChosenRectangleIDs;
    chooseRandomRectanglesFromPartition(randomlyChosenRectangleIDs, numRectanglesToBeCovered, numRectangles);
    if (numRectanglesToBeCovered != numRectangles) {
        printAllRandomRectanglesChosenFromPartition(randomlyChosenRectangleIDs);
    }    
    
    for (int i = 0; i < numRectangles; i++) {
        int rectangleID;
        int numVertices;
        inputFile >> rectangleID >> numVertices;

        bool rectangleIsInPartitionSubset = randomlyChosenRectangleIDs.contains(rectangleID);

        for (int j = 0; j < numVertices; j++) {
            int x, y;
            inputFile >> x >> y;

            if (rectangleIsInPartitionSubset) {
                const auto vertex = Vertex(x, y);
                vertexOutDegree[vertex] += 1;
                rectangleBoundaryVertices[rectangleID].insert(vertex);
                rectanglesAtVertex[vertex].insert(rectangleID);
            }
        }
    }
}

Vertex rectanglesVertexWithHighestDegree(int rectangleID, const map<int, set<Vertex>>& rectangleBoundaryVertices, const map<Vertex, int>& vertexOutDegree) {
    Vertex bestVertex = Vertex(-1, -1);
    int bestVertexDegree = 0;
    bool printComma = false;

    for (const auto& vertex : rectangleBoundaryVertices.at(rectangleID)) {
        if (vertexOutDegree.at(vertex) > bestVertexDegree) {
            bestVertexDegree = vertexOutDegree.at(vertex);
            bestVertex = vertex;
        }
    }
    return bestVertex;
}

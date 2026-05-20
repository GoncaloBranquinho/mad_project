#include "utils.h"


// shared by more than one .cpp file
void printNumInstancesToConsider(int numInstances) {
    print("\nTotal instances to consider: {}\n\n", numInstances);
}

void printCurrentInstanceNumber(int currentInstance) {
    print("Instance {}:\n", currentInstance);
}

void printPercentageOfRectanglesInConsideration(float percentageToCover, int numRectanglesToBeCovered, int numRectangles) {
    print("\tConsidering {}% coverage of the partition's rectangles ({} / {})\n", percentageToCover, numRectanglesToBeCovered, numRectangles);
}

void printPercentageToCoverInputMessage() {
    print("\tPercentage (%) of the partition to cover (rounded, invalid inputs default to 100%): ");
}

float getInputPercentageIfValidOrDefault100() {
        string input;
        getline(cin ,input);

        if (!input.empty() && input.back() == '%') {
            input.pop_back();
        }

        try {
            float percentageToCoverInput = stof(input);
            if (0.0 <= percentageToCoverInput && percentageToCoverInput <= 100.0) {
                return percentageToCoverInput;
            }
        } catch (...) {}
        return 100.0;
}

void printMinimumNumberOfGuardsRequired(int minNumGuardsRequired) {
    print("\tMinimum number of guards required: {}\n", minNumGuardsRequired);
}

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

void printAllRandomRectanglesChosenFromPartition(const set<int>& randomlyChosenRectangleIDs) {
    print("\tPress 'y' to show the {} randomly chosen rectangles: ", randomlyChosenRectangleIDs.size());

    string input;
    getline(cin ,input);

    if (input == "y") {
        print("\tRandom subset of the partition's rectangles chosen: {{");
        bool printComma = false;

        for (const auto& rectangleID : randomlyChosenRectangleIDs) {     
            if (printComma) {
                print(", ");
            } else {
                printComma = true;
            }

            print("{}", rectangleID);
        }

        print("}}\n");
    }
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

void printAllRectanglesToUpdate(set<int>& rectanglesToUpdate) {  
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

void processInputsAndAddToOutputFile(istream &inputFile, ofstream& outputFile, int& numRectangles, const float percentageToCover, map<int, set<Vertex>>& rectangleBoundaryVertices, map<Vertex, int>& vertexOutDegree, map<Vertex, set<int>>& rectanglesAtVertex) {
    inputFile >> numRectangles;

    int numRectanglesToBeCovered = lround(numRectangles * percentageToCover / 100.0);
    printPercentageOfRectanglesInConsideration(percentageToCover, numRectanglesToBeCovered, numRectangles);
    outputFile << numRectanglesToBeCovered << "\n";    
    
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

                if (j == 0) {
                    outputFile << rectangleID << " " << numVertices;
                }

                outputFile << " " << x << " " << y;

                if (j == numVertices - 1) {
                    outputFile << "\n";
                }
            }
        }
    }
}

void chooseRandomRectanglesFromPartition(set<int>& randomlyChosenRectangleIDs, int numRectanglesToBeCovered, int numRectangles) {
    vector<int> rectangleIDs(numRectangles);
    iota(rectangleIDs.begin(), rectangleIDs.end(), 1);
    shuffle(rectangleIDs.begin(), rectangleIDs.end(), mt19937(random_device{}()));
    randomlyChosenRectangleIDs.insert(rectangleIDs.begin(), rectangleIDs.begin() + numRectanglesToBeCovered);
}

// greedy3.cpp exclusive
void printAllRectanglesDegrees(const map<int, int>& rectangleDegree) { 
    print("\n\tDegree associated to each rectangle (sum of the degrees of the vertices it SHARES i.e. ≠ 1)\n");
    print("\trectangleDegree = {{\n");
    bool printComma = false;

    for (const auto& entry : rectangleDegree) {
        const auto& rectangleID = entry.first;
        const auto& degree = entry.second;

        if (printComma) {
            print(",\t\n");
        } else {
            printComma = true;
        }

        print("\t   {}: {}", rectangleID, degree);
    }
    
    print("\n\t}}\n\n");
}

// greedy3v2.cpp exclusive
void printAllRectanglesDegrees(const map<int, set<int>>& rectangleDegree) {
    print("\n\tDegree associated to each rectangle (number of different rectangles it is in contact with / shares vertices with)\n");
    print("\trectangleDegree = {{\n");

    for (const auto& entry : rectangleDegree) {
        const auto& rectangleID = entry.first;
        const auto& adjacentRectanglesSet = entry.second;

        bool printComma = false;
        print("\t   {}: ({}) {{", rectangleID, adjacentRectanglesSet.size());

        for (const auto& rectangleID : adjacentRectanglesSet) {
            if (printComma) {
                print(", ");
            } else {
                printComma = true;
            }

            print("{}", rectangleID);
        }

        print("}},\n");
    }
    
    print("\t}}\n\n");
}

// or-tools-solver.cpp exclusive
void printVerticesSet(set<Vertex>& verticesSet) {
    print("\n\tSet of all vertices in the partition ordered acoording to Vertex' comparator\n");
    print("\tverticesSet = {{");
    bool printComma = false;

    for (const auto& vertex : verticesSet) {
        if (printComma) {
            print(", ");
        } else {
            printComma = true;
        }

        print("\n\t\t({}, {})", vertex.x, vertex.y);
    }
    print("\n\t}}\n");
}

void printIDsToVertices(map<int, Vertex>& idToVertex) {
    print("\n\tVertex associated to each ID (all vertices / ids of the partition included)\n");
    print("\tidToVertex = {{");
    bool printComma = false;

    for (const auto& entry : idToVertex) {
        const auto&  vertexID = entry.first;
        const auto&  vertex = entry.second;

        if (printComma) {
            print(",");
        } else {
            printComma = true;
        }

        print("\n\t\t{}: ({}, {})", vertexID, vertex.x, vertex.y);
    }
    print("\n\t}}\n");
}

void printVerticesToIds(map<Vertex, int>& vertexToId) {
    print("\n\tID associated to each vertex (all vertices / ids of the partition included)\n");
    print("\tvertexToId = {{");
    bool printComma = false;

    for (const auto& entry : vertexToId) {
        const auto& vertex = entry.first;
        const auto& vertexID = entry.second;

        if (printComma) {
            print(",");
        } else {
            printComma = true;
        }

        print("\n\t\t({}, {}): {}", vertex.x, vertex.y, vertexID);
    }
    print("\n\t}}\n");
}

void processInputAndAddToOutputFile(istream &inputFile, ofstream& outputFile, int numRectangles, const set<int>& randomlyChosenRectangleIDs, map<int, set<Vertex>>& rectangleBoundaryVertices, set<Vertex>& verticesSet) {
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
                verticesSet.insert(vertex);
                rectangleBoundaryVertices[rectangleID].insert(vertex);
                
                if (j == 0) {
                    outputFile << rectangleID << " " << numVertices;
                }

                outputFile << " " << x << " " << y;

                if (j == numVertices - 1) {
                    outputFile << "\n";
                }
            }
        }
    }
}
#include "ORToolsSolverNecessaryUtils.h"

void printVerticesSet(const set<Vertex>& verticesSet) {
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
    print("\n\t}}\n\n");
}

void printIDsToVertices(const map<int, Vertex>& idToVertex) {
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
    print("\n\t}}\n\n");
}

void printVerticesToIDs(const map<Vertex, int>& vertexToId) {
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
    print("\n\t}}\n\n");
}

void processCurrentInstanceInputs(istream &inputFile, int& numRectangles, map<int, set<Vertex>>& rectangleBoundaryVertices, set<Vertex>& verticesSet) {
    printPercentageToCoverInputMessage();
    float percentageToCover = getInputPercentageIfValidOrDefault100();
    inputFile >> numRectangles;

    int numRectanglesToBeCovered = lround(numRectangles * percentageToCover / 100.0);
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
                verticesSet.insert(vertex);
                rectangleBoundaryVertices[rectangleID].insert(vertex);
            }
        }
    }
}

void printAndOrSaveToFileIDsMappingToVertices(ofstream* outputModelSolutionFile, bool printToOutput, const map<int, Vertex>& idToVertex) {
    string currentLine = ("\n\tVertex Mapping: (ID associated to each vertex (all vertices/IDs of the considered partition included))\n");
    if (outputModelSolutionFile) { *outputModelSolutionFile << currentLine; }
    if (printToOutput) { print("{}", currentLine); }

    for (const auto& entry : idToVertex) {
        const auto& vertexID = entry.first;
        const auto& vertex = entry.second;

        currentLine = "\t\t" + to_string(vertexID) + " → (" + to_string(vertex.x) + ", " + to_string(vertex.y) + ")\n";
        if (outputModelSolutionFile) { *outputModelSolutionFile << currentLine; }
        if (printToOutput) { print("{}", currentLine); }
    }

    currentLine = "\n";
    if (outputModelSolutionFile) { *outputModelSolutionFile << currentLine; }
    if (printToOutput) { print("{}", currentLine); }
}

void printAndOrSaveToFileModelObjective(ofstream* outputModelSolutionFile, bool printToOutput, const map<int, Vertex>& idToVertex) {
    string currentLine = "\t\tMINIMIZE: (We want the least number of guards possible watching the whole partition, i.e. ∑x[i] for all i ∈ [1, " + to_string(idToVertex.size()) +  "])\n\t\t\t";
    if (outputModelSolutionFile) { *outputModelSolutionFile << currentLine; }
    if (printToOutput) { print("{}", currentLine); }
    bool printPlusSign = false;

    for (const auto& entry : idToVertex) {
        int vertexID = entry.first;
        currentLine = "";

        if (printPlusSign) {
            currentLine = " + ";
        } else {
            printPlusSign = true;
        }

        currentLine += "x[" + to_string(vertexID) + "]";
        if (outputModelSolutionFile) { *outputModelSolutionFile << currentLine; }
        if (printToOutput) { print("{}", currentLine); }
    }
    currentLine = "\n\n";
    if (outputModelSolutionFile) { *outputModelSolutionFile << currentLine; }
    if (printToOutput) { print("{}", currentLine); }
}

void printAndOrSaveToFileModelSubjectTo(ofstream* outputModelSolutionFile, bool printToOutput, const map<int, set<Vertex>>& rectangleBoundaryVertices, const map<Vertex, int>& vertexToId) {
    string currentLine = "\t\tSUBJECT TO: (Every rectangle must be watched, therefore needs at least one guard in one of its boundary vertices)\n";
    if (outputModelSolutionFile) { *outputModelSolutionFile << currentLine; }
    if (printToOutput) { print("{}", currentLine); }

    for (const auto& entry : rectangleBoundaryVertices) {
        const auto& rectangleID = entry.first;
        const auto& verticesSet = entry.second;

        currentLine = "\t\t\tRectangle " + to_string(rectangleID) + ": ";
        if (outputModelSolutionFile) { *outputModelSolutionFile << currentLine; }
        if (printToOutput) { print("{}", currentLine); }
        bool printPlusSign = false;

        for (const auto& vertex : verticesSet) {
            currentLine = "";
            if (printPlusSign) {
                currentLine = " + ";
            } else {
                printPlusSign = true;
            }

            currentLine += "x[" + to_string(vertexToId.at(vertex)) + "]";
            if (outputModelSolutionFile) { *outputModelSolutionFile << currentLine; }
            if (printToOutput) { print("{}", currentLine); }
        }
        currentLine = " ≥ 1\n";
        if (outputModelSolutionFile) { *outputModelSolutionFile << currentLine; }
        if (printToOutput) {print("{}", currentLine); }
    }

    currentLine = "1\n";
    if (outputModelSolutionFile) { *outputModelSolutionFile << "\n"; }
    if (printToOutput) { print("\n"); }
}

void printAndOrSaveToFileModelBounds(ofstream* outputModelSolutionFile, bool printToOutput, const map<int, Vertex>& idToVertex) {
    string currentLine = "\t\tBOUNDS: (There can be one guard per vertex at most (vertex either has, 1, or doesn't, 0), i.e. 0 ≤ x[i] ≤ 1 for all i ∈ [1, " +  to_string(idToVertex.size()) + "])\n";
    if (outputModelSolutionFile) { *outputModelSolutionFile << currentLine; }
    if (printToOutput) { print("{}", currentLine); }

    for (const auto& entry : idToVertex) {
        const auto& vertexID = entry.first;

        currentLine = "\t\t\t0 ≤ x[" + to_string(vertexID) + "] ≤ 1\n";
        if (outputModelSolutionFile) { *outputModelSolutionFile << currentLine; }
        if (printToOutput) { print("{}", currentLine); }
    }
    currentLine = "\n";
    if (outputModelSolutionFile) { *outputModelSolutionFile << currentLine; }
    if (printToOutput) { print("{}", currentLine); }
}

void printAndOrSaveToFileModel(ofstream* outputModelSolutionFile, bool printToOutput, const map<int, Vertex>& idToVertex, const map<Vertex, int>& vertexToId, const map<int, set<Vertex>>& rectangleBoundaryVertices) {
    printAndOrSaveToFileIDsMappingToVertices(outputModelSolutionFile, printToOutput, idToVertex);
    string currentLine = "\tMODEL: (i corresponds to the id of a vertex and x[i] to whether that vertex has a guard placed or not)\n\n";
    if (outputModelSolutionFile) { *outputModelSolutionFile << currentLine; }
    if (printToOutput) { print("{}", currentLine); }
    printAndOrSaveToFileModelObjective(outputModelSolutionFile, printToOutput, idToVertex);
    printAndOrSaveToFileModelSubjectTo(outputModelSolutionFile, printToOutput, rectangleBoundaryVertices, vertexToId);
    printAndOrSaveToFileModelBounds(outputModelSolutionFile, printToOutput, idToVertex);
}

void printAndOrSaveToFileSolutionFound(ofstream* outputModelSolutionFile, bool printToOutput, const set<int>& chosenVerticesIDs, const map<int, Vertex>& idToVertex) {
    string currentLine = "\tSOLUTION FOUND:" + to_string(chosenVerticesIDs.size()) + "\n";
    if (outputModelSolutionFile) { *outputModelSolutionFile << currentLine; }
    if (printToOutput) { print("{}", currentLine); }

    for (const auto& vertexID : chosenVerticesIDs) {
        const auto& vertex = idToVertex.at(vertexID);

        currentLine = "\t\tPlaced a guard in vertex: " + to_string(vertexID) + " → (" + to_string(vertex.x) + ", " + to_string(vertex.y) + ")\n";
        if (outputModelSolutionFile) { *outputModelSolutionFile << currentLine; }
        if (printToOutput) { print("{}", currentLine); }
    }
    currentLine = "\n";
    if (outputModelSolutionFile) { *outputModelSolutionFile << currentLine; }
    if (printToOutput) { print("{}", currentLine); }
}

string printAndGetOptionFromModelAndSolutionFoundMenu(string& fileName) {
    print("\tRegarding the current instance's model and solution, please select an option:\n");
    print("\t  - Type '1' to view it\n");
    if (fileName == "") {
        print("\t  - Type '2' to save it to a file\n");
    } else {
        print("\t  - Type '2' to save it to {}\n", fileName);
    }
    print("\t  - Type '3' for both of the above\n");
    print("\t  - Any other input for none of the above\n");
    print("\tOption: ");

    string selectedOption;
    getline(cin, selectedOption);
    
    if (selectedOption == "2" || selectedOption == "3") {
        if (fileName == "") {
            getOutputFileNameInput(fileName, "models_and_solutions.txt");
        }
    }

    return selectedOption;
}

void printAndOrSaveToFileModelAndSolutionFound(int& currentInstance, string& fileName, const map<int, Vertex>& idToVertex, const map<Vertex, int>& vertexToId, const map<int, set<Vertex>>& rectangleBoundaryVertices, const set<int>& chosenVerticesIDs) {
    string selectedOption = printAndGetOptionFromModelAndSolutionFoundMenu(fileName);
    bool printToOutput = false;
    ofstream* outputModelSolutionFile = nullptr;
    ofstream outputModelSolutionFileStream;

    if (selectedOption == "1" || selectedOption == "3") {
        printToOutput = true;
    }

    if (selectedOption == "2" || selectedOption == "3") {
        if (fileName != "") {
            print("\tAppending current instance's model and solution to {}...\n", fileName);
            outputModelSolutionFileStream.open(fileName, ios::app);
            outputModelSolutionFile = &outputModelSolutionFileStream;
            *outputModelSolutionFile << "Instance " << currentInstance << ":\n";
        }
    }

    if (selectedOption != "1" && selectedOption != "3") {
        print("\n");
    }

    printAndOrSaveToFileModel(outputModelSolutionFile, printToOutput, idToVertex, vertexToId, rectangleBoundaryVertices);
    printAndOrSaveToFileSolutionFound(outputModelSolutionFile, printToOutput, chosenVerticesIDs, idToVertex);
    outputModelSolutionFileStream.close();
}


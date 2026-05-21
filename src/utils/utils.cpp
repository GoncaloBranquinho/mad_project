#include "utils.h"

string pathToFolderContainingFile = "../../PartsRectangulares/";

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
    print("\tMinimum number of guards required: {}\n\n", minNumGuardsRequired);
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
    print("\tType 'y' to show the {} randomly chosen rectangles: ", randomlyChosenRectangleIDs.size());

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

void chooseRandomRectanglesFromPartition(set<int>& randomlyChosenRectangleIDs, int numRectanglesToBeCovered, int numRectangles) {
    vector<int> rectangleIDs(numRectangles);
    iota(rectangleIDs.begin(), rectangleIDs.end(), 1);
    shuffle(rectangleIDs.begin(), rectangleIDs.end(), mt19937(random_device{}()));
    randomlyChosenRectangleIDs.insert(rectangleIDs.begin(), rectangleIDs.begin() + numRectanglesToBeCovered);
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

void getOutputFileNameInput(string& fileName, string defaultOutputFileName) {
    bool viableFileName = false;
    print("\n\tWARNING - The file you choose will have contents appended to it for all instances you decide to save\n");

    while (!viableFileName) {
        print("\tType output file name (\'Enter\' to default to {}, \'c\' to cancel): ", defaultOutputFileName);
        getline(cin, fileName);

        if (fileName == "c") {
            print("\tFile saving canceled\n");
            fileName = "";
            break;
        }

        if (fileName.empty()) {
            fileName = defaultOutputFileName;
        }

        if (filesystem::exists(pathToFolderContainingFile + fileName)) {
            print("\n\tWARNING - The file \'{}\' already exists and all contents will be appended to it!\n", fileName);
            print("\tType 'y' to proceed (any other input to cancel): ");

            string proceed;
            getline(cin, proceed);

            if (proceed == "y") {
                viableFileName = true;
            }

        } else {
            viableFileName = true;
        }
    }
}

string printSavePartitionToOutputFileNameMenu(string& fileName) {
    print("\tRegarding the current considered instance:\n");
    print("\t(The following feature is useful in particular to test random subsets considered as input in other algorithms)\n");
    if (fileName == "") {
        print("\tType 'y' to save it to a file (any other input otherwise): ");
    } else {
        print("\tType 'y' to save it to {} (any other input otherwise): ", fileName);
    }

    string input;
    getline(cin, input);

    if (input == "y") {
        if (fileName == "") {
            getOutputFileNameInput(fileName, "filtered_input_partitions.txt");
        }
    }

    return input;
}

void savePartitionToOutputFile(int& numInstancesAddedToOutputFile, string& fileName, const map<int, set<Vertex>>& rectangleBoundaryVertices) {
    string save = printSavePartitionToOutputFileNameMenu(fileName);
    ofstream* outputModelSolutionFile = nullptr;
    ofstream outputModelSolutionFileStream;

    if (save == "y") {
        if (fileName != "") {
            print("\tAppending current instance to {}...\n", fileName);
            numInstancesAddedToOutputFile++;

            outputModelSolutionFileStream.open(pathToFolderContainingFile + fileName, ios::app);
            outputModelSolutionFile = &outputModelSolutionFileStream;
            *outputModelSolutionFile << rectangleBoundaryVertices.size() << "\n";
            bool printNewLine = false;

            for (const auto& entry : rectangleBoundaryVertices) {
                const auto& rectangleID = entry.first;
                const auto& verticesSet = entry.second;

                *outputModelSolutionFile << rectangleID << " " << verticesSet.size();

                for (const auto& vertex : verticesSet) {
                    *outputModelSolutionFile << " " << vertex.x << " " << vertex.y;
                }

                *outputModelSolutionFile << "\n";
            }
        }
    }

    print("\n");
    outputModelSolutionFileStream.close();
}

void insertAtBegginingNumInstancesAddedToOutputFile(int& numInstancesAddedToOutputFile, string& fileName) {
    ifstream infile(pathToFolderContainingFile + fileName);
    ofstream tempfile(pathToFolderContainingFile + "temporary.txt");
    
    tempfile << numInstancesAddedToOutputFile << "\n";
    
    string line;
    while (getline(infile, line)) {
        tempfile << line << '\n';
    }
    
    infile.close();
    tempfile.close();
    
    remove((pathToFolderContainingFile + fileName).c_str());
    rename((pathToFolderContainingFile + "temporary.txt").c_str(), (pathToFolderContainingFile + fileName).c_str());
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


// or-tools-solver.cpp exclusive
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


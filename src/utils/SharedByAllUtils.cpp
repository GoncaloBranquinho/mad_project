#include "SharedByAllUtils.h"

string pathToFolderContainingFile = "../../inputs/input-files/";
bool alreadyExistingFile = false;

void printNumInstancesToConsider(int numInstances) {
    print("\nTotal instances to consider: {}\n\n", numInstances);
}

void printCurrentInstanceNumber(int currentInstance) {
    print("Instance {}:\n", currentInstance);
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
                alreadyExistingFile = true;
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
            getOutputFileNameInput(fileName, "modified_input");
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

void printMinimumNumberOfGuardsRequired(int minNumGuardsRequired) {
    print("\tMinimum number of guards required: {}\n\n", minNumGuardsRequired);
}

void insertAtBegginingNumInstancesAddedToOutputFile(int numInstancesAddedToOutputFile, string& fileName) {
    if (fileName != "") {
        ifstream infile(pathToFolderContainingFile + fileName);
        ofstream tempfile(pathToFolderContainingFile + "temporary.txt");
        
        if (!alreadyExistingFile) {
            tempfile << numInstancesAddedToOutputFile << "\n";
        }
        
        string line;
        int iteration = 1;
        while (getline(infile, line)) {
            if (iteration == 1 && alreadyExistingFile) {
                int updatedNumInstances = numInstancesAddedToOutputFile + stoi(line);
                tempfile << updatedNumInstances << "\n";
            } else {
                tempfile << line << '\n'; 
            }
            iteration++;
        }
        
        infile.close();
        tempfile.close();
        
        remove((pathToFolderContainingFile + fileName).c_str());
        rename((pathToFolderContainingFile + "temporary.txt").c_str(), (pathToFolderContainingFile + fileName).c_str());
    }
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

void chooseRandomRectanglesFromPartition(set<int>& randomlyChosenRectangleIDs, int numRectanglesToBeCovered, int numRectangles) {
    vector<int> rectangleIDs(numRectangles);
    iota(rectangleIDs.begin(), rectangleIDs.end(), 1);
    shuffle(rectangleIDs.begin(), rectangleIDs.end(), mt19937(random_device{}()));
    randomlyChosenRectangleIDs.insert(rectangleIDs.begin(), rectangleIDs.begin() + numRectanglesToBeCovered);
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

#pragma once

#include <set>
#include <map>
#include <queue>
#include <print>
#include <vector>
#include <random>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <filesystem>

using namespace std;


class Vertex {
    public:
    int x;
    int y;

    Vertex() {
        this->x = -1;
        this->y = -1;
    }

    Vertex(int x, int y) {
        this->x = x;
        this->y = y;
    }

    bool operator <(const Vertex &other) const {
        if (this->x != other.x) {
            return this->x < other.x;
        }

        return this->y < other.y;
    }   
};

// shared by more than one .cpp file
void printNumInstancesToConsider(int numInstances);
void printCurrentInstanceNumber(int currentInstance);
void printPercentageOfRectanglesInConsideration(float percentageToCover, int numRectanglesToBeCovered, int numRectangles);
void printPercentageToCoverInputMessage();
float getInputPercentageIfValidOrDefault100();
void printMinimumNumberOfGuardsRequired(int minNumGuardsRequired);
void printAllVerticesOutDegrees(const map<Vertex, int>& vertexOutDegree);
void printAllRectanglesBoundaryVertices(const map<int, set<Vertex>>& rectangleBoundaryVertices);
void printAllRectanglesAtVertices(const map<Vertex, set<int>>& rectanglesAtVertex);
void printAllRandomRectanglesChosenFromPartition(const set<int>& randomlyChosenRectangleIDs);
void printAllRectanglesCoveredByCurrentBestVertex(const set<int>& rectanglesCoveredByVertex);
void printAllRectanglesToUpdate(const set<int>& rectanglesToUpdate);
void processCurrentInstanceInputsAndAddToOutputFile(istream &inputFile, ofstream& outputFile, int& numRectangles, int& numRectanglesToBeCovered, map<int, set<Vertex>>& rectangleBoundaryVertices, map<Vertex, int>& vertexOutDegree, map<Vertex, set<int>>& rectanglesAtVertex);
void chooseRandomRectanglesFromPartition(set<int>& randomlyChosenRectangleIDs, int numRectanglesToBeCovered, int numRectangles);

// greedy3.cpp exclusive
void printAllRectanglesDegrees(const map<int, int>& rectangleDegree);

// greedy3v2.cpp exclusive
void printAllRectanglesDegrees(const map<int, set<int>>& rectangleDegree);

// or-tools-solver.cpp exclusive
void printVerticesSet(const set<Vertex>& verticesSet);
void printIDsToVertices(const map<int, Vertex>& idToVertex);
void printVerticesToIDs(const map<Vertex, int>& vertexToId);
void processCurrentInstanceInputsAndAddToOutputFile(istream &inputFile, ofstream& outputFile, int& numRectangles, map<int, set<Vertex>>& rectangleBoundaryVertices, set<Vertex>& verticesSet);
void printAndOrSaveToFileIDsMappingToVertices(ofstream* outputModelSolutionFile, bool printToOutput, const map<int, Vertex>& idToVertex);
void printAndOrSaveToFileModelObjective(ofstream* outputModelSolutionFile, bool printToOutput, const map<int, Vertex>& idToVertex);
void printAndOrSaveToFileModelSubjectTo(ofstream* outputModelSolutionFile, bool printToOutput, const map<int, set<Vertex>>& rectangleBoundaryVertices, const map<Vertex, int>& vertexToId);
void printAndOrSaveToFileModelBounds(ofstream* outputModelSolutionFile, bool printToOutput,const map<int, Vertex>& idToVertex);
void printAndOrSaveToFileModel(ofstream* outputModelSolutionFile, bool printToOutput, const map<int, Vertex>& idToVertex, const map<Vertex, int>& vertexToId, const map<int, set<Vertex>>& rectangleBoundaryVertices);
string printAndGetOptionFromModelAndSolutionFoundMenu();
void getModelsAndSolutionsFileName(string& fileName);
void printAndOrSaveToFileModelAndSolutionFound(int& currentInstance, string& fileName, const map<int, Vertex>& idToVertex, const map<Vertex, int>& vertexToId, const map<int, set<Vertex>>& rectangleBoundaryVertices, const set<int>& chosenVerticesIDs);

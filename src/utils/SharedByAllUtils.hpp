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
#include <print>


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

void printNumInstancesToConsider(int numInstances);
void printCurrentInstanceNumber(int currentInstance);
void getOutputFileNameInput(string& fileName, string defaultOutputFileName);
string printSavePartitionToOutputFileNameMenu(string& fileName);
void savePartitionToOutputFile(int& numInstancesAddedToOutputFile, string& fileName, const map<int, set<Vertex>>& rectangleBoundaryVertices);
void printMinimumNumberOfGuardsRequired(int minNumGuardsRequired);
void insertAtBegginingNumInstancesAddedToOutputFile(int numInstancesAddedToOutputFile, string& fileName);
void printPercentageOfRectanglesInConsideration(float percentageToCover, int numRectanglesToBeCovered, int numRectangles);
void printPercentageToCoverInputMessage();
float getInputPercentageIfValidOrDefault100();
void chooseRandomRectanglesFromPartition(set<int>& randomlyChosenIndices, int numRectanglesToBeCovered, int numRectangles);
void printAllRandomRectanglesChosenFromPartition(const set<int>& randomlyChosenRectangleIDs);
string chooseRandomPrintColor();

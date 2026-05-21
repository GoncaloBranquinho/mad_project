#pragma once

#include "SharedByAllUtils.h"

void printVerticesSet(const set<Vertex>& verticesSet);
void printIDsToVertices(const map<int, Vertex>& idToVertex);
void printVerticesToIDs(const map<Vertex, int>& vertexToId);
void processCurrentInstanceInputs(istream &inputFile, int& numRectangles, map<int, set<Vertex>>& rectangleBoundaryVertices, set<Vertex>& verticesSet);
void printAndOrSaveToFileIDsMappingToVertices(ofstream* outputModelSolutionFile, bool printToOutput, const map<int, Vertex>& idToVertex);
void printAndOrSaveToFileModelObjective(ofstream* outputModelSolutionFile, bool printToOutput, const map<int, Vertex>& idToVertex);
void printAndOrSaveToFileModelSubjectTo(ofstream* outputModelSolutionFile, bool printToOutput, const map<int, set<Vertex>>& rectangleBoundaryVertices, const map<Vertex, int>& vertexToId);
void printAndOrSaveToFileModelBounds(ofstream* outputModelSolutionFile, bool printToOutput,const map<int, Vertex>& idToVertex);
void printAndOrSaveToFileModel(ofstream* outputModelSolutionFile, bool printToOutput, const map<int, Vertex>& idToVertex, const map<Vertex, int>& vertexToId, const map<int, set<Vertex>>& rectangleBoundaryVertices);
void printAndOrSaveToFileSolutionFound(ofstream* outputModelSolutionFile, bool printToOutput, const set<int>& chosenVerticesIDs, const map<int, Vertex>& idToVertex);
string printAndGetOptionFromModelAndSolutionFoundMenu();
void printAndOrSaveToFileModelAndSolutionFound(int& currentInstance, string& fileName, const map<int, Vertex>& idToVertex, const map<Vertex, int>& vertexToId, const map<int, set<Vertex>>& rectangleBoundaryVertices, const set<int>& chosenVerticesIDs);

#include "ortools/linear_solver/linear_solver.h"
#include "../ex1/utils.h"

#include <memory>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <stdexcept>

using namespace operations_research;

void solve(istream &inputFile, int percentageToCover) {
    set<int> randomlyChosenRectangleIDs;
    set<Vertex> verticesSet;
    map<int, set<Vertex>> rectangleBoundaryVertices;
    map<int, Vertex> idToVertex;
    map<Vertex, int> vertexToId;
    
    int numRectangles;
    inputFile >> numRectangles;
    
    int numRectanglesToBeCovered = lround(numRectangles * percentageToCover / 100.0);
    printPercentageOfRectanglesInConsideration(percentageToCover, numRectanglesToBeCovered, numRectangles);
    chooseRandomRectanglesFromPartition(randomlyChosenRectangleIDs, numRectanglesToBeCovered, numRectangles);
    
    if (numRectanglesToBeCovered != numRectangles) {
        printAllRandomRectanglesChosenFromPartition(randomlyChosenRectangleIDs);
    }

    processInput(inputFile, numRectangles, randomlyChosenRectangleIDs, rectangleBoundaryVertices, verticesSet);

    int id = 1;
    for (const auto& vertexCoords : verticesSet) {
        idToVertex.emplace(id, vertexCoords);
        vertexToId.emplace(vertexCoords, id);
        id++;
    }


// OR-Tools
    // Declare Solver
    unique_ptr<MPSolver> solver(MPSolver::CreateSolver("SCIP"));

    if (!solver) {
        throw runtime_error("SCIP solver unavailable.");
    }

    const double inf = solver->infinity();

    // Decision variables
    const auto totalVertices = verticesSet.size();
    vector<MPVariable*> x(totalVertices + 1);

    for (int i = 1; i <= totalVertices; i++) {
        x[i] = solver->MakeIntVar(0.0, 1.0, "x_" + to_string(i));
    }

    // Constraints (number of constraints = number of rectangles)    
    for (const auto& entry : rectangleBoundaryVertices) {
        const auto& rectangleID = entry.first;
        const auto& verticesSet = entry.second;

        MPConstraint* const c = solver->MakeRowConstraint(1.0, inf);
        // cout << "contraint_rect" << rectangleID << ": ";
        bool printPlusSign = false;

        for (const auto& vertex : verticesSet) {
            const auto& vertexID = vertexToId.at(vertex);
            c->SetCoefficient(x[vertexID], 1);

            // if (printPlusSign) {
            //     cout << " + ";
            // } else {
            //     printPlusSign = true;
            // }

            // cout << "x[" << vertexID << "]";
        }

        // cout << " ≥ 1\n";
    }

    // Objective function definition
    MPObjective* const objective = solver->MutableObjective();
    for (int i = 1; i <= totalVertices; i++) {
        objective->SetCoefficient(x[i], 1);
    }
    objective->SetMinimization();

    // Print result
    print("\tCalculating solution, please wait...\n\t");
    const MPSolver::ResultStatus result_status = solver->Solve();
    int minNumGuardsRequired = 0;

    if (result_status == MPSolver::OPTIMAL || result_status == MPSolver::FEASIBLE) {
        // cout << "x[" << i << "] = " << x[i]->solution_value() << "\n";


        for (int i = 1; i <= totalVertices; i++) {
            if (x[i]->solution_value() > 0.0) {
                const auto& vertex = idToVertex.at(i);
                minNumGuardsRequired++;
                // cout << "Guard placed at: (" << vertex.x << ", " << vertex.y << ")\n";
            }
        }

        if (result_status == MPSolver::OPTIMAL) {
            print("Solution found is OPTIMAL → ");
        } else {
            print("Solution found is FEASIBLE → ");
        }
        printMinimumNumberOfGuardsRequired(minNumGuardsRequired);
    } else {
        print("No solution found\n");
    } 
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

    int numInstances;
    inputFile >> numInstances;
    printNumInstancesToConsider(numInstances);

    for (int currentInstance = 1; currentInstance <= numInstances; currentInstance++) {
        printCurrentInstanceNumber(currentInstance);
        printPercentageToCoverInputMessage();

        float percentageToCover = getInputPercentageIfValidOrDefault100();

        solve(inputFile, percentageToCover);
        print("\n");
    }

    inputFile.close();
}

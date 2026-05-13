#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>
#include <memory>
#include <stdexcept>
#include <utility>

#include "ortools/linear_solver/linear_solver.h"

using namespace std;
using namespace operations_research;

class VertexCoords {
    public:
    int x;
    int y;

    VertexCoords(int x, int y) {
        this->x = x;
        this->y = y;
    }

    bool operator <(const VertexCoords &other) const {
        if (this->y == other.y) {
            return this->x < other.x;
        }

        return this->y > other.y;
    }
};

void printVerticesSet(set<VertexCoords>& verticesSet) {
    
    cout << "\nSet of all vertices in the partition ordered acoording to VertexCoords' comparator\n";
    cout << "verticesSet = {";
    bool printComma = false;

    for (const auto& vertex : verticesSet) {
        
        if (printComma) {
            cout << ",";
        } else {
            printComma = true;
        }

        cout << "\n   (" << vertex.x << ", " << vertex.y << ")";
    }
    cout << "\n}\n";
}

void printAllRectanglesBoundaryVertices(const map<int, set<VertexCoords>>& rectangleBoundVertices) {

    cout << "\nBoundary vertices associated to each rectangle\n";
    cout << "rectangleBoundVertices = {\n";

    for (const auto& entry : rectangleBoundVertices) {
        const int rectangleID = entry.first;
        const auto& rectVerticesSet = entry.second;

        cout << "    " << rectangleID << ": {";
        bool printComma = false;

        for (const auto& vertex : rectVerticesSet) {
            if (printComma) {
                cout << ", ";
            } else {
                printComma = true;
            }

            cout << "(" << vertex.x << ", " << vertex.y << ")";
            printComma = true;
        }

        cout << "},\n";
    }
    cout << "}\n";
}

void printIdsToVertices(map<int, VertexCoords>& idToVertex) {
    
    cout << "\nVertex associated to each ID (all vertices / ids of the partition included)";
    cout << "\nidToVertex = {\n";
    bool printComma = false;

    for (const auto& entry : idToVertex) {
        if (printComma) {
            cout << ",\n";
        } else {
            printComma = true;
        }

        cout << "   " << entry.first << ": (" << entry.second.x << ", " << entry.second.y << ")";
    }
    cout << "\n}\n";
}

void printVerticesToIds(map<VertexCoords, int>& vertexToId) {
    
    cout << "\nID associated to each vertex (all vertices / ids of the partition included)";
    cout << "\nvertexToId = {\n";
    bool printComma = false;

    for (const auto& entry : vertexToId) {
        if (printComma) {
            cout << ",\n";
        } else {
            printComma = true;
        }

        cout << "   (" << entry.first.x << ", " << entry.first.y << "): " << entry.second;
    }
    cout << "\n}\n";
}

void solve(istream &inputFile) {
    set<VertexCoords> verticesSet;
    map<int, set<VertexCoords>> rectangleBoundVertices;
    
    int numRectangles;
    inputFile >> numRectangles;

    for (int i = 0; i < numRectangles; i++) {
        int rectangleID;
        int numVertices;
        inputFile >> rectangleID >> numVertices;

        for (int j = 0; j < numVertices; j++) {
            int x, y;
            inputFile >> x >> y;
            auto vertex = VertexCoords(x, y);
            verticesSet.insert(vertex);
            rectangleBoundVertices[rectangleID].insert(vertex);
        }
    }

    map<int, VertexCoords> idToVertex; // Only used for print / debug
    map<VertexCoords, int> vertexToId;
    int id = 1;

    for (const auto& vertexCoords : verticesSet) {
        idToVertex.emplace(id, vertexCoords);
        vertexToId.emplace(vertexCoords, id);
        id++;
    }

    // printVerticesSet(verticesSet);
    // printAllRectanglesBoundaryVertices(rectangleBoundVertices);
    // printIdsToVertices(idToVertex);
    // printVerticesToIds(vertexToId);

    // OR-Tools
    // Declare Solver
    unique_ptr<MPSolver> solver(MPSolver::CreateSolver("SCIP"));

    if (!solver) {
        throw runtime_error("SCIP solver unavailable.");
    }

    const double inf = solver->infinity();

    // Decision variables
    int totalVertices = verticesSet.size();
    vector<MPVariable*> x(totalVertices + 1);

    for (int i = 1; i <= totalVertices; i++) {
        x[i] = solver->MakeIntVar(0.0, 1.0, "x_" + to_string(i));
    }

    // Constraints (number of constraints = number of rectanles)
    int currentConstraint = 1;
    
    for (const auto& entry : rectangleBoundVertices) {
        int rectangleID = entry.first;
        const auto& verticesSet = entry.second;

        MPConstraint* const c = solver->MakeRowConstraint(1.0, inf);
        // cout << "c" << currentConstraint << ": ";
        // bool printPlusSign = false;

        for (const auto& vertex : verticesSet) {
            int vertexID = vertexToId.at(vertex);
            c->SetCoefficient(x[vertexID], 1);

            // if (printPlusSign) {
            //     cout << " + ";
            // } else {
            //     printPlusSign = true;
            // }

            // cout << "x[" << vertexID << "]";
        }

        // cout << " ≥ 1\n";
        currentConstraint++;
    }

    // Objective function definition
    MPObjective* const objective = solver->MutableObjective();
    for (int i = 1; i <= totalVertices; i++) {
        objective->SetCoefficient(x[i], 1);
    }
    objective->SetMinimization();

    // Print result
    const MPSolver::ResultStatus result_status = solver->Solve();
    int numGuardsPlaced = 0;

    if (result_status == MPSolver::OPTIMAL) {
        // cout << "x[" << i << "] = " << x[i]->solution_value() << "\n";

        for (int i = 1; i <= totalVertices; i++) {
            if (x[i]->solution_value() > 0.0) {
                auto vertex = idToVertex.at(i);
                numGuardsPlaced++;
                // cout << "Guard placed at: (" << vertex.x << ", " << vertex.y << ")\n";
            }
        }
        cout << "Total number of guards: " << numGuardsPlaced << "\n";
    } else {
        cout << "Solution found is not optimal.";
    } 
}


int main(int argc, char *argv[]) {
    ifstream inputFile(argv[1]);

    if (!inputFile.is_open()) {
        return 1;
    }

    int numInstances;
    inputFile >> numInstances;

    for (int i = 0; i < numInstances; i++) {
        solve(inputFile);
    }

    inputFile.close();
}
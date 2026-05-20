#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>

#include "absl/base/log_severity.h"
#include "absl/log/globals.h"
#include "ortools/base/init_google.h"
#include "ortools/base/logging.h"
#include "ortools/sat/cp_model.h"
#include "ortools/sat/cp_model.pb.h"
#include "ortools/sat/cp_model_solver.h"
#include "ortools/util/sorted_interval_list.h"
#include <stdlib.h>
#include <vector>

using namespace operations_research;
using namespace sat;

std::unordered_map<int, std::set<int>> vertices;
std::unordered_map<int, std::set<int>> rectangles;

void solve(std::istream &f) {
  vertices.clear();
  rectangles.clear();
  std::map<std::pair<int, int>, int> pair_to_id;
  std::unordered_map<int, std::pair<int, int>> id_to_pair;
  CpModelBuilder cp_model;
  const Domain domain(0, 2);
  int n_rect;
  int n_vert;
  int last = 1;

  int n;
  f >> n;

  for (int i = 0; i < n; i++) {
    int id;
    int m;
    f >> id >> m;
    for (int j = 0; j < m; j++) {
      int x, y;
      f >> x >> y;
      auto p = std::make_pair(x, y);
      int pair_id;
      if (pair_to_id.find(p) == pair_to_id.end()) {
        pair_id = last++;
        pair_to_id[p] = pair_id;
        id_to_pair[pair_id] = p;
      } else {
        pair_id = pair_to_id[p];
      }
      vertices[id].insert(pair_id);
      rectangles[pair_id].insert(id);
    }
  }

  std::vector<IntVar> cp_vars(rectangles.size() + 1);
  LinearExpr expr;
  for (int i = 1; i <= rectangles.size(); i++) {
    cp_vars[i] = cp_model.NewIntVar(domain).WithName(std::to_string(i));
    expr += cp_vars[i];
  }
  cp_model.Minimize(expr);

  for (auto r : vertices) {
    LinearExpr expr;
    for (auto v : r.second) {
      expr += cp_vars[v];
    }
    cp_model.AddGreaterOrEqual(expr, 1);
  }

  const CpSolverResponse response = Solve(cp_model.Build());

  int numGuardsPlaced = 0;

  if (response.status() == CpSolverStatus::OPTIMAL) {

    std::cout << "Total number of guards: " << response.objective_value()
              << "\n";
  } else {
    std::cout << "Solution found is not optimal." << "\n";
  }
}
int main(int argc, char *argv[]) {
  std::ifstream f(argv[1]);
  if (!f.is_open()) {
    return 1;
  }
  int instances;
  f >> instances;
  for (int i = 0; i < instances; i++)
    solve(f);
  f.close();
}

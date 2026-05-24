#include <algorithm>
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
std::map<std::pair<int, int>, int> pair_to_id;
std::unordered_map<int, std::pair<int, int>> id_to_pair;

void solve2(int sol) {
  CpModelBuilder cp_model;
  int max_verts = 0;
  for (auto r : vertices) {
    if (r.second.size() > max_verts) {
      max_verts = r.second.size();
    }
  }
  const Domain domain(0, 1);
  std::vector<IntVar> v(rectangles.size() + 1);
  std::vector<IntVar> c(max_verts + 1);
  std::vector<std::vector<IntVar>> u(rectangles.size() + 1,
                                     std::vector<IntVar>(max_verts + 1));
  LinearExpr expr;
  for (int i = 1; i <= max_verts; i++) {
    c[i] = cp_model.NewIntVar(domain).WithName("c_" + std::to_string(i));
    expr += c[i];
  }
  cp_model.Minimize(expr);

  expr = LinearExpr();
  for (int i = 1; i <= rectangles.size(); i++) {
    v[i] = cp_model.NewIntVar(domain).WithName("v_" + std::to_string(i));
    expr += v[i];
    LinearExpr expr1;
    for (int j = 1; j <= max_verts; j++) {
      u[i][j] = cp_model.NewIntVar(domain).WithName("u_" + std::to_string(i) +
                                                    "_" + std::to_string(j));
      expr1 += u[i][j];
      cp_model.AddGreaterOrEqual(c[j], u[i][j]);
    }
    cp_model.AddEquality(expr1, v[i]);
  }
  cp_model.AddEquality(expr, sol);
  for (auto r : vertices) {
    expr = LinearExpr();
    for (auto vert : r.second) {
      expr += v[vert];
    }
    cp_model.AddGreaterOrEqual(expr, 1);
  }
  for (int color = 1; color < c.size(); color++) {
    for (auto r : vertices) {
      expr = LinearExpr();
      for (auto vert : r.second) {
        expr += u[vert][color];
      }
      cp_model.AddGreaterOrEqual(1, expr);
    }
  }

  for (int color = 1; color < c.size(); color++) {
    expr = LinearExpr();

    for (int i = 1; i <= rectangles.size(); i++) {
      expr += u[i][color];
    }
    cp_model.AddGreaterOrEqual(expr, c[color]);
  }

  const CpSolverResponse response = Solve(cp_model.Build());

  if (response.status() == CpSolverStatus::OPTIMAL) {
    std::cout << "Total number of colors: " << response.objective_value()
              << "\n";
  } else {
    std::cout << "Solution found is not optimal." << "\n";
  }
}

void solve1(std::istream &f) {
  vertices.clear();
  rectangles.clear();
  pair_to_id.clear();
  id_to_pair.clear();
  CpModelBuilder cp_model;
  const Domain domain(0, 1);
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

  std::vector<IntVar> v(rectangles.size() + 1);
  LinearExpr expr;
  for (int i = 1; i <= rectangles.size(); i++) {
    v[i] = cp_model.NewIntVar(domain).WithName(std::to_string(i));
    expr += v[i];
  }
  cp_model.Minimize(expr);

  for (auto r : vertices) {
    LinearExpr expr;
    for (auto vert : r.second) {
      expr += v[vert];
    }
    cp_model.AddGreaterOrEqual(expr, 1);
  }

  const CpSolverResponse response = Solve(cp_model.Build());

  int numGuardsPlaced = 0;

  if (response.status() == CpSolverStatus::OPTIMAL) {
    solve2(response.objective_value());
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
    solve1(f);
  f.close();
}

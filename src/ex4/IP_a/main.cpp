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
#include "ortools/linear_solver/linear_solver.h"
#include <stdlib.h>
#include <vector>

using namespace operations_research;

std::unordered_map<int, std::set<int>> vertices;
std::unordered_map<int, std::set<int>> rectangles;
std::map<std::pair<int, int>, int> pair_to_id;
std::unordered_map<int, std::pair<int, int>> id_to_pair;

void solve2(int sol) {
  std::unique_ptr<MPSolver> solver(MPSolver::CreateSolver("SCIP"));

  if (!solver) {
    throw std::runtime_error("SCIP solver unavailable.");
  };

  int max_verts = 0;
  for (auto r : vertices) {
    if (r.second.size() > max_verts) {
      max_verts = r.second.size();
    }
  }
  std::vector<MPVariable *> v(rectangles.size() + 1);
  std::vector<MPVariable *> c(max_verts + 1);
  std::vector<std::vector<MPVariable *>> u(
      rectangles.size() + 1, std::vector<MPVariable *>(max_verts + 1));
  MPObjective *obj = solver->MutableObjective();

  for (int i = 1; i <= max_verts; i++) {
    c[i] = solver->MakeBoolVar("c_" + std::to_string(i));
    obj->SetCoefficient(c[i], 1);
  }
  obj->SetMinimization();

  MPConstraint *total = solver->MakeRowConstraint(sol, sol);
  for (int i = 1; i <= rectangles.size(); i++) {
    v[i] = solver->MakeBoolVar("v_" + std::to_string(i));
    total->SetCoefficient(v[i], 1);
    MPConstraint *expr = solver->MakeRowConstraint(0.0, 0.0);
    for (int j = 1; j <= max_verts; j++) {
      u[i][j] = solver->MakeBoolVar("u_" + std::to_string(i) + "_" +
                                    std::to_string(j));
      expr->SetCoefficient(u[i][j], 1);
      MPConstraint *expr1 = solver->MakeRowConstraint(0.0, solver->infinity());
      expr1->SetCoefficient(c[j], 1);
      expr1->SetCoefficient(u[i][j], -1);
    }
    expr->SetCoefficient(v[i], -1);
  }

  for (auto r : vertices) {
    MPConstraint *expr = solver->MakeRowConstraint(1.0, solver->infinity());
    for (auto vert : r.second) {
      expr->SetCoefficient(v[vert], 1);
    }
  }
  for (int color = 1; color < c.size(); color++) {
    for (auto r : vertices) {
      MPConstraint *expr = solver->MakeRowConstraint(-solver->infinity(), 1.0);
      for (auto vert : r.second) {
        expr->SetCoefficient(u[vert][color], 1);
      }
    }
  }

  for (int color = 1; color < c.size(); color++) {
    MPConstraint *expr = solver->MakeRowConstraint(0.0, solver->infinity());

    for (int i = 1; i <= rectangles.size(); i++) {
      expr->SetCoefficient(u[i][color], 1);
    }
    expr->SetCoefficient(c[color], -1);
  }

  const MPSolver::ResultStatus response = solver->Solve();

  if (response == MPSolver::OPTIMAL) {
    std::cout << "Total number of colors: " << obj->Value() << "\n";

  } else {
    std::cout << "Solution found is not optimal." << "\n";
  }
}

void solve1(std::istream &f) {
  vertices.clear();
  rectangles.clear();
  pair_to_id.clear();
  id_to_pair.clear();
  std::unique_ptr<MPSolver> solver(MPSolver::CreateSolver("SCIP"));

  if (!solver) {
    throw std::runtime_error("SCIP solver unavailable.");
  };
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

  std::vector<MPVariable *> v(rectangles.size() + 1);
  MPObjective *obj = solver->MutableObjective();
  for (int i = 1; i <= rectangles.size(); i++) {
    v[i] = solver->MakeBoolVar(std::to_string(i));
    obj->SetCoefficient(v[i], 1);
  }
  obj->SetMinimization();

  for (auto r : vertices) {
    MPConstraint *const expr =
        solver->MakeRowConstraint(1.0, solver->infinity());
    for (auto vert : r.second) {
      expr->SetCoefficient(v[vert], 1);
    }
  }

  const MPSolver::ResultStatus response = solver->Solve();

  if (response == MPSolver::OPTIMAL) {
    solve2(obj->Value());
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

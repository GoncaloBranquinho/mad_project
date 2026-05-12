#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "ortools/linear_solver/linear_solver.h"
using namespace operations_research;

class Rectangle;
std::unordered_map<int, std::shared_ptr<Rectangle>> rects;
std::map<std::pair<int, int>, bool> guards;
std::map<std::pair<int, int>, std::vector<int>> Points;
class Rectangle {
 public:
  const int id = 0;
  int outdegree = 0;
  Rectangle(const int id) : id(id) {}
  std::vector<std::pair<int, int>> points;
  std::vector<int> out;
  void addPoint(const int x, const int y) {
    if (std::find(points.begin(), points.end(), std::pair{x, y}) ==
        points.end())
      this->points.emplace_back(std::make_pair(x, y));
    for (const auto& [rId, r] : rects) {
      if (rId == this->id) continue;
      if (r->contains(std::pair{x, y})) this->addOut(rId);
    }
  }
  void addOut(const int r) {
    if (std::find(out.begin(), out.end(), r) == out.end()) {
      outdegree++;
      this->out.emplace_back(r);
      rects[r]->addOut(this->id);
    }
  }
  bool contains(const std::pair<int, int>& p) const {
    if (std::find(points.begin(), points.end(), p) != points.end()) return true;
    return false;
  }
  int intersect(const std::pair<int, int> p, const Rectangle& r) {
    if (r.contains(p)) return r.id;
    return -1;
  }

  int numGuards() {
    int sum = 0;
    for (const auto& p : points)
      if (guards[p]) sum++;

    if (sum == 0) throw std::runtime_error("WTF\n");
    return sum;
  }
};

bool canRemove(const std::pair<int, int>& p) {
  for (const auto& id : Points[p])
    if (rects[id]->numGuards() - 1 == 0) return false;
  return true;
}
void removePoint(const std::pair<int, int>& p) {
  if (canRemove(p)) guards[p] = false;
}
struct ComparePoints {
  bool operator()(const std::pair<int, int>& x,
                  const std::pair<int, int>& y) const {
    if (Points[x].size() != Points[y].size())
      return Points[x].size() > Points[y].size();
    return x > y;
  }
};

void solve(std::istream& f) {
  rects.clear();
  guards.clear();
  Points.clear();
  std::unique_ptr<MPSolver> solver(MPSolver::CreateSolver("SCIP"));
  if (!solver) throw std::runtime_error("Can't allocate solver\n");

  int n;
  f >> n;

  for (int i = 0; i < n; i++) {
    int id;
    int m;
    f >> id >> m;
    rects[id] = std::make_shared<Rectangle>(Rectangle(id));
    for (int j = 0; j < m; j++) {
      int x, y;
      f >> x >> y;

      rects[id]->addPoint(x, y);
      guards[std::pair{x, y}] = true;
    }
  }
  for (const auto& [rId, r] : rects)
    for (const auto& p : r->points) Points[p].emplace_back(rId);

  const MPSolver::ResultStatus result_status = solver->Solve();

  MPObjective* const objective = solver->MutableObjective();
  std::vector<MPVariable*> vars(Points.size(), nullptr);
  for (int i = 0; i < Points.size(); i++)
    vars[i] = solver->MakeIntVar(0.0, 1.0, "x_" + std::to_string(i));

  for (int i = 1; i <= 8; ++i) objective->SetCoefficient(vars[i], 1.0);

  objective->SetMinimization();

  // 6. Imprimir os resultados
  if (result_status == MPSolver::OPTIMAL) {
    std::cout << "Optimal cost: " << objective->Value() << std::endl;
    for (int i = 1; i <= 8; ++i) {
      if (vars[i]->solution_value() > 0.5) {
        std::cout << "Guard at node " << i << std::endl;
      }
    }
  } else {
    std::cout << "No optimal solution found." << std::endl;
  }
}

int main() {
  // std::unique_ptr<MPSolver> solver(MPSolver::CreateSolver("SCIP"));
  // if (!solver) {
  //   std::cerr << "Solver SCIP não está disponível." << std::endl;
  //   return -1;
  // }
  // std::vector<MPVariable*> vars(9, nullptr);
  // for (int i = 1; i <= 8; ++i) {
  //   vars[i] = solver->MakeIntVar(0.0, 1.0, "x_" + std::to_string(i));
  // }
  // MPConstraint* c1 = solver->MakeRowConstraint(1.0, solver->infinity());
  // c1->SetCoefficient(vars[8], 1.0);
  //
  // MPConstraint* c2 = solver->MakeRowConstraint(1.0, solver->infinity());
  // c2->SetCoefficient(vars[8], 1.0);
  // c2->SetCoefficient(vars[7], 1.0);
  //
  // MPConstraint* c3 = solver->MakeRowConstraint(1.0, solver->infinity());
  // c3->SetCoefficient(vars[7], 1.0);
  // c3->SetCoefficient(vars[6], 1.0);
  // c3->SetCoefficient(vars[4], 1.0);
  // c3->SetCoefficient(vars[5], 1.0);
  //
  // MPConstraint* c4 = solver->MakeRowConstraint(1.0, solver->infinity());
  // c4->SetCoefficient(vars[7], 1.0);
  // c4->SetCoefficient(vars[6], 1.0);
  // c4->SetCoefficient(vars[8], 1.0);
  //
  // MPConstraint* c5 = solver->MakeRowConstraint(1.0, solver->infinity());
  // c5->SetCoefficient(vars[3], 1.0);
  // c5->SetCoefficient(vars[4], 1.0);
  //
  // MPConstraint* c6 = solver->MakeRowConstraint(1.0, solver->infinity());
  // c6->SetCoefficient(vars[2], 1.0);
  // c6->SetCoefficient(vars[1], 1.0);
  //
  // MPConstraint* c7 = solver->MakeRowConstraint(1.0, solver->infinity());
  // c7->SetCoefficient(vars[5], 1.0);
  // c7->SetCoefficient(vars[4], 1.0);
  // c7->SetCoefficient(vars[3], 1.0);
  // c7->SetCoefficient(vars[2], 1.0);
  //
  // MPConstraint* c8 = solver->MakeRowConstraint(1.0, solver->infinity());
  // c8->SetCoefficient(vars[5], 1.0);
  // c8->SetCoefficient(vars[6], 1.0);
  //
  // MPConstraint* c9 = solver->MakeRowConstraint(1.0, solver->infinity());
  // c9->SetCoefficient(vars[1], 1.0);
  // c9->SetCoefficient(vars[3], 1.0);
  // c9->SetCoefficient(vars[2], 1.0);
  //
  // MPConstraint* c10 = solver->MakeRowConstraint(1.0, solver->infinity());
  // c10->SetCoefficient(vars[1], 1.0);
  //
  // // 4. Função Objetivo: minimizar o custo (soma de todos os guardas)
  // MPObjective* const objective = solver->MutableObjective();
  // for (int i = 1; i <= 8; ++i) {
  //   objective->SetCoefficient(vars[i], 1.0);
  // }
  // objective->SetMinimization();
  //
  // // 5. Resolver o problema
  // const MPSolver::ResultStatus result_status = solver->Solve();
  //
  // // 6. Imprimir os resultados
  // if (result_status == MPSolver::OPTIMAL) {
  //   std::cout << "Optimal cost: " << objective->Value() << std::endl;
  //   for (int i = 1; i <= 8; ++i) {
  //     if (vars[i]->solution_value() > 0.5) {
  //       std::cout << "Guard at node " << i << std::endl;
  //     }
  //   }
  // } else {
  //   std::cout << "No optimal solution found." << std::endl;
  // }
  // return 0;

  std::ifstream f(argv[1]);
  if (!f) {
    return 1;
  }
  int instances;
  f >> instances;
  for (int i = 0; i < instances; i++) solve(f);
  f.close();
}

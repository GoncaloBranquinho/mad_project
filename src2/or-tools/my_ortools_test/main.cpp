#include <fstream>
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
std::map<std::pair<int, int>, std::vector<int>> Points;
std::map<std::pair<int, int>, int> PointsId;
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
    for (const auto &[rId, r] : rects) {
      if (rId == this->id)
        continue;
      if (r->contains(std::pair{x, y}))
        this->addOut(rId);
    }
  }
  void addOut(const int r) {
    if (std::find(out.begin(), out.end(), r) == out.end()) {
      outdegree++;
      this->out.emplace_back(r);
      rects[r]->addOut(this->id);
    }
  }
  bool contains(const std::pair<int, int> &p) const {
    if (std::find(points.begin(), points.end(), p) != points.end())
      return true;
    return false;
  }
  int intersect(const std::pair<int, int> p, const Rectangle &r) {
    if (r.contains(p))
      return r.id;
    return -1;
  }
  const std::vector<int> &adjacent() { return this->out; }
};

void solve(std::istream &f) {
  rects.clear();
  Points.clear();
  PointsId.clear();
  std::unique_ptr<MPSolver> solver(MPSolver::CreateSolver("SCIP"));
  if (!solver)
    throw std::runtime_error("Can't allocate solver\n");

  int idx = 0;
  int n;
  f >> n;

  for (int i = 0; i < n; i++) {
    int id;
    int m;
    f >> id >> m;
    id--;
    rects[id] = std::make_shared<Rectangle>(Rectangle(id));
    for (int j = 0; j < m; j++) {
      int x, y;
      f >> x >> y;

      rects[id]->addPoint(x, y);
      if (PointsId.find({x, y}) == PointsId.end())
        PointsId[{x, y}] = idx++;
    }
  }
  for (const auto &[rId, r] : rects)
    for (const auto &p : r->points)
      Points[p].emplace_back(rId);

  MPObjective *const objective = solver->MutableObjective();
  std::vector<MPVariable *> vars(PointsId.size(), nullptr);

  for (const auto &[p, id] : PointsId)
    vars[id] = solver->MakeIntVar(0.0, 1.0, "x_" + std::to_string(id));

  for (const auto &[rId, r] : rects) {
    MPConstraint *c = solver->MakeRowConstraint(1.0, solver->infinity());
    for (const auto &p : r->points)
      c->SetCoefficient(vars[PointsId[p]], 1.0);
  }

  for (const auto &[p, id] : PointsId)
    objective->SetCoefficient(vars[id], 1.0);

  objective->SetMinimization();

  const MPSolver::ResultStatus resultStatus = solver->Solve();
  if (resultStatus == MPSolver::OPTIMAL) {
    for (const auto &[rId, r] : rects)
      if (vars[rId]->solution_value() > 0.5)
        std::cout << "Guard at node " << rId + 1 << '\n';
    std::cout << "Optimal cost: " << objective->Value() << '\n';
  } else
    std::cout << "No optimal solution found." << '\n';
}

int main(int argc, char *argv[]) {
  std::ifstream f(argv[1]);
  if (!f)
    return 1;

  int instances;
  f >> instances;
  for (int i = 0; i < instances; i++)
    solve(f);
  f.close();
}

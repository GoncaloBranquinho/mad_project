#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>
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

  int numGuards() {
    int sum = 0;
    for (const auto &p : points)
      if (guards[p])
        sum++;

    if (sum == 0)
      throw std::runtime_error("WTF\n");
    return sum;
  }
};

bool canRemove(const std::pair<int, int> &p) {
  for (const auto &id : Points[p])
    if (rects[id]->numGuards() - 1 == 0)
      return false;
  return true;
}
void removePoint(const std::pair<int, int> &p) {
  if (canRemove(p))
    guards[p] = false;
}
struct ComparePoints {
  bool operator()(const std::pair<int, int> &x,
                  const std::pair<int, int> &y) const {
    if (Points[x].size() != Points[y].size())
      return Points[x].size() > Points[y].size();
    return x < y;
  }
};

void solve(std::istream &f) {
  rects.clear();
  guards.clear();
  Points.clear();

  int n;
  f >> n;

  std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>,
                      ComparePoints>
      queue;
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
  for (const auto &[rId, r] : rects)
    for (const auto &p : r->points)
      Points[p].emplace_back(rId);

  for (const auto &[p, r] : Points)
    queue.emplace(p);

  while (!queue.empty()) {
    removePoint(queue.top());
    queue.pop();
  }
  std::cout << std::accumulate(
                   guards.begin(), guards.end(), 0,
                   [](int sum, const auto &p) { return sum + p.second; })
            << '\n';
}
int main(int argc, char *argv[]) {
  std::ifstream f(argv[1]);
  if (!f) {
    return 1;
  }
  int instances;
  f >> instances;
  for (int i = 0; i < instances; i++)
    solve(f);
  f.close();
}

#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <unordered_map>
#include <utility>

class Vertex {

public:
  int x;
  int y;
  int outdegree;

  Vertex(int x, int y, int outdegree) {
    this->x = x;
    this->y = y;
    this->outdegree = outdegree;
  }

  bool operator<(const Vertex &v) const {
    if (this->outdegree == v.outdegree) {
      if (this->x == v.x) {
        return this->y > v.y;
      }
      return this->x < v.x;
    }
    return this->outdegree < v.outdegree;
  }
};

void solve(std::istream &f) {
  std::unordered_map<int, std::set<std::pair<int, int>>> vertices;
  std::map<std::pair<int, int>, std::set<int>> rectangles;
  std::map<std::pair<int, int>, int> outdegrees;
  std::map<std::pair<int, int>, int> curr_outdegree;
  std::priority_queue<Vertex> queue;

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
      outdegrees[p] += 1;
      vertices[id].insert(p);
      rectangles[p].insert(id);
    }
  }

  for (auto v : outdegrees) {
    int x = v.first.first;
    int y = v.first.second;
    curr_outdegree[v.first] = v.second;
    queue.push(Vertex(x, y, v.second));
  }
  int sol = 0;
  while (!queue.empty()) {
    auto v = queue.top();
    queue.pop();
    auto v_pair = std::make_pair(v.x, v.y);
    if (curr_outdegree[v_pair] != v.outdegree || v.outdegree == 0)
      continue;
    sol += 1;
    for (auto r : rectangles[v_pair]) {
      vertices[r].erase({v.x, v.y});
      for (auto other_v : vertices[r]) {
        int x = other_v.first;
        int y = other_v.second;
        auto p = std::make_pair(x, y);
        outdegrees[p] -= 1;
        rectangles[p].erase(r);
        curr_outdegree[p] = outdegrees[p];
        queue.push(Vertex(x, y, outdegrees[p]));
      }
    }
  }
  std::cout << sol << '\n';
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

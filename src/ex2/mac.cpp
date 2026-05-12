#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>

std::unordered_map<int, std::set<int>> vertices;
std::unordered_map<int, std::set<int>> rectangles;

int compute(std::vector<int> solution) {
  int value = 0;
  for (int i = 1; i <= rectangles.size(); i++) {
    value += solution[i];
  }
  return value;
}

bool revise(std::unordered_map<int, std::set<int>> &domains, int x, int r,
            std::unordered_map<int, std::pair<int, int>> rect_sum, int m) {
  bool change = false;
  auto dom = domains[x];
  for (auto val : dom) {
    int sum = val;
    if (r == -1) {
      sum = rect_sum[r].first;
      if (sum > m) {
        change = true;
        domains[x].erase(val);
        std::cout << "olaaaaaaaaaaa";
      }
    } else {
      sum += (rect_sum[r].first + (rectangles.size() - rect_sum[r].second));
      if (sum == 0) {
        change = true;
        domains[x].erase(val);
        std::cout << "olaaaaaaaaaaa";
      }
    }
  }
  return change;
}

bool ac3(std::vector<int> &solution,
         std::unordered_map<int, std::set<int>> &domains,
         std::set<std::pair<int, int>> aux_constraints,
         std::unordered_map<int, std::pair<int, int>> rect_sum, int x, int m) {
  std::set<std::pair<int, int>> constraints = aux_constraints;
  for (auto r : rectangles[x]) {
    for (auto v : vertices[r])
      constraints.insert(std::make_pair(v, r));
  }
  while (!constraints.empty()) {
    auto [i, c] = *constraints.begin();
    constraints.erase(constraints.begin());
    if (revise(domains, i, c, rect_sum, m)) {
      if (domains[i].empty())
        return false;
      for (auto r : rectangles[i]) {
        if (r == c)
          continue;
        for (auto v : rectangles[r]) {
          if (v != i) {
            constraints.insert(std::make_pair(v, r));
            constraints.insert(std::make_pair(v, -1));
          }
        }
      }
    }
  }
  return true;
}

bool consistent(std::vector<int> &solution, int x,
                std::unordered_map<int, std::pair<int, int>> rect_sum, int m) {
  for (auto r : rectangles[x]) {
    bool flag = true;
    int sum = 0;
    /*for (auto v : vertices[r]) {
      if (solution[v] != -1) {
        sum += solution[v];
      } else if (v != x) {
        flag = false;
        break;
      }
    }*/
    sum += rect_sum[r].first;
    if (rect_sum[r].second == vertices[r].size() && sum == 0) {
      return false;
    }
  }
  if (rect_sum[-1].first > m) {
    return false;
  }
  /*for (int i = 1; i <= rectangles.size(); i++) {
    if (solution[i] != -1) {
      sum += solution[i];
    }
    if (sum > m) {
      return false;
    }
  }*/
  return true;
}

bool bt(std::vector<int> &solution, std::vector<int> &vars,
        std::unordered_map<int, std::set<int>> &domains,
        std::set<std::pair<int, int>> constraints,
        std::unordered_map<int, std::pair<int, int>> rect_sum, int m) {
  if (vars.empty())
    return true;
  auto next_vars = vars;
  int x = next_vars.back();
  next_vars.pop_back();
  for (auto a : domains[x]) {
    std::unordered_map<int, std::set<int>> aux_domains = domains;
    int old_val = solution[x];
    solution[x] = a;
    for (auto r : rectangles[x]) {
      rect_sum[r].first += a;
      rect_sum[r].second += 1;
    }
    rect_sum[-1].first += a;
    if (consistent(solution, x, rect_sum, m)) {
      if (ac3(solution, aux_domains, constraints, rect_sum, x, m)) {
        bool flag =
            bt(solution, next_vars, aux_domains, constraints, rect_sum, m);
        if (flag) {
          return true;
        }
      }
    }
    solution[x] = old_val;
    for (auto r : rectangles[x]) {
      rect_sum[r].first -= a;
      rect_sum[r].second -= 1;
    }
    rect_sum[-1].first -= a;
  }
  return false;
}

void solve(std::istream &f) {
  vertices.clear();
  rectangles.clear();
  std::map<std::pair<int, int>, int> pair_to_id;
  std::unordered_map<int, std::pair<int, int>> id_to_pair;
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

  int l = 0;
  int r = vertices.size() + 1;

  std::unordered_map<int, std::set<int>> domains;
  std::vector<int> solution;
  std::vector<int> vars;
  std::set<std::pair<int, int>> constraints;
  std::unordered_map<int, std::pair<int, int>> rect_sum;

  for (auto v : rectangles) {
    constraints.insert(std::make_pair(v.first, -1));
  }

  solution.push_back(-1);
  for (int i = 1; i <= rectangles.size(); i++) {
    domains[i] = {0, 1};
    solution.push_back(-1);
    vars.push_back(i);
  }

  for (auto r : vertices) {
    rect_sum[r.first].first = 0;
    rect_sum[r.first].second = 0;
  }
  rect_sum[-1].first = 0;

  while (l < r) {
    int m = l + (r - l) / 2;
    auto aux_solution = solution;
    auto aux_domains = domains;
    auto aux_rect_sum = rect_sum;
    bool flag =
        bt(aux_solution, vars, aux_domains, constraints, aux_rect_sum, m);
    if (!flag) {
      l = m + 1;
    } else {
      r = compute(aux_solution);
    }
  }

  std::cout << l << '\n';
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

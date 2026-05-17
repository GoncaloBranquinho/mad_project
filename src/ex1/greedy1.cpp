#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <vector>
#include <unordered_map>
#include <utility>
#include <numeric>
#include <random>
#include <algorithm>

using namespace std;

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

void printAllRectanglesChosenFromPartition(const set<int>& randomlyChosenRectangleIDs) {

    cout << "Randomly chosen subset of rectangles of the partition: {";

    bool printComma = false;
    for (const auto& rectangleID : randomlyChosenRectangleIDs) {
        
        if (printComma) {
            cout << ", ";
        } else {
            printComma = true;
        }

        cout << rectangleID;
    }

    cout << "}\n";
}

void chooseRandomRectanglesFromPartition(set<int>& randomlyChosenRectangleIDs, int numRectanglesToBeCovered, int numRectangles) {
    vector<int> rectangleIDs(numRectangles);
    iota(rectangleIDs.begin(), rectangleIDs.end(), 1);
    shuffle(rectangleIDs.begin(), rectangleIDs.end(), mt19937(random_device{}()));
    randomlyChosenRectangleIDs.insert(rectangleIDs.begin(), rectangleIDs.begin() + numRectanglesToBeCovered);
}


void solve(istream &f, int percentageToCover) {
    set<int> randomlyChosenRectangleIDs;
    map<pair<int, int>, int> outdegrees;
    map<pair<int, int>, int> curr_outdegree;
    unordered_map<int, set<pair<int, int>>> vertices;
    map<pair<int, int>, set<int>> rectangles;
    priority_queue<Vertex> queue;

    int n;
    f >> n;

    int numRectanglesToBeCovered = lround(n * percentageToCover / 100.0);
    cout << "Considering " << numRectanglesToBeCovered << " / " << n << " of the partition's rectangles to be covered\n";
    chooseRandomRectanglesFromPartition(randomlyChosenRectangleIDs, numRectanglesToBeCovered, n);
    
    if (numRectanglesToBeCovered == n) {
        cout << "All rectangles of the partition included\n";
    } else {
        printAllRectanglesChosenFromPartition(randomlyChosenRectangleIDs);
    }

    for (int i = 0; i < n; i++) {
        int id;
        int m;
        f >> id >> m;

        bool rectangleIsInPartitionSubset = randomlyChosenRectangleIDs.contains(id);

        for (int j = 0; j < m; j++) {
            int x, y;
            f >> x >> y;

            if (rectangleIsInPartitionSubset) {
                auto p = make_pair(x, y);
                outdegrees[p] += 1;
                vertices[id].insert(p);
                rectangles[p].insert(id);
            }
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
        auto v = queue.top(); queue.pop();
        auto v_pair = make_pair(v.x, v.y);

        if (curr_outdegree[v_pair] != v.outdegree)
            continue;
        sol += 1;

        for (auto r : rectangles[v_pair]) {
            vertices[r].erase({v.x, v.y});
            for (auto other_v : vertices[r]) {
                int x = other_v.first;
                int y = other_v.second;
                auto p = make_pair(x, y);
                outdegrees[p] -= 1;
                rectangles[p].erase(r);
                curr_outdegree[p] = outdegrees[p];
                if (outdegrees[p] > 0)
                    queue.push(Vertex(x, y, outdegrees[p]));
            }
        }
    }

    cout << "Total number of guards: " << sol << "\n";
}


int main(int argc, char *argv[]) {

    if (argc < 2) {
        cout << "Insufficient arguments\n";
        return 1;
    }

    ifstream f(argv[1]);

    if (!f.is_open()) {
        cout << "Unable to open file " << argv[1] << "\n";
        return 1;
    }

    int percentageToCover = 100;

    if (argc >= 3) {
        percentageToCover = stoi(argv[2]);

        if (percentageToCover < 0 || percentageToCover > 100) {
            cout << "Invalid percentage\n";
            return 1;
        }
    }

    int numInstances;
    f >> numInstances;

    for (int i = 0; i < numInstances; i++) {
        solve(f, percentageToCover);
    }

    f.close();
}

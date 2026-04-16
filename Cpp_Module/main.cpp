#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "graph.h"
#include "heuristics.h"
using namespace std;

int main() {
    // --- Read edges from edges.txt ---
    ifstream edgeFile("../data/edges.txt");
    if (!edgeFile.is_open()) {
        cerr << "Error: Could not open ../data/edges.txt\n";
        return 1;
    }

    vector<pair<int,int>> edges;
    int maxNode = 0;
    int u, v;
    while (edgeFile >> u >> v) {
        edges.push_back({u, v});
        maxNode = max(maxNode, max(u, v));
    }
    edgeFile.close();

    int n = maxNode + 1;
    Graph g(n);
    for (auto &e : edges) {
        g.addEdge(e.first, e.second);
    }

    // --- Read labels from labels.csv ---
    ifstream labelFile("../data/labels.csv");
    if (!labelFile.is_open()) {
        cerr << "Error: Could not open ../data/labels.csv\n";
        return 1;
    }

    vector<int> labels(n, 0);
    string line;
    getline(labelFile, line);  // skip header row ("node,label")

    while (getline(labelFile, line)) {
        stringstream ss(line);
        string nodeStr, labelStr;
        getline(ss, nodeStr, ',');
        getline(ss, labelStr, ',');
        int node = stoi(nodeStr);
        int label = stoi(labelStr);
        if (node < n) {
            labels[node] = label;
        }
    }
    labelFile.close();

    // --- Run heuristics ---
    Heuristics h(g, labels);

    cout << "Heuristic Fraud Scores:\n";

    for (int i = 0; i < n; i++) {
        double score = h.riskScore(i);
        cout << "Node " << i << " Score: " << score;

        if (score > 2.0) cout << " -> FRAUD\n";
        else cout << " -> NORMAL\n";
    }

    return 0;
}
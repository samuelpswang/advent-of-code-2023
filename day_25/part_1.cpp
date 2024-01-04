// File Name:       day_25/part_1.cpp
// Author:          Samuel Wang (@samuelpswang)
// Purpose:         AoC 2023 Day 25 Part 1 (https://adventofcode.com/2023/day/25)
// Last Updated:    04 Jan 2023

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
using namespace std;

// Stoer-Wagner Global Minimum Cut Algorithm -
// via Wikipedia: https://en.wikipedia.org/wiki/Stoer%E2%80%93Wagner_algorithm
pair<int, vector<int>> stoer_wagner_min_cut(vector<vector<int>> mat) {
    pair<int, vector<int>> best = {INT_MAX, {}};
    int n = mat.size();

    vector<vector<int>> co(n); // "co" = cut of
    for (int i = 0; i < n; i++) co[i] = {i};

    for (int ph = 1; ph < n; ph++) { // "ph" = phase
        vector<int> w = mat[0]; // weights of the supernode
        size_t s = 0, t = 0;
        for (int it = 0; it < n - ph; it++) { // searching for most tightly connected node to supernode
            w[t] = INT_MIN;
            s = t, t = max_element(w.begin(), w.end()) - w.begin(); // select s node and t node
            for (int i = 0; i < n; i++) w[i] += mat[t][i]; // update the weights of the supernode
        }

        best = min(best, {w[t] - mat[t][t], co[t]}); // compare "cut of the phase" to global best cut
        co[s].insert(co[s].end(), co[t].begin(), co[t].end()); // merge the last merged node
        for (int i = 0; i < n; i++) mat[s][i] += mat[t][i]; // update the graph
        for (int i = 0; i < n; i++) mat[i][s] = mat[s][i]; // update other side of the graph
        mat[0][t] = INT_MIN;
    }

    return best;
}

vector<string> split(const string& s, const string& sep) {
    vector<string> result;
    size_t prev = 0, next = s.find(sep);
    while (next != string::npos) {
        if (!prev) result.emplace_back(s.begin()+prev, s.begin()+next);
        else result.emplace_back(s.begin()+prev+sep.size(), s.begin()+next);
        prev = next;
        next = s.find(sep, next+1);
    }
    if (!prev) result.emplace_back(s.begin()+prev, s.end());
    else result.emplace_back(s.begin()+prev+sep.size(), s.end());
    return result;
}

int main() {
    // find vertices
    ifstream vertex_input("input.txt");
    if (!vertex_input) exit(1);
    unordered_map<string,int> vertices;
    int next = 0;
    string line;
    while (getline(vertex_input, line)) {
        size_t div = line.find(':');
        string v1 = string(line.begin(), line.begin()+div);
        if (vertices.find(v1) == vertices.end()) {
            vertices[v1] = next;
            ++next;
        }
        vector<string> v2s = split(string(line.begin()+div+2, line.end()), " ");
        for (const string& v2: v2s) {
            if (vertices.find(v2) == vertices.end()) {
                vertices[v2] = next;
                ++next;
            }
        }
    }
    vertex_input.close();

    // find edges
    ifstream edge_input("input.txt");
    if (!edge_input) exit(1);
    vector<vector<int>> mat(vertices.size(), vector(vertices.size(), 0));
    while (getline(edge_input, line)) {
        size_t div = line.find(':');
        string v1 = string(line.begin(), line.begin()+div);
        vector<string> v2s = split(string(line.begin()+div+2, line.end()), " ");
        for (const string& v2: v2s) {
            mat[vertices[v1]][vertices[v2]] = 1;
            mat[vertices[v2]][vertices[v1]] = 1;
        }
    }
    edge_input.close();
    
    pair<int, vector<int>> min_cut = stoer_wagner_min_cut(mat);
    cout << min_cut.second.size() * (vertices.size() - min_cut.second.size()) << endl;
}

// one of the lazier solutions is to use the Python "networkx" package, which
// was the original solution used to solve the question as inspired by 
// @hyper-neutrino; more rersearch into the subject revealed that the problem is
// a global minimum-cut question for a undirected unweighted graph, which is 
// most efficient with the Karger's probabilistic algorithm, which was 
// implemented and run with poor performances; a decision was made to use the 
// Stoer-Wagner algorithm instead, which produced the correct result with 
// acceptable performance

// TODO:

#include <iostream>
#include <fstream> 
#include <string>
#include <unordered_map>
#include <unordered_set>
using namespace std;

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

void find_bridge(unordered_map<string,vector<string>>& graph, const string& cur, const string& par, int& time, \
unordered_map<string,bool>& visited, unordered_map<string,int>& lowest, unordered_map<string,int>& discovery,
pair<string,string>& bridge) {
    // makr current node as visited
    visited[cur] = true;

    // set discovery and lowest time
    ++time;
    discovery[cur] = time;
    lowest[cur] = time;

    // go through all edges
    for (int i = 0; i < graph[cur].size(); ++i) {
        string nex = graph[cur][i];
        if (nex == par) {
            continue;
        } else if (visited[nex]) {
            lowest[cur] = min(lowest[cur], discovery[nex]);
        } else {
            find_bridge(graph, nex, cur, time, visited, lowest, discovery, bridge);
            lowest[cur] = min(lowest[cur], lowest[nex]);
            if (lowest[nex] > discovery[cur]) {
                bridge = {cur, nex};
            }
        }
    }
}

int main() {
    // create adjacency list
    ifstream input("input.txt");
    if (!input) exit(1);
    string line;
    unordered_map<string,vector<string>> graph;
    vector<string> vertexv;
    vector<pair<string,string>> edgev;
    while (getline(input, line)) {
        size_t div = line.find(':');
        string vertex = string(line.begin(), line.begin()+div);
        vector<string> otherv = split(string(line.begin()+div+2, line.end()), " ");
        for (const string& ov: otherv) {
            graph[vertex].push_back(ov);
            graph[ov].push_back(vertex);
            edgev.push_back({vertex, ov});
        }
        vertexv.push_back(vertex);
    }


    // bool found = false;
    // vector<pair<string,string>> bridges;
    // for (int i = 0; i < edgev.size()-1; ++i) {
    //     for (int j = i; j < edgev.size(); ++j) {
    //         cout << i << " " << j << endl;
    //         // remove ith edge
    //         string iu = edgev[i].first, iv = edgev[i].second;
    //         for (auto it = graph[iu].cbegin(); it != graph[iu].cend();) {
    //             if (*it == iv) {
    //                 graph[iu].erase(it++);
    //                 break;
    //             }
    //             ++it;
    //         }
    //         for (auto it = graph[iv].cbegin(); it != graph[iv].cend();) {
    //             if (*it == iu) {
    //                 graph[iv].erase(it++);
    //                 break;
    //             }
    //             ++it;
    //         }

    //         // remove jth edge
    //         string ju = edgev[j].first, jv = edgev[j].second;
    //         for (auto it = graph[ju].cbegin(); it != graph[ju].cend();) {
    //             if (*it == jv) {
    //                 graph[ju].erase(it++);
    //                 break;
    //             }
    //             ++it;
    //         }
    //         for (auto it = graph[jv].cbegin(); it != graph[jv].cend();) {
    //             if (*it == ju) {
    //                 graph[jv].erase(it++);
    //                 break;
    //             }
    //             ++it;
    //         }

            cout << "start" << endl;
            // prep algorithm
            int steps = 0;
            unordered_map<string,bool> visited;
            unordered_map<string,int> lowest, discovery;
            pair<string,string> bridge;
            for (const string& vertex: vertexv) {
                visited[vertex] = false;
                lowest[vertex] = -1;
                discovery[vertex] = -1;
            }
            // run algorithm
            for (const string& vertex: vertexv) {
                string empty_parent = "";
                if (!visited[vertex]) find_bridge(graph, vertex, empty_parent, steps, visited, lowest, discovery, bridge);
                if (bridge.first != "") break;
            }
            cout << bridge.first << " " << bridge.second << endl;
            cout << "finish" << endl;

    //         // add ith edge
    //         graph[iu].push_back(iv);
    //         graph[iv].push_back(iu);
            
    //         // add jth edge
    //         graph[ju].push_back(jv);
    //         graph[jv].push_back(ju);

    //         // check if exit 
    //         if (bridge.first != "") {
    //             bridges.push_back(bridge);
    //             bridges.push_back(edgev[i]);
    //             bridges.push_back(edgev[j]);
    //         }
    //     }
    //     if (found) break;
    // }
    // if (!found) exit(2);
    // cout << bridges[0].first << "/" << bridges[0].second << endl;
    // cout << bridges[1].first << "/" << bridges[1].second << endl;
    // cout << bridges[2].first << "/" << bridges[2].second << endl;
}

// note that this is equivalent to finding a bridge within a graph -
// https://www.geeksforgeeks.org/bridge-in-a-graph/
// this particular solution uses tarjan's algorithm

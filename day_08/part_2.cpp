#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <numeric>
using namespace std;

void parse_map(const string& filename, string& instr, unordered_map<string,vector<string>>& map, vector<string>& loc) {
    ifstream ifs("input.txt");
    if (!ifs) exit(1);
    
    getline(ifs, instr);
    
    string node, start, left, right;
    getline(ifs, node);
    while (getline(ifs, node)) {
        start = string(node.begin(), node.begin()+3);
        left = string(node.begin()+7, node.begin()+10);
        right = string(node.begin()+12, node.end()-1);
        map[start] = {left, right};
        if (*(start.cend()-1) == 'A') loc.push_back(start);
    }
}

int find_steps(string start, string& instr, unordered_map<string,vector<string>>& map) {
    int steps = 0;
    while (*(start.cend()-1) != 'Z') {
        if (instr[steps % instr.size()] == 'L') start = map[start][0];
        else start = map[start][1];
        ++steps;
    }
    return steps;
}

long long find_all_steps(vector<string>& loc, string& instr, unordered_map<string,vector<string>>& map) {
    vector<int> sv;
    for (string start: loc) {
        sv.push_back(find_steps(start, instr, map));
    }

    long long all = 1;
    for (int steps: sv) {
        all = lcm(all, steps);
    }
    return all;
}

int main() {
    string instr;
    unordered_map<string,vector<string>> map;
    vector<string> loc;
    parse_map("input.txt", instr, map, loc);

    cout << find_all_steps(loc, instr, map) << endl;
}

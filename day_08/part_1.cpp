#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

void parse_map(const string& filename, string& instr, unordered_map<string,vector<string>>& map) {
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
    }
}

int find_steps(string& instr, unordered_map<string,vector<string>>& map) {
    int steps = 0;
    string start = "AAA";
    while (start != "ZZZ") {
        if (instr[steps % instr.size()] == 'L') start = map[start][0];
        else start = map[start][1];
        ++steps;
    }
    return steps;
}

int main() {
    string instr;
    unordered_map<string,vector<string>> map;
    parse_map("input.txt", instr, map);

    cout << find_steps(instr, map) << endl;
}

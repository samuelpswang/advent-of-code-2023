#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
using namespace std;

int my_hash(const string& s) {
    int curr = 0;
    for (char c: s) {
        curr += c;
        curr *= 17;
        curr %= 256;
    }
    return curr;
}

int main() {
    ifstream input("input.txt");
    if (!input) exit(1);

    string line;
    vector<vector<pair<string,int>>> data(256);
    while (getline(input, line, ',')) {
        if (line.find('=') != string::npos) {
            string label(line.begin(), line.begin()+line.find('='));
            int loc = my_hash(label), id = stoi(string(line.begin()+line.find('=')+1, line.end()));
            bool found = false;
            for (pair<string,int>& p: data[loc]) {
                if (p.first == label) {
                    p.second = id;
                    found = true;
                }
            }
            if (!found) data[loc].push_back({label, id});
        } else {
            string label(line.begin(), line.begin()+line.find('-'));
            int loc = my_hash(label);
            vector<pair<string,int>> nv;
            for (const pair<string,int>& p: data[loc]) {
                if (p.first == label) continue;
                else nv.push_back(p);
            }
            data[loc] = nv;
        }
    }

    int total = 0;
    for (int i = 0; i < data.size(); ++i) {
        for (int j = 0; j < data[i].size(); ++j) {
            total += (i+1) * (j+1) * (data[i][j].second);
        }
    }
    cout << total << endl;
}
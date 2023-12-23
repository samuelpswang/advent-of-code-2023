#include <iostream>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <queue>
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

struct cube {
    vector<int> data;
    cube() {}
    cube(const string& line) {
        vector<string> cv = split(line, "~");
        vector<string> vv1 = split(cv[0], ",");
        vector<string> vv2 = split(cv[1], ",");
        data.push_back(stoi(vv1[0]));
        data.push_back(stoi(vv1[1]));
        data.push_back(stoi(vv1[2]));
        data.push_back(stoi(vv2[0]));
        data.push_back(stoi(vv2[1]));
        data.push_back(stoi(vv2[2]));
        assert(data[0] <= data[3]);
        assert(data[1] <= data[4]);
        assert(data[2] <= data[5]);
    }
    int operator[](int n) const {
        assert(0 <= n && n <= 5);
        return data[n];
    }
    int& operator[](int n) {
        assert(0 <= n && n <= 5);
        return data[n];
    }
    bool operator<(const cube& rhs) const {
        return this->data[2] < rhs[2];
    }
};

bool overlap(const cube& c1, const cube& c2) {
    return (max(c1[0], c2[0]) <= min(c1[3], c2[3]) && max(c1[1], c2[1]) <= min(c1[4], c2[4]));
}

int main() {
    ifstream input("input.txt");
    if (!input) exit(1);

    string line;
    vector<cube> voc(0);
    while (getline(input, line)) {
        cube curr(line);
        voc.push_back(line);
    }
    input.close();
    sort(voc.begin(), voc.end());

    for (int i = 0; i < voc.size(); ++i) {
        int max_below = 1;
        for (int j = 0; j < i; ++j) {
            if (overlap(voc[i], voc[j]))
                max_below = max(max_below, voc[j][5]+1);
        }
        voc[i][5] -= voc[i][2] - max_below;
        voc[i][2] = max_below;
    }
    sort(voc.begin(), voc.end());
    
    unordered_map<int,unordered_set<int>> key_supports_value, key_is_supported_by_value;
    for (int i = 0; i < voc.size(); ++i) {
        for (int j = 0; j < i; ++j) {
            if (overlap(voc[i], voc[j]) && voc[i][2] == voc[j][5]+1) {
                key_supports_value[j].insert(i);
                key_is_supported_by_value[i].insert(j);
            }
        }
    }

    int total = 0;
    for (int i = 0; i < voc.size(); ++i) {
        unordered_set<int> fall;
        queue<int> q;
        q.push(i);
        fall.insert(i);
        while (!q.empty()) {
            int ci = q.front();
            q.pop();
            for (auto it: key_supports_value[ci]) {
                bool is_not_supported = true;
                for (auto it2: key_is_supported_by_value[it]) {
                    if (fall.count(it2) == 0) is_not_supported = false;
                }
                if (is_not_supported && fall.find(it) == fall.end()) {
                    q.push(it);
                    fall.insert(it);
                }
            }
        }
        total += fall.size()-1;
    }
    cout << total << endl;
}

#include <iostream>
#include <fstream>
#include <vector>
#include <string> 
using namespace std;

bool empty(const string& s) {
    for (char c: s) {
        if (c != '.') return false;
    }
    return true;
}

void transpose(vector<string>& data) {
    vector<string> new_data;
    for (int c = 0; c < data[0].size(); ++c) {
        string col;
        for (int r = 0; r < data.size(); ++r) {
            col += data[r][c];
        }
        new_data.push_back(col);
    }
    data = new_data;
}

void row_expand(vector<string>& data) {
    vector<string> new_data;
    for (const string& s: data) {
        if (empty(s)) new_data.push_back(s);
        new_data.push_back(s);
    }
    data = new_data;
}

void col_expand(vector<string>& data) {
    transpose(data);
    vector<string> new_data;
    for (const string& s: data) {
        if (empty(s)) new_data.push_back(s);
        new_data.push_back(s);
    }
    data = new_data;
    transpose(data);
}

vector<pair<int,int>> find_pos(const vector<string>& data) {
    vector<pair<int,int>> pos;
    for (int r = 0; r < data.size(); ++r) {
        for (int c = 0; c < data[0].size(); ++c) {
            if (data[r][c] == '#') {
                pair<int,int> p = {r, c};
                pos.push_back(p);
            }
        }
    }
    return pos;
}

int d(const pair<int,int>& p1, const pair<int,int>& p2) {
    return (p1.first > p2.first ? p1.first-p2.first : p2.first-p1.first) + \
           (p1.second > p2.second ? p1.second-p2.second : p2.second-p1.second);
}

int find_dist_sum(const vector<pair<int,int>>& pos) {
    int sum = 0;
    for (int first = 0; first < pos.size()-1; ++first) {
        for (int second = first+1; second < pos.size(); ++second) {
            sum += d(pos.at(first), pos.at(second));
        }
    }
    return sum;
}

int main() {
    ifstream input("input.txt");
    if (!input) exit(1);
    
    string line;
    vector<string> data;
    while (getline(input, line)) {
        data.push_back(line);
    }
    input.close();
    
    row_expand(data);
    col_expand(data);

    vector<pair<int,int>> loc = find_pos(data);
    cout << find_dist_sum(loc) << endl;
}
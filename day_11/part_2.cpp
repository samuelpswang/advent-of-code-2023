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

vector<int> find_empty_row(vector<string>& data) {
    vector<int> row_loc;
    int row = 0;
    for (const string& s: data) {
        if (empty(s)) row_loc.push_back(row);
        ++row;
    }
    return row_loc;
}

vector<int> find_empty_col(vector<string>& data) {
    transpose(data);
    vector<int> col_loc;
    int col = 0;
    for (const string& s: data) {
        if (empty(s)) col_loc.push_back(col);
        ++col;
    }
    transpose(data);
    return col_loc;
}

int count_empty(int start, int end, const vector<int>& data) {
    int count = 0;
    for (int i = 0; i < data.size(); ++i) {
        if (start < data[i] && data[i] < end) ++count;
    }
    return count;
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

long d(const pair<int,int>& p1, const pair<int,int>& p2, \
       const vector<int>& row_data, const vector<int>& col_data, long expand) {
    int empty_row_count = 0, empty_col_count = 0, row_dis = 0, col_dis = 0;

    // process row
    if (p1.first > p2.first) {
        empty_row_count = count_empty(p2.first, p1.first, row_data);
        row_dis = p1.first - p2.first + empty_row_count * (expand-1);
    } else {
        empty_row_count = count_empty(p1.first, p2.first, row_data);
        row_dis = p2.first - p1.first + empty_row_count * (expand-1);
    }

    // process col
    if (p1.second > p2.second) {
        empty_col_count = count_empty(p2.second, p1.second, col_data);
        col_dis = p1.second - p2.second + empty_col_count * (expand-1);
    } else {
        empty_col_count = count_empty(p1.second, p2.second, col_data);
        col_dis = p2.second - p1.second + empty_col_count * (expand-1);
    }

    return row_dis + col_dis ;
}

long long find_dist_sum(const vector<pair<int,int>>& pos, \
                        const vector<int>& row_data, \
                        const vector<int>& col_data, long expand) {
    long long sum = 0;
    for (int first = 0; first < pos.size()-1; ++first) {
        for (int second = first+1; second < pos.size(); ++second) {
            sum += d(pos.at(first), pos.at(second), row_data, col_data, expand);
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

    vector<int> row_data = find_empty_row(data);
    vector<int> col_data = find_empty_col(data);
    
    vector<pair<int,int>> loc = find_pos(data);
    cout << find_dist_sum(loc, row_data, col_data, 1000000) << endl;
}
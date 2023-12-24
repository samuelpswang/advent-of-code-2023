#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
using namespace std;

const double LOWER = 200000000000000;
const double UPPER = 400000000000000;

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

pair<bool, pair<double,double>> solve(double x1, double y1, double dx1, double dy1, double x2, double y2, double dx2, double dy2) {
    // x1 + k * dx1 = x2 + m * dx2 -> dx1 * k - dx2 * m = x2 - x1
    // y1 + k * dy1 = y2 + m * dy2 -> dy1 * k - dy2 * m = y2 - y1
    // | dx1, -dx2 | | k |   | x2-x1 |
    // | dy1, -dy2 | | m | = | y2-y1 | 

    // make sure it has a solution
    if (dx1 == dx2 && dy1 == dy2) return {false, {0, 0}};

    // find the inverse of the matrix
    double det = (dx1 * (-dy2) - (-dx2 * dy1));
    double r1c1 = (-dy2)/det;
    double r1c2 = -(-dx2)/det;
    double r2c1 = -(dy1)/det;
    double r2c2 = (dx1)/det;
    
    // multiply to other side to find k and m
    double k = (r1c1 * (x2-x1)) + (r1c2 * (y2-y1));
    double m = (r2c1 * (x2-x1)) + (r2c2 * (y2-y1));
    double x = x1 + k * dx1;
    double y = y1 + k * dy1;

    return {(k > 0 && m > 0), {x, y}};
}

int main() {
    ifstream input("input.txt");
    if (!input) exit(1);
    string line;
    vector<vector<double>> data;
    while (getline(input, line)) {
        vector<double> stone_data;
        vector<string> sp1 = split(line, " @ ");
        vector<string> sp2 = split(sp1[1], ", ");
        sp1 = split(sp1[0], ", ");
        for (const string& s: sp1) {
            stone_data.push_back(stod(s));
        }
        for (const string& s: sp2) {
            stone_data.push_back(stod(s));
        }
        data.push_back(stone_data);
    }
    input.close();

    int count = 0;
    for (int i = 0; i < data.size()-1; ++i) {
        for (int j = i; j < data.size(); ++j) {
            pair<bool, pair<double,double>> cross = solve(data[i][0], data[i][1], data[i][3], data[i][4], data[j][0], data[j][1], data[j][3], data[j][4]);
            if (LOWER <= cross.second.first && cross.second.first <= UPPER && LOWER <= cross.second.second && cross.second.second <= UPPER && cross.first) ++count;
        }
    }
    cout << count << endl;
}
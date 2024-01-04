// File Name:       day_14/part_2.cpp
// Author:          Samuel Wang (@samuelpswang)
// Purpose:         AoC 2023 Day 14 Part 2 (https://adventofcode.com/2023/day/14)
// Last Updated:    04 Jan 2023

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

class Box {
private:
    vector<string> data;
    bool is_row;

    void transpose() {
        vector<string> new_data;
        for (int c = 0; c < data[0].size(); ++c) {
            string line;
            for (int r = 0; r < data.size(); ++r) {
                line += data[r][c];
            }
            new_data.push_back(line);
        }
        data = new_data;
        is_row = !is_row;
    }

    void tilt_north() {
        if (is_row) transpose();
        for (int c = 0; c < data.size(); ++c) {
            string new_line;
            int o_count = 0, dot_count = 0;
            for (int r = 0; r < data[0].size(); ++r) {
                switch (data[c][r]) {
                    case '.':
                        ++dot_count;
                        break;
                    case 'O':
                        ++o_count;
                        break;
                    case '#':
                        for (; o_count > 0; --o_count) {
                            new_line += 'O';
                        }
                        for (; dot_count > 0; --dot_count) {
                            new_line += '.';
                        }
                        new_line += '#';
                        break;
                }
            }
            for (; o_count > 0; --o_count) {
                new_line += 'O';
            }
            for (; dot_count > 0; --dot_count) {
                new_line += '.';
            }
            data[c] = new_line;
        }
        if (!is_row) transpose();
    }

    void tilt_south() {
        if (is_row) transpose();
        for (int c = 0; c < data.size(); ++c) {
            string new_line;
            int o_count = 0, dot_count = 0;
            for (int r = data[0].size()-1; r >= 0; --r) {
                switch (data[c][r]) {
                    case '.':
                        ++dot_count;
                        break;
                    case 'O':
                        ++o_count;
                        break;
                    case '#':
                        for (; o_count > 0; --o_count) {
                            new_line += 'O';
                        }
                        for (; dot_count > 0; --dot_count) {
                            new_line += '.';
                        }
                        new_line += '#';
                        break;
                }
            }
            for (; o_count > 0; --o_count) {
                new_line += 'O';
            }
            for (; dot_count > 0; --dot_count) {
                new_line += '.';
            }
            data[c] = string(new_line.rbegin(), new_line.rend());
        }
        if (!is_row) transpose();
    }

    void tilt_east() {
        if (!is_row) transpose();
        for (int r = 0; r < data.size(); ++r) {
            string new_line;
            int o_count = 0, dot_count = 0;
            for (int c = data[0].size()-1; c >= 0; --c) {
                switch (data[r][c]) {
                    case '.':
                        ++dot_count;
                        break;
                    case 'O':
                        ++o_count;
                        break;
                    case '#':
                        for (; o_count > 0; --o_count) {
                            new_line += 'O';
                        }
                        for (; dot_count > 0; --dot_count) {
                            new_line += '.';
                        }
                        new_line += '#';
                        break;
                }
            }
            for (; o_count > 0; --o_count) {
                new_line += 'O';
            }
            for (; dot_count > 0; --dot_count) {
                new_line += '.';
            }
            data[r] = string(new_line.rbegin(), new_line.rend());
        }
        if (!is_row) transpose();
    }

    void tilt_west() {
        if (!is_row) transpose();
        for (int r = 0; r < data.size(); ++r) {
            string new_line;
            int o_count = 0, dot_count = 0;
            for (int c = 0; c < data[0].size(); ++c) {
                switch (data[r][c]) {
                    case '.':
                        ++dot_count;
                        break;
                    case 'O':
                        ++o_count;
                        break;
                    case '#':
                        for (; o_count > 0; --o_count) {
                            new_line += 'O';
                        }
                        for (; dot_count > 0; --dot_count) {
                            new_line += '.';
                        }
                        new_line += '#';
                        break;
                }
            }
            for (; o_count > 0; --o_count) {
                new_line += 'O';
            }
            for (; dot_count > 0; --dot_count) {
                new_line += '.';
            }
            data[r] = new_line;
        }
        if (!is_row) transpose();
    }

public:
    Box(const vector<string>& data): data(data), is_row(true) {}

    void rotate() {
        tilt_north();
        tilt_west();
        tilt_south();
        tilt_east();
    }

    int load() {
        if (is_row) transpose();
        int sum = 0;        
        for (const string& col: data) {
            int row = col.size(), col_sum = 0;
            for (char c: col) {
                if (c == 'O') col_sum += row;
                --row;
            }
            sum += col_sum;
        }
        if (!is_row) transpose();
        return sum;
    }

    void print() {
        if (!is_row) transpose();
        for (const string& r: data) {
            cout << r << endl;
        }
    }
};

int main() {
    ifstream input("input.txt");
    if (!input) exit(1);

    vector<string> data;
    string line;
    while (getline(input, line)) {
        data.push_back(line);
    }
    input.close();

    // initial investigation
    Box dut(data);
    vector<int> loadv;
    int initial = 1000;
    for (long long it = 0; it < initial; it++) {
        dut.rotate();
        loadv.push_back(dut.load());
    }

    // pattern finding
    int last_seq = 0, seq = 20;
    vector<int> last_twenty = vector(loadv.end()-seq, loadv.end());
    for (int i = loadv.size()-seq-1; i > 0; --i) {
        bool matched = true;
        for (int j = 0; j < last_twenty.size(); ++j) {
            if (loadv[i+j] != last_twenty[j]) {
                matched = false; 
                break;
            }
        }
        if (matched) {
            last_seq = i;
            break;
        }
    }

    // identify index
    int cycle = loadv.size()-seq - last_seq;
    long long start = 1000000000;
    while (start > initial-1) { start -= cycle; }
    cout << loadv[start-1] << endl;
}

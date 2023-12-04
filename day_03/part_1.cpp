#include <iostream>
#include <fstream> 
#include <vector>
#include <string>
using namespace std;

void parse_schematics(const string& filename, vector<vector<int>>& symbol, vector<vector<vector<int>>>& number) {
    ifstream ifs;
    ifs.open(filename);
    string line, stack = "";
    int num_col_start = -1;
    while (getline(ifs, line)) {
        vector<int> sym_row_pos;
        vector<vector<int>> num_row_pos;
        for (int i = 0; i < line.size(); i++) {
            char c = line[i];
            if ('0' <= c && c <= '9') {
                if (stack.empty()) num_col_start = i;
                stack += c;
            } else if (c == '.') {
                if (!stack.empty()) {
                    num_row_pos.push_back({num_col_start, i, stoi(stack)});
                    stack = "";
                }
            } else {
                if (!stack.empty()) {
                    num_row_pos.push_back({num_col_start, i, stoi(stack)});
                    stack = "";
                }
                sym_row_pos.push_back(i);
            }
        }
        if (!stack.empty()) num_row_pos.push_back({num_col_start, int(line.size())+1, stoi(stack)});
        stack = "";
        num_col_start = -1;
        symbol.push_back(sym_row_pos);
        number.push_back(num_row_pos);
    }
}

int find_sum(vector<vector<int>>& symbol, vector<vector<vector<int>>>& num_pos) {
    int sum = 0;
    for (int r = 0; r < symbol.size(); r++) {
        for (int c: symbol[r]) {
            for (int dr = -1; dr < 2; dr++) {
                for (int dc = -1; dc < 2; dc++) {
                    int pr = r+dr, pc = c+dc;
                    for (int i = 0; i < num_pos[pr].size(); i++) {
                        if (num_pos[pr][i][2] && num_pos[pr][i][0] <= pc && pc < num_pos[pr][i][1]) {
                            sum += num_pos[pr][i][2];
                            num_pos[pr][i][2] = 0;
                        }
                    }
                }
            }
        }
    }
    return sum;
}

int main() {
    vector<vector<int>> sym_pos;
    vector<vector<vector<int>>> num_pos;
    parse_schematics("input.txt", sym_pos, num_pos);
    cout << find_sum(sym_pos, num_pos) << endl;
}


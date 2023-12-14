#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

int load(const string& col) {
    int row = col.size(), prev = col.size(), total = 0;
    for (char c: col) {
        switch (c) {
            case 'O':
                total += prev;
                --prev;
                break;
            case '#':
                prev = row-1;
                break;
        }
        --row;
    }
    return total;
}

int main() {
    ifstream input("input.txt");
    if (!input) exit(1);

    vector<string> row;
    string line;
    while (getline(input, line)) {
        row.push_back(line);
    }
    input.close();
    
    vector<string> col;
    for (int c = 0; c < row[0].size(); ++c) {
        string col_item;
        for (int r = 0; r < row.size(); ++r) {
            col_item += row[r][c];
        }
        col.push_back(col_item);
        col_item = "";
    }
    
    int total = 0;
    for (int c = 0; c < col.size(); ++c) {
        total += load(col[c]);
    }
    cout << total << endl;
}
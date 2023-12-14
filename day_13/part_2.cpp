#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int diff_count(const string& left, const string& right) {
    int count = 0;
    for (int i = 0; i < left.size(); ++i) {
        if (left[i] != right[i]) ++count;
    }
    return count;
}

class RockMap {
private:
    vector<string> data;
    
public:
    RockMap(const vector<string>& data): data(data) {}

    int rdim() { return data.size(); }
    int cdim() { return data[0].size(); }

    string row(int n) {
        --n;
        if (n < 0 || n >= rdim()) return "";
        return data.at(n);
    }
    
    string col(int n) {
        --n;
        if (n < 0 || n >= cdim()) return "";
        string c;
        for (int r = 0; r < rdim(); ++r) {
            c = c + data[r][n];
        }
        return c;
    }

    bool row_reflect(int n) {
        int top = n, bottom = n+1, count = 0, incre = (top < rdim()-bottom+1 ? top : rdim()-bottom+1);
        if (!incre) return false;
        for (int i = 0; i < incre; ++i) {
            count += diff_count(row(top-i), row(bottom+i));
        }
        return count == 1;
    }

    bool col_reflect(int n) {
        int left = n, right = n+1, count = 0, incre = (left < cdim()-right+1 ? left : cdim()-right+1);
        if (!incre) return false;
        for (int i = 0; i < incre; ++i) {
            count += diff_count(col(left-i), col(right+i));
        }
        return (count == 1);
    }

    int close_magic_num() {
        int row = 0, col = 0;
        for (int i = 1; i <= rdim(); ++i) {
            if (row_reflect(i)) {
                row = i; 
                break;
            }
        }
        for (int i = 1; i <= cdim(); ++i) {
            if (col_reflect(i)) {
                col = i; 
                break;
            }
        }
        return col + 100 * row;
    }
};

int main() {
    vector<string> temp;
    vector<RockMap> vrm;

    ifstream input("input.txt");
    if (!input) exit(1);
    
    string line;
    while (getline(input, line)) {
        if (line != "") {
            temp.push_back(line);
        } else {
            vrm.emplace_back(temp);
            temp = {};
        }
    }
    if (temp.size()) vrm.emplace_back(temp);

    long sum = 0;
    for (int i = 0; i < vrm.size(); ++i) {
        sum += vrm[i].close_magic_num();
    }
    cout << sum << endl;
}
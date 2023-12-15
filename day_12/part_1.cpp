#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
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

bool seperated(const string& s) {
    if (s.find("aa") != string::npos) return false;
    return true;
}

string replace(const string& s, const vector<int>& data) {
    string new_string;
    int count = 0;
    for (char c: s) {
        if (c == 'a') {
            for (int i = 0; i < data[count]; ++i) {
                new_string += '#';
            }
            ++count;
        } else {
            new_string += '.';
        }
    }
    return new_string;
}

bool match(const string& pattern, const string& input) {
    if (pattern.size() != input.size()) return false;
    for (int i = 0; i < pattern.size(); ++i) {
        if (pattern[i] == '?') continue;
        else if (pattern.at(i) != input.at(i)) return false;
    }
    return true;
}

// very silly way to to this - 
// see part 2 for dynamic programming methods
int arrangements(const string& pattern, const vector<int>& data) {
    int total_len = pattern.size();
    int data_len = 0;
    for (int n: data) { data_len += n; }
    int dot_len = total_len-data_len;
    data_len = data.size();

    string initial;
    for (int i = 0; i < data_len; ++i) { initial += 'a'; }
    for (int i = 0; i < dot_len; ++i) { initial += '.'; }

    int count = 0;
    do {
        if (seperated(initial) && match(pattern, replace(initial, data))) count++;
    } while (prev_permutation(initial.begin(), initial.end()));

    return count;
}

int main() {
    ifstream input("input.txt");
    if (!input) exit(1);
    
    string pattern, data;
    vector<string> vpat;
    vector<vector<int>> vdata;
    while (getline(input, pattern, ' ')) {
        getline(input, data);
        vector<string> str_data = split(data, ",");
        vector<int> num_data;
        for (const string& s: str_data) {
            num_data.push_back(stoi(s));
        }
        vpat.push_back(pattern);
        vdata.push_back(num_data);
    }
    input.close();

    long sum = 0;
    for (int i = 0; i < vpat.size(); i++) {
        sum += arrangements(vpat[i], vdata[i]);
    }
    
    cout << sum << endl;
}
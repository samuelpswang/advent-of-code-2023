#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

const unordered_map<string,int> IND = {{"red", 0}, {"green", 1}, \
    {"blue", 2}};

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

void min_cube_draw(string& draw, vector<int>& minv) {
    vector<string> colors = split(draw, ", ");
    for (string& c: colors) {
        vector<string> v = split(c, " ");
        int count = stoi(v[0]);
        if (count > minv[IND.at(v[1])]) minv[IND.at(v[1])] = count;
    }
}

long long int power_of_game(string& game) {
    // find min cubes
    vector<string> draws = split(game, "; ");
    vector<int> minv(3, 0);
    for (string& d: draws) {
        min_cube_draw(d, minv);
    }
    
    // calculate power
    long long int power = 1;
    for (int n: minv) {
        power *= n;
    }
    return power;
}

int main() {
    // open file
    ifstream ifs;
    ifs.open("input.txt");
    if (!ifs) exit(1);
    
    // parse
    string line;
    long long int sum = 0;
    while (getline(ifs, line)) {
        line = string(line.begin()+line.find(':')+2, line.end());
        sum += power_of_game(line);
    }
    ifs.close();
    
    // output answer
    cout << sum << endl;
}

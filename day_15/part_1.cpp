#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
using namespace std;

int my_hash(const string& s) {
    int curr = 0;
    for (char c: s) {
        curr += c;
        curr *= 17;
        curr %= 256;
    }
    return curr;
}

int main() {
    ifstream input("input.txt");
    if (!input) exit(1);

    string line;
    vector<string> seq;
    while (getline(input, line, ',')) {
        seq.push_back(line);
    }

    int sum = 0;
    for (const string& s: seq) {
        sum += my_hash(s);
    }

    cout << sum << endl;
}
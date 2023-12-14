#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
using namespace std;

void parse_card(const string& filename, vector<unordered_set<int>>& win, vector<vector<int>>& you) {
    ifstream ifs;
    ifs.open(filename);
    if (!ifs) exit(1);

    string card, colon, sep, nextline;
    int gameno;
    while (ifs >> card >> gameno >> colon) {
        unordered_set<int> w;
        vector<int> y;
        int num;
        for (int i = 0; i < 10; i++) {
            ifs >> num;
            w.insert(num);
        }
        ifs >> sep;
        for (int i = 0; i < 25; i++) {
            ifs >> num;
            y.push_back(num);
        }
        win.push_back(w);
        you.push_back(y);
    }

    ifs.close();
}

void count_matches(vector<unordered_set<int>>& win, vector<vector<int>>& you, vector<vector<int>>& matches) {
    for (int cid = 0; cid < win.size(); cid++) {
        int count = 0;
        for (int nid = 0; nid < you[cid].size(); nid++) {
            if (win[cid].count(you[cid][nid])) count++;
        }
        matches.push_back({1, count});
    }
}

int process_matches(vector<vector<int>>& matches) {
    for (int cid = 0; cid < matches.size(); cid++) {
        int copies = matches[cid][0];
        for (int next = 1; next <= matches[cid][1]; next++) {
            matches[cid+next][0] += copies;
        }
    }

    int total = 0;
    for (int cid = 0; cid < matches.size(); cid++) {
        total += matches[cid][0];
    }
    return total;
}


int main() {
    vector<unordered_set<int>> win;
    vector<vector<int>> you;
    parse_card("input.txt", win, you);

    vector<vector<int>> matches;
    count_matches(win, you, matches);

    cout << process_matches(matches) << endl;
}

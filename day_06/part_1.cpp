#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct race {
    int total;
    int max;

    race(int time, int dist): total(time), max(dist) {}

    bool is_valid(int hold) {
        int dis = (total-hold) * hold;
        if (total > hold && dis > max) return true;
        else return false;
    }
};

void parse_record(const string& filename, vector<race>& record) {
    ifstream ifs;
    ifs.open(filename);
    if (!ifs) exit(1);

    // better way to do this - 
    // string manipulation    
    string dummy;
    int num;
    vector<int> time, dist;
    ifs >> dummy;
    for (int i = 0; i < 4; i++) {
        ifs >> num;
        time.push_back(num);
    }
    ifs >> dummy;
    for (int i = 0; i < 4; i++) {
        ifs >> num;
        dist.push_back(num);
    }

    for (size_t i = 0; i < time.size(); i++) {
        record.emplace_back(time[i], dist[i]);
    }
}

// better way to do this - 
// use binary search
void find_options(vector<race>& record, vector<int>& options) {
    for (race r: record) {
        int total = r.total, start, end;
        for (int t = 0; t < total; t++) {
            if (r.is_valid(t)) {
                start = t;
                break;
            }
        }
        for (int t = total; t >= 0; t--) {
            if (r.is_valid(t)) {
                end = t;
                break;
            }
        }
        options.push_back(end-start+1);
    }
}

long multiply(vector<int>& v) {
    long res = 1;
    for (int n: v) {
        res *= n;
    }
    return res;
}

int main() {
    vector<race> record;
    parse_record("input.txt", record);

    vector<int> options;
    find_options(record, options);

    cout << multiply(options) << endl;
}
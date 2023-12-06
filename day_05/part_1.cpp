#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

struct record {
    long source;
    long target;
    long range;

    record(long s, long t, long r): source(s), target(t), range(r) {}

    long operator[](long index) {
        if (source <= index && index < source+range) return target + (index - source);
        else return -1;
    }
};

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

void parse_almanac(const string& filename, vector<long>& seeds, vector<vector<record>>& maps) {
    ifstream ifs;
    ifs.open(filename);
    if (!ifs) exit(1);
    string line;
    while (getline(ifs, line)) {
        if (line.find("seeds:") != string::npos) {
            vector<string> seeds_str_v = split(string(line.begin()+7, line.end()), " ");
            for (const string& seed_str: seeds_str_v) {
                seeds.push_back(stol(seed_str));
            }
        } else if (line != "") {
            string record_str;
            vector<string> record_str_v;
            vector<record> record_v;
            while (getline(ifs, record_str) && record_str != "") {
                record_str_v = split(record_str, " ");
                record_v.emplace_back(stol(record_str_v[1]), stol(record_str_v[0]), stol(record_str_v[2]));
            }
            maps.push_back(record_v);
        }
    }
    ifs.close();
}

long find_loc(long seed, vector<vector<record>>& maps) {
    vector<long> seed_v = {seed};
    vector<long> next_seed_v;
    for (vector<record>& vr: maps) {
        for (long xseed: seed_v) {
            for (record& r: vr) {
                long next_seed = r[xseed];
                if (next_seed >= 0) next_seed_v.push_back(next_seed);
            }
        }
        if (!next_seed_v.empty()) {
            seed_v = next_seed_v;
            next_seed_v = {};
        }
    }

    long mseed = LONG_MAX;
    for (long xseed: seed_v) {
        mseed = min(mseed, xseed);
    }

    return mseed;
}

int main() {
    vector<long> seeds;
    vector<vector<record>> maps;
    parse_almanac("input.txt", seeds, maps);

    long min_loc = LONG_MAX;
    for (long seed: seeds) {
        min_loc = min(min_loc, find_loc(seed, maps));
    }

    cout << min_loc << endl;
}
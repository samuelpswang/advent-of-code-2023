// File Name:       day_05/part_2.cpp
// Author:          Samuel Wang (@samuelpswang)
// Purpose:         AoC 2023 Day 5 Part 2 (https://adventofcode.com/2023/day/5)
// Last Updated:    04 Jan 2023

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
    
    long get_one(long index) {
        if (source <= index && index < source+range) return target + (index - source);
        else return -1;
    }

    pair<vector<vector<long>>, vector<vector<long>>> get_range(vector<long>& seed_range) {
        // set up
        long seed_start = seed_range[0], seed_end = seed_range[1];
        long source_start = source, source_end = source_start+range-1;
        long shift = target-source;
        // split -
        // there has to be a better way to handle ranges
        // carelessly setting inequality results in huge errors
        if (seed_start <= source_start && source_start <= seed_end && seed_end <= source_end) {
            if (seed_start != source_start)
                return {{{source_start+shift, seed_end+shift}}, \
                        {{seed_start, source_start}}};
            else
                return {{{source_start+shift, seed_end+shift}}, \
                        {}};
        } else if (source_start <= seed_start && seed_start <= source_end && source_end <= seed_end) {
            return {{{seed_start+shift, source_end+shift}}, \
                    {{source_end, seed_end}}};
        } else if (source_start <= seed_start && seed_start <= seed_end && seed_end <= source_end) {
            return {{{seed_start+shift, seed_end+shift}}, \
                    {}};
        } else if (seed_start <= source_start && source_start <= source_end && source_end <= seed_end) {
            if (seed_start != source_start)
                return {{{source_start+shift, source_end+shift}}, \
                        {{seed_start, source_start}, {source_end, seed_end}}};
            else
                return {{{source_start+shift, source_end+shift}}, \
                        {{source_end, seed_end}}};
        } else {
            return {{}, \
                    {{seed_start, seed_end}}};
        }
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

void merge_range(vector<vector<long>>& lhs, const vector<vector<long>>& rhs) {
    for (const vector<long>& item: rhs) {
        lhs.push_back(item);
    }
}

void parse_almanac(const string& filename, vector<vector<long>>& seeds, vector<vector<record>>& maps) {
    ifstream ifs;
    ifs.open(filename);
    if (!ifs) exit(1);
    string line;
    while (getline(ifs, line)) {
        if (line.find("seeds:") != string::npos) {
            vector<string> seeds_str_v = split(string(line.begin()+7, line.end()), " ");
            for (size_t i = 0; i < seeds_str_v.size()/2; i++) {
                seeds.push_back({stol(seeds_str_v[2*i]), stol(seeds_str_v[2*i])+stol(seeds_str_v[2*i+1])});
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

long find_min_loc_via_brute_force(vector<long>& seed, vector<vector<record>>& maps) {
    long min_loc = LONG_MAX;
    for (long s = seed[0]; s < seed[1]; s++) {
        long loc = s;
        for (vector<record>& level: maps) {
            for (record& filter: level) {
                long result = filter.get_one(loc);
                if (result >= 0) loc = result;
            }
        }
        min_loc = min(min_loc, loc);
    }
    return min_loc;
}

long find_min_loc_via_ranges(vector<long>& seed, vector<vector<record>>& maps) {
    // this is key to solving second challenge -
    // for eacth level, feed only the ranges that have not been shifted back in
    vector<vector<long>> seed_v = {seed};
    int level = 0;
    for (vector<record>& record_v: maps) {
        
        vector<vector<long>> shifted_col;
        for (record r: record_v) {
            vector<vector<long>> not_shifted_col;
            for (vector<long>& seed_range: seed_v) {
                pair<vector<vector<long>>, vector<vector<long>>> p = r.get_range(seed_range);
                merge_range(shifted_col, p.first);
                merge_range(not_shifted_col, p.second);
            }
            seed_v = not_shifted_col;
        }
        merge_range(seed_v, shifted_col);
    }

    long min_loc = LONG_MAX;
    for (vector<long>& range: seed_v) {
        cout << range[0] << " " << range[1] << endl;
        min_loc = min(min_loc, range[0]);
    }

    return min_loc;
}

int main() {
    vector<vector<long>> seeds;
    vector<vector<record>> maps;
    parse_almanac("input.txt", seeds, maps);

    long min_loc = LONG_MAX;
    for (vector<long>& seed_range: seeds) {
        long loc = find_min_loc_via_ranges(seed_range, maps);
        min_loc = min(loc, min_loc);
    }
    
    cout << min_loc << endl;
}
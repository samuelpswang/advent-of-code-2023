// File Name:       day_06/part_2.cpp
// Author:          Samuel Wang (@samuelpswang)
// Purpose:         AoC 2023 Day 6 Part 2 (https://adventofcode.com/2023/day/6)
// Last Updated:    04 Jan 2023

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct race {
    long total;
    long max;

    race(long time, long dist): total(time), max(dist) {}

    bool is_valid(long hold) {
        long dis = (total-hold) * hold;
        if (total > hold && dis > max) return true;
        else return false;
    }
};

void parse_record(const string& filename, race& record) {
    ifstream ifs;
    ifs.open(filename);
    if (!ifs) exit(1);

    // better way to do this - 
    // string manipulation    
    string dummy;
    int num;
    string time, dist;
    ifs >> dummy;
    for (int i = 0; i < 4; i++) {
        ifs >> num;
        time += to_string(num);
    }
    ifs >> dummy;
    for (int i = 0; i < 4; i++) {
        ifs >> num;
        dist += to_string(num);
    }

    ifs.close();
    
    record.total = stol(time);
    record.max = stol(dist);
}

// better way to do this - 
// use binary search
long find_options(race& r) {
    long total = r.total, start, end;
    for (long t = 0; t < total; t++) {
        if (r.is_valid(t)) {
            start = t;
            break;
        }
    }
    for (long t = total; t >= 0; t--) {
        if (r.is_valid(t)) {
            end = t;
            break;
        }
    }
    return end-start+1;
}

int main() {
    race record(0, 0);
    parse_record("input.txt", record);

    cout << find_options(record) << endl;
}
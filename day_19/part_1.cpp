// File Name:       day_19/part_1.cpp
// Author:          Samuel Wang (@samuelpswang)
// Purpose:         AoC 2023 Day 19 Part 1 (https://adventofcode.com/2023/day/19)
// Last Updated:    04 Jan 2023

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
using namespace std;

enum prop { x, m, a, s };

struct part {
    int x, m, a, s;
    part(int x, int m, int a, int s): x(x), m(m), a(a), s(s) {}
    int sum() const { return x + m + a + s; }
};

struct rule {
    prop p; 
    bool g, f;
    int v;
    string gt;
    rule() {}

    rule(char pc, char gc, const string& vs, const string& gt): f(false), gt(gt) {
        switch (pc) {
            case 'x': 
                p = x;
                break;
            case 'm':
                p = m;
                break;
            case 'a':
                p = a;
                break;
            case 's':
                p = s;
                break;
        }
        if (gc == '>') g = true;
        else g = false;
        v = stoi(vs);
    }

    rule(const string& gt): f(true), gt(gt) {}

    string operator()(const part& pt) {
        if (f) return gt;
        switch (p) {
            case x:
                if (g) return (pt.x > v ? gt : "next");
                else return (pt.x < v ? gt : "next");
            case m:
                if (g) return (pt.m > v ? gt : "next");
                else return (pt.m < v ? gt : "next");
            case a:
                if (g) return (pt.a > v ? gt : "next");
                else return (pt.a < v ? gt : "next");
            case s:
                if (g) return (pt.s > v ? gt : "next");
                else return (pt.s < v ? gt : "next");
        }
    }
};

struct flow {
    vector<rule> rv;
    string operator()(const part& pt) {
        string dest = "";
        for (int i = 0; i < rv.size(); ++i) {
            dest = rv[i](pt);
            if (dest != "next") break;
        }
        return dest;
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

int main() {
    ifstream input("input.txt");
    if (!input) exit(1);

    map<string,flow> flowm;
    vector<part> partv;
    string line;
    while (getline(input, line) && line != "") {
        size_t ne = line.find('{');
        string name(line.begin(), line.begin()+ne);
        flow cf;
        vector<string> rsv = split(string(line.begin()+ne+1, line.end()-1), ",");
        for (const string& s: rsv) {
            size_t colon_pos = s.find(':');
            rule cr;
            if (s.find(':') == string::npos) cr = rule(s);
            else cr = rule(s[0], s[1], string(s.begin()+2, s.begin()+colon_pos), string(s.begin()+colon_pos+1, s.end()));
            cf.rv.push_back(cr);
        }
        flowm[name] = cf;
    }

    while (getline(input, line)) {
        int v[4];
        size_t prev = line.find('=');
        for (int i = 0; i < 4; ++i) {
            size_t curr = line.find('=', prev+1);
            if (i == 3) v[i] = stoi(string(line.begin()+prev+1, line.end()-1));
            else v[i] = stoi(string(line.begin()+prev+1, line.begin()+curr-2));
            prev = curr;
        }
        partv.emplace_back(v[0], v[1], v[2], v[3]);
    }
    input.close();

    long long total = 0;
    for (const part& p: partv) {
        string dest = flowm["in"](p);
        while (dest != "A" && dest != "R") {
            dest = flowm[dest](p);
        }
        if (dest == "A") {
            total += p.sum();
        }
    }
    cout << total << endl;
}

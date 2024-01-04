// File Name:       day_20/part_1.cpp
// Author:          Samuel Wang (@samuelpswang)
// Purpose:         AoC 2023 Day 20 Part 1 (https://adventofcode.com/2023/day/20)
// Last Updated:    04 Jan 2023

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
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

bool contains(const vector<string>& output, const string& name) {
    for (const string& s: output) {
        if (s == name) return true;
    }
    return false;
}

struct mod {
    string name;
    string type;
    vector<string> output;
    map<string,string> memory;
    mod() {}
    mod(const string& name, const string& type, const vector<string>& output): \
        name(name), type(type), output(output) {
        if (type == "%") memory["state"] = "off";
    }
};

int main() {
    ifstream input("input.txt");
    if (!input) exit(1);

    // read setup
    string line;
    map<string,mod> modules;
    vector<string> broadcast;
    while (getline(input, line)) {
        vector<string> sep = split(line, " -> ");
        string type_name(sep[0]), out(sep[1]);
        if (type_name == "broadcaster") {
            broadcast = split(out, ", ");
        } else {
            string type(type_name.begin(), type_name.begin()+1);
            string name(type_name.begin()+1, type_name.end());
            vector<string> output = split(out, ", ");
            modules[name] = mod(name, type, output);
        }
    }

    // set conjunction modules
    for (const pair<string,mod>& p: modules) {
        if (p.second.type == "&") {
            for (const pair<string,mod>& cp: modules) {
                if (contains(cp.second.output, p.second.name))
                    modules[p.second.name].memory[cp.second.name] = "lo";
            } 
        }
    }

    // run machine
    map<string,long long> tally = {{"lo", 0}, {"hi", 0}};
    for (int i = 0; i < 1000; i++) {
        ++tally["lo"];
        queue<vector<string>> q;
        for (const string& name: broadcast) {
            q.push({"broadcaster", name, "lo"});
        }
        
        while (!q.empty()) {
            string src(q.front()[0]), dst(q.front()[1]), sig(q.front()[2]);
            q.pop();
            ++tally[sig];
            
            if (modules.find(dst) == modules.end()) continue;
            
            if (modules[dst].type == "%") {
                if (sig == "lo") {
                    if (modules[dst].memory["state"] == "off") {
                        modules[dst].memory["state"] = "on";
                        for (const string& o: modules[dst].output) {
                            q.push({dst, o, "hi"});
                        }
                    } else {
                        modules[dst].memory["state"] = "off";
                        for (const string& o: modules[dst].output) {
                            q.push({dst, o, "lo"});
                        }
                    }
                }
            } else {
                modules[dst].memory[src] = sig;
                bool is_all_hi = true;
                for (const pair<string,string>& cp: modules[dst].memory) {
                    if (cp.second != "hi") {
                        is_all_hi = false;
                        break;
                    }
                }
                if (is_all_hi) {
                    for (const string& o: modules[dst].output) {
                        q.push({dst, o, "lo"});
                    }
                } else {
                    for (const string& o: modules[dst].output) {
                        q.push({dst, o, "hi"});
                    }   
                }
            }
        }
    }

    cout << (long long) tally["hi"] * (long long) tally["lo"] << endl;
}

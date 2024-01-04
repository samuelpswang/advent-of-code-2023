// File Name:       day_20/part_2.cpp
// Author:          Samuel Wang (@samuelpswang)
// Purpose:         AoC 2023 Day 20 Part 2 (https://adventofcode.com/2023/day/20)
// Last Updated:    04 Jan 2023

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <numeric>
#include <cassert>
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

    // find who feeds into rx module
    // we assume this is just one conjunction module
    vector<string> feedv;
    for (const pair<string,mod>& p: modules) {
        if (contains(p.second.output, "rx")) {
            feedv.push_back(p.first);
        }
    }
    assert(feedv.size() == 1);
    string feed(feedv[0]);
    
    // find who feeds into the conjunction module and find their cycle
    map<string,int> cycles;
    map<string,bool> seen;
    for (const pair<string,mod>& p: modules) {
        if (contains(p.second.output, feed)) {
            cycles[p.first] = 0;
            seen[p.first] = false;
        }
    }

    // run machine
    bool found = false;
    int presses = 0;
    long long lowest_presses = 1;
    while (!found) {
        ++presses;
        // cout << presses << endl;

        // add braodcast signal
        queue<vector<string>> q;
        for (const string& name: broadcast) {
            q.push({"broadcaster", name, "lo"});
        }
        
        // process
        while (!q.empty()) {
            string src(q.front()[0]), dst(q.front()[1]), sig(q.front()[2]);
            q.pop();

            // eliminate bad outputs
            if (modules.find(dst) == modules.end()) continue;

            // check if we have the values we need
            bool is_target = (cycles.find(src) != cycles.end());
            bool is_not_seen = (!seen[src]);
            bool is_going_to_feed = (dst == feed);
            bool is_high = (sig == "hi");
            bool is_written = false;
            if (is_target && is_not_seen && is_going_to_feed && is_high) {
                cycles[src] = presses;
                seen[src] = true;
            }

            // check if we have everything
            bool is_all_seen = true;
            for (const pair<string,int>& p: cycles) {
                is_all_seen = is_all_seen && seen[p.first];
            }
            if (is_all_seen) {
                found = true;
                for (const pair<string,int>& p: cycles) {
                    lowest_presses = lcm(lowest_presses, p.second);
                }
                break;
            }
            
            // update enxt outputs
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

    cout << lowest_presses << endl;
}

// based on @hyper-neutrino's solution - which assumes that: 1) there is only
// one module that feeds into rx and it is a conjunction moculde, and 2) the
// modules that feeds into that conjunction module will repeat its behavior

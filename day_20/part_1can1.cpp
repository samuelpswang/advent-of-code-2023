#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
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

struct mod {
    int type; // 0: broadcaster, 1: flip-flop, 2: conjunction
    string name;
    map<string,bool> memory;
    vector<string> output;
    mod(const string& modname, const vector<string>& out, const vector<string>& in): output(out) {
        assert(modname[0] == '&');
        if (modname[0] == '&') {
            type = 2;
            name = modname.substr(1);
            for (const string& instr: in) {
                memory[string(instr.begin()+1, instr.end())] = false;
            }
        }
    }
    mod(const string& modname, const vector<string>& out): output(out) {
        if (modname == "broadcaster") {
            type = 0;
            name = modname;
            memory = {};
        } else if (modname[0] == '%') {
            type = 1;
            name = modname.substr(1);
            memory["state"] = false;
        }
    }
    void process(map<string,map<string,bool>>& in, map<string,map<string,bool>>& out) {
        if (type == 0) {
            assert(in[name].size() <= 1); // assert that only one or no signal is passed to a broadcaster at a time
            if (in[name].size() == 1) {
                for (const string& outstr: output) {
                    out[outstr]["broadcaster"] = false;
                }
            }
        } else if (type == 1) {
            assert(in[name].size() <= 1); // assert that only one or no signal is passed to a flip-flop at a time
            if (in[name].size() == 1) {
                for (const pair<string,bool>& p: in[name]) {
                    if (!p.second) memory["state"] = !memory["state"];
                    break;
                }
                for (const string& outstr: output) {
                    out[outstr][name] = memory["state"];
                }
            }
        } else if (type == 2) {
            assert(in[name].size() <= memory.size()); // assert that a conjuction unit cannot accept more than its designated input
            if (in[name].size() != 0) {
                for (const pair<string,bool>& p: in[name]) {
                    memory[p.first] = p.second;
                }
                bool all_high = true;
                for (const pair<string,bool>& p: memory) {
                    if (!p.second) {
                        all_high = false;
                        break;
                    }
                }
                for (const string& outstr: output) {
                    out[outstr][name] = !all_high;
                }
            }
        }
    }
};

pair<int,int> tally(const map<string,map<string,bool>>& signal) {
    int high = 0, low = 0;
    for (const pair<string,map<string,bool>>& p1: signal) {
        for (const pair<string,bool>& p2: p1.second) {
            if (p2.second) ++high;
            else ++low;
        }
    }
    return {high, low};
}

void print(const map<string,map<string,bool>>& signal) {
    for (const pair<string,map<string,bool>>& p1: signal) {
        cout << p1.first << "{ ";
        for (const pair<string,bool>& p2: p1.second) {
            cout << p2.first << ": " << p2.second << ", ";
        }
        cout << " }" << endl;
    }
}

void prune(map<string,map<string,bool>>& signal) {
    for (auto it = signal.cbegin(); it != signal.cend();) {
        if (it->second.size() == 0) signal.erase(it++);
        else ++it;
    }
}

int main() {
    // read data
    ifstream input("test.txt");
    if (!input) exit(1);
    string line;
    map<string,vector<string>> data;
    while (getline(input, line)) {
        size_t div = line.find(" -> ");
        string name(line.begin(), line.begin()+div);
        data[name] = split(string(line.begin()+div+4, line.end()), ", ");
    }
    input.close();
    cout << "parse done" << endl;

    // construct
    vector<mod> vmod;
    for (const pair<string,vector<string>>& p: data) {
        if (p.first[0] == '&') {
            string name(p.first.begin()+1, p.first.end());
            vector<string> in;
            for (const pair<string,vector<string>>& cp: data) {
                if (find(cp.second.begin(), cp.second.end(), name) != cp.second.end())
                    in.push_back(cp.first);
            }
            vmod.emplace_back(p.first, p.second, in);
        } else {
            vmod.emplace_back(p.first, p.second);
        }
    }
    cout << "construct done" << endl;
    
    // process
    map<string,map<string,bool>> signal, next_signal;
    int time = 1, count = 0, high = 0, low = 0;
    while (count < time) {
        bool start = true;
        int subcount = 0;
        signal["broadcaster"]["button"] = false;
        while (signal != next_signal && subcount < 50) {
            cout << endl << endl << subcount << endl;
            cout << "> signal: " << endl;
            print(signal);
            cout << endl << "> next_signal: " << endl;
            print(next_signal);

            if (!start) signal = next_signal;
            else start = false;
            next_signal = map<string,map<string,bool>>();
            for (mod& m: vmod) {
                m.process(signal, next_signal);
            }
            ++subcount;
            prune(signal);
            pair<int,int> p = tally(signal);
            high += p.first;
            low += p.second;
        }
        ++count;
    }
    cout << "high: " << high << endl;
    cout << "low: " << low << endl;
}

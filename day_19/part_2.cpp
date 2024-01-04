#include <iostream>
#include <fstream>
#include <string>
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

struct rule {
    char opd;
    char ieq;
    int val;
    string dest;
    rule(char opd, char ieq, int val, const string& dest): opd(opd), ieq(ieq), val(val), dest(dest) {}
};

int main() {
    ifstream input("input.txt");
    if (!input) exit(1);
    
    string line;
    map<string,vector<rule>> flows;
    while (getline(input, line)) {
        if (line.size() == 0 || line[0] == '{') continue;
        vector<string> sep = split(line, "{");
        string flow_name(sep[0]), flow(sep[1].begin(), sep[1].end()-1);
        flows[flow_name] = {};
        sep = split(flow, ",");
        for (const string& r: sep) {
            if (r.find(':') != string::npos) {
                size_t colon = r.find(':');
                // operand, inequality, value, destination
                flows[flow_name].emplace_back( \
                    r.at(0), \
                    r.at(1), \
                    stoi(string(r.cbegin()+2,r.cbegin()+colon)), \
                    string(r.cbegin()+colon+1, r.cend())
                );
            } else {
                flows[flow_name].emplace_back('_', '_', '_', r);
            }
        }
    }

    long long total = 0;
    map<char,int> trans = {{'x', 0}, {'m', 1}, {'a', 2}, {'s', 3}};
    queue<pair<string,vector<long long>>> q;
    q.push({"in", {1, 4000, 1, 4000, 1, 4000, 1, 4000}});
    
    while (!q.empty()) {
        string state(q.front().first);
        vector<long long> bound(q.front().second);
        q.pop();
        if (bound[0] > bound[1] || bound[2] > bound[3] || bound[4] > bound[5] || bound[6] > bound[7]){
            continue;
        } else if (state == "R") {
            continue;
        } else if (state == "A") {
            total += (bound[1]-bound[0]+1) * (bound[3]-bound[2]+1) * (bound[5]-bound[4]+1) * (bound[7]-bound[6]+1);
        }

        vector<rule> rules = flows[state];
        for (const rule& r: rules) {
            // last rule
            if (r.opd == '_') {
                q.push({r.dest, bound});
                continue;
            }
            // other rules
            int ind = trans[r.opd]*2;
            if (r.ieq == '<') {
                if (r.val <= bound[ind]) { // all false - continue in this flow
                    continue;
                } else if (bound[ind] < r.val && r.val <= bound[ind+1]) { // split - process
                    vector<long long> true_bound(bound), false_bound(bound);
                    true_bound[ind+1] = r.val-1;
                    false_bound[ind] = r.val;
                    bound = false_bound;
                    q.push({r.dest, true_bound});
                } else { // all true - exit flow
                    q.push({r.dest, bound});
                    break;
                }
            } else {
                if (r.val < bound[ind]) { // all true - exit flow
                    q.push({r.dest, bound});
                    break;
                } else if (bound[ind] <= r.val && r.val < bound[ind+1]) { // split - process
                    vector<long long> true_bound(bound), false_bound(bound);
                    true_bound[ind] = r.val+1;
                    false_bound[ind+1] = r.val;
                    bound = false_bound;
                    q.push({r.dest, true_bound});
                } else { // all false - continue in this flow
                    continue;
                }
            }
        }
    }

    cout << total << endl;
}

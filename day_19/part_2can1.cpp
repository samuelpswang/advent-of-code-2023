#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
#include <map>
using namespace std;

enum prop { x, m, a, s };

struct range {
    int xu, xl, mu, ml, au, al, su, sl;
    string state;

    range(): xu(4000), xl(1), mu(4000), ml(1), au(4000), al(1), su(4000), sl(1), state("in") {}

    long long size() const { return (long long) (xu-xl+1) * (long long) (mu-ml+1) * (long long) (au-al+1) * (long long) (su-sl+1); }

    void print() const { printf("%d %d %d %d %d %d %d %d %lld\n", xu, xl, mu, ml, au, al, su, sl, size()); }
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

    vector<range> operator()(const range& r) {
        if (f) {
            range nr(r);
            nr.state = gt;
            return {nr};
        } else if (p == x) {
            if (g) {
                if (v < r.xl) {
                    range nr(r);
                    nr.state = gt;
                    return {nr};
                } else if (r.xu <= v) {
                    range nr(r);
                    nr.state = "next";
                    return {nr};
                } else {
                    range ur(r), lr(r);
                    ur.xl = v+1;
                    ur.state = gt;
                    lr.xu = v;
                    lr.state = "next";
                    return {ur, lr};
                }
            } else {
                if (v <= r.xl) {
                    range nr(r);
                    nr.state = "next";
                    return {nr};
                } else if (r.xu < v) {
                    range nr(r);
                    nr.state = gt;
                    return {nr};
                } else { 
                    range ur(r), lr(r);
                    ur.xl = v;
                    ur.state = "next";
                    lr.xu = v-1;
                    lr.state = gt;
                    return {ur, lr};
                }
            }
        } else if (p == m) {
            if (g) {
                if (v < r.ml) {
                    range nr(r);
                    nr.state = gt;
                    return {nr};
                } else if (r.mu <= v) {
                    range nr(r);
                    nr.state = "next";
                    return {nr};
                } else {
                    range ur(r), lr(r);
                    ur.ml = v+1;
                    ur.state = gt;
                    lr.mu = v;
                    lr.state = "next";
                    return {ur, lr};
                }
            } else {
                if (v <= r.ml) {
                    range nr(r);
                    nr.state = "next";
                    return {nr};
                } else if (r.mu < v) {
                    range nr(r);
                    nr.state = gt;
                    return {nr};
                } else { 
                    range ur(r), lr(r);
                    ur.ml = v;
                    ur.state = "next";
                    lr.mu = v-1;
                    lr.state = gt;
                    return {ur, lr};
                }
            }
        } else if (p == a) {
            if (g) {
                if (v < r.al) {
                    range nr(r);
                    nr.state = gt;
                    return {nr};
                } else if (r.au <= v) {
                    range nr(r);
                    nr.state = "next";
                    return {nr};
                } else {
                    range ur(r), lr(r);
                    ur.al = v+1;
                    ur.state = gt;
                    lr.au = v;
                    lr.state = "next";
                    return {ur, lr};
                }
            } else {
                if (v <= r.al) {
                    range nr(r);
                    nr.state = "next";
                    return {nr};
                } else if (r.au < v) {
                    range nr(r);
                    nr.state = gt;
                    return {nr};
                } else { 
                    range ur(r), lr(r);
                    ur.al = v;
                    ur.state = "next";
                    lr.au = v-1;
                    lr.state = gt;
                    return {ur, lr};
                }
            }
        } else if (p == s) {
            if (g) {
                if (v < r.sl) {
                    range nr(r);
                    nr.state = gt;
                    return {nr};
                } else if (r.su <= v) {
                    range nr(r);
                    nr.state = "next";
                    return {nr};
                } else {
                    range ur(r), lr(r);
                    ur.sl = v+1;
                    ur.state = gt;
                    lr.su = v;
                    lr.state = "next";
                    return {ur, lr};
                }
            } else {
                if (v <= r.sl) {
                    range nr(r);
                    nr.state = "next";
                    return {nr};
                } else if (r.su < v) {
                    range nr(r);
                    nr.state = gt;
                    return {nr};
                } else { 
                    range ur(r), lr(r);
                    ur.sl = v;
                    ur.state = "next";
                    lr.su = v-1;
                    lr.state = gt;
                    return {ur, lr};
                }
            }
        } else {
            return {};
        }
    }
};

struct flow {
    string name;
    vector<rule> rv;
    void operator()(map<string,vector<range>>& old_map, map<string,vector<range>>& new_map) {
        for (const range& r: old_map[name]) {
            int rc = 0;
            queue<range> q, nq;
            q.push(r);
            while (rc < rv.size()) {
                while (!q.empty()) {
                    range cr = q.front();
                    q.pop();
                    vector<range> vnr = rv[rc](cr);
                    for (const range& nr: vnr) {
                        if (nr.state == "next") nq.push(nr);
                        else new_map[nr.state].push_back(nr);
                    }
                }
                ++rc;
                q = nq;
            }
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

int map_size(const map<string,vector<range>>& m) {
    int size = 0;
    for (const pair<string,vector<range>>& p: m) {
        size += p.second.size();
    }
    return size; 
}

int main() {
    ifstream input("test.txt");
    if (!input) exit(1);

    map<string,flow> flowm;
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
        cf.name = name;
        flowm[name] = cf;
    }
    input.close();
    
    // dictionary method - can't quite figure out what I am doing or what is wrong there
    // we know that total is more than actual because of duplicates though
    range init;
    map<string,vector<range>> old_map, new_map;
    old_map["in"] = {init};
    int count = 1;
    while (map_size(old_map) != 0) {
        for (const pair<string,vector<range>>& p: old_map) {
            if (p.first == "A" || p.first == "R") {
            //     for (const range& r: p.second) {
            //         total += r.size();
            //         ++count;
            //     }
            // } else if (p.first == "R") {
                continue;
            } else {
                flowm[p.first](old_map, new_map);
            }
        }
        old_map = new_map;
        new_map = map<string,vector<range>>();
    }

    long long total = 0;
    for (const vector<range>& r: new_map["A"]) {
        total += r.size();
    }
    cout << total << endl;
}

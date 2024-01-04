#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <cassert>
using namespace std;

unordered_set<long> universe(long upp, long low) {
    unordered_set<long> ns;
    for (long n = low; n <= upp; ++n) {
        ns.insert(n);
    }
    return ns;
}

unordered_set<long> factor_with_offset(long diff, long offset, long upp, long low) {
    unordered_set<long> ns;
    for (long n = low; n <= upp; ++n) {
        if (diff % (n-offset) == 0) {
            ns.insert(n);
        }
    }
    return ns;
}

unordered_set<long> intersection(const unordered_set<long>& lhs, const unordered_set<long>& rhs) {
    unordered_set<long> ns;
    for (long n: lhs) {
        if (rhs.find(n) != rhs.end()) ns.insert(n);
    }
    return ns;
}

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
    string line;
    vector<vector<long>> data;
    while (getline(input, line)) {
        vector<long> stone_data;
        vector<string> sp1 = split(line, " @ ");
        vector<string> sp2 = split(sp1[1], ", ");
        sp1 = split(sp1[0], ", ");
        for (const string& s: sp1) {
            stone_data.push_back(stol(s));
        }
        for (const string& s: sp2) {
            stone_data.push_back(stol(s));
        }
        data.push_back(stone_data);
    }
    input.close();

    // find rock speed
    // for every parallel hailstone: diff % (v_rock - v_hailstone) = 0
    unordered_map<long,vector<long>> vx, vy, vz;
    for (const vector<long>& hs: data) {
        vx[hs[3]].push_back(hs[0]);
        vy[hs[4]].push_back(hs[1]);
        vz[hs[5]].push_back(hs[2]);
    }
    unordered_set<long> vxrs(universe(500, -500)), vyrs(universe(500, -500)), vzrs(universe(500, -500));
    for (const pair<long,vector<long>>& p: vx) {
        if (p.second.size() == 2) vxrs = intersection(vxrs, factor_with_offset(p.second[1]-p.second[0], p.first, 500, -500));
        if (vxrs.size() == 1) break;
    }
    for (const pair<long,vector<long>>& p: vy) {
        if (p.second.size() == 2) vyrs = intersection(vyrs, factor_with_offset(p.second[1]-p.second[0], p.first, 500, -500));
        if (vyrs.size() == 1) break;
    }
    for (const pair<long,vector<long>>& p: vz) {
        if (p.second.size() == 2) vzrs = intersection(vzrs, factor_with_offset(p.second[1]-p.second[0], p.first, 500, -500));
        if (vzrs.size() == 1) break;
    }
    assert(vxrs.size() == 1);
    assert(vyrs.size() == 1);
    assert(vzrs.size() == 1);

    // prepping vairalbes
    double vxr = *vxrs.begin(), vyr = *vyrs.begin(), vzr = *vzrs.begin();
    double x1 = data[0][0], y1 = data[0][1], vx1 = data[0][3], vy1 = data[0][4];
    double x2 = data[1][0], y2 = data[1][1], vx2 = data[1][3], vy2 = data[1][4];
    
    // find initial position
    // eliminate k and m:
    // xr + k * vxr = x1 + k * vx1 
    // yr + k * vyr = y1 + k * vy1 -> (xr - x1) / (vxr - vx1) = (yr - y1) / (vyr - vy1) ...(1)
    // xr + m * vxr = x2 + m * vx2
    // yr + m * vyr = y2 + m * vy2 -> (xr - x2) / (vxr - vx2) = (yr - y2) / (vyr - vy2) ...(2)
    // combining (1) and (2):
    // (vyr - vy1) * (xr) + (-vxr + vx1) * (yr) = (vyr - vy1) * (x1) + (-vxr + vx1) * (y1)
    // (vyr - vy2) * (xr) + (-vxr + vx2) * (yr) = (vyr - vy2) * (x2) + (-vxr + vx2) * (y2)
    // we then solve:
    double a = (vyr - vy1), b = (-vxr + vx1), c = (vyr - vy2), d = (-vxr + vx2);
    double k1 = (a * x1 + b * y1), k2 = (c * x2 + d * y2);
    double det = 1 / (a * d - b * c);
    double ia = det * (d), ib = det * (-b), ic = det * (-c), id = det * (a);
    double xr = ia * k1 + ib * k2;
    double yr = ic * k1 + id * k2;

    // find z coordinate
    // (xr - x1) / (vxr - vx1) = (zr - z1) / (vzr - vz1)
    // zr = [(xr - x1) / (vxr - vx1) * (vzr - vz1)] + z1
    double z1 = data[0][2], vz1 = data[0][5];
    double zr = ((xr - x1) / (vxr - vx1) * (vzr - vz1)) + z1;
    
    long fxr = round(xr), fyr = round(yr), fzr = round(zr);
    cout << ((long long) fxr + (long long) fyr + (long long) fzr) << endl;
}

// originally took @hyper-neutrino's lazy method, just writing down the
// equations with "sympy" in Python and finding integer solutions - but later on
// when translating solutions to c++ - a search was conducted for a non-linear
// numeric solver, to no avail - but when re-watching @jonthanpaulson's YouTube
// solutions, I came across @every116's comments, which used parallel hailstones
// to find the speed of the rock and then the initial position, which is what
// this solution is based on.

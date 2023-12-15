#include <iostream> 
#include <fstream>
#include <vector>
#include <string>
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

// brute-force method does not scale -
// revised dynamic programming method
long arrangements(map<vector<long>,long>& dp, const string& pattern, \
                 const vector<long>& data, long pat_ind, long data_ind, \
                 long curr_hash) {
    // cout << pattern << endl; 
    // stopping condition
    if (pat_ind == pattern.size()) {
        if (data_ind == data.size() && curr_hash == 0) return 1;
        else if (data_ind == data.size()-1 && curr_hash == data.back()) return 1;
        else return 0;
    }

    // avoid repeating
    if (dp.find({pat_ind, data_ind, curr_hash}) != dp.end()) {
        return dp.at({pat_ind, data_ind, curr_hash});
    }

    // increment condition
    long ans = 0;
    // choosing .
    if (pattern[pat_ind] == '.' || pattern[pat_ind] == '?') { 
        // . before a # block
        if (curr_hash == 0) {
            ans += arrangements(dp, pattern, data, pat_ind+1, data_ind, 0);
        }
        // . after a # block
        if (data_ind < data.size() && curr_hash == data[data_ind]) {
            ans += arrangements(dp, pattern, data, pat_ind+1, data_ind+1, 0);
        }
    }
    // choosing #
    if (pattern[pat_ind] == '#' || pattern[pat_ind] == '?') { 
        ans += arrangements(dp, pattern, data, pat_ind+1, data_ind, curr_hash+1);
    }
    dp[{pat_ind, data_ind, curr_hash}] = ans;
    return ans;
}

int main() {
    ifstream input("input.txt");
    if (!input) exit(1);
    
    string pattern, data;
    vector<string> vpat;
    vector<vector<long>> vdata;
    while (getline(input, pattern, ' ')) {
        getline(input, data);
        vector<string> str_data = split(data, ",");
        vector<long> num_data;
        for (long i = 0; i < 5; ++i) {
            for (const string& s: str_data) {
                num_data.push_back(stoi(s));
            }
        }
        string new_pattern;
        for (long i = 0; i < 4; ++i) { new_pattern += pattern + '?'; }
        new_pattern += pattern;
        vpat.push_back(new_pattern);
        vdata.push_back(num_data);
    }
    input.close();

    long long sum = 0;
    for (long i = 0; i < vpat.size(); i++) {
        map<vector<long>,long> dp;
        sum += arrangements(dp, vpat[i], vdata[i], 0, 0, 0);
    }
    
    cout << sum << endl;
}
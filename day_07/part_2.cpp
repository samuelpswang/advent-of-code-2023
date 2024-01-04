// File Name:       day_07/part_2.cpp
// Author:          Samuel Wang (@samuelpswang)
// Purpose:         AoC 2023 Day 7 Part 2 (https://adventofcode.com/2023/day/7)
// Last Updated:    04 Jan 2023

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <unordered_map>
using namespace std;

enum hand_type {
    five_of_a_kind = 6,
    four_of_a_kind = 5,
    full_house = 4,
    three_of_a_kind = 3,
    two_pair = 2,
    one_pair = 1,
    high_card = 0
};

class Hand {
public:
    Hand(const string& s, int bid): bid(bid) {
        for (char c: s) {
            switch (c) {
                case 'A':
                    cards.push_back(12);
                    count[12]++;
                    break;
                case 'K':
                    cards.push_back(11);
                    count[11]++;
                    break;
                case 'Q':
                    cards.push_back(10);
                    count[10]++;
                    break;
                case 'J':
                    cards.push_back(0);
                    count[0]++;
                    break;
                case 'T':
                    cards.push_back(9);
                    count[9]++;
                    break;
                default:
                    int num = c - '0' - 1;
                    cards.push_back(num);
                    count[num]++;
                    break;
            }
        }
        
        if (is_five_of_a_kind()) type = five_of_a_kind;
        else if (is_four_of_a_kind()) type = four_of_a_kind;
        else if (is_full_house()) type = full_house;
        else if (is_three_of_a_kind()) type = three_of_a_kind;
        else if (is_two_pair()) type = two_pair;
        else if (is_one_pair()) type = one_pair;
        else type = high_card;
    }

    int get_bid() const { return bid; }

    friend bool operator>(const Hand& lhs, const Hand& rhs);
    friend bool operator==(const Hand& lhs, const Hand& rhs);
    friend bool operator<(const Hand& lhs, const Hand& rhs);

private:
    vector<int> cards;
    unordered_map<int,int> count;
    hand_type type;
    int bid;
    
    bool is_five_of_a_kind() {
        if (count[0] == 5) return true;
        for (const pair<int,int>& cp: count) {
            if (cp.first == 0) continue;
            if (cp.second == 5 || (cp.second + count[0]) == 5) return true;
        }
        return false;
    }

    bool is_four_of_a_kind() {
        if (count[0] == 4) return true;
        bool has_four = false;
        for (const pair<int,int>& cp: count) {
            if (cp.first == 0) continue;
            if (cp.second == 4 || (cp.second + count[0]) == 4) has_four = true;
        }
        return has_four;
    }
    
    bool is_full_house() {
        bool has_one = false, has_two = false, has_three = false;
        int has_two_count = 0;
        switch (count[0]) {
            case 3:
                for (const pair<int,int>& cp: count) {
                    if (cp.first == 0) continue;
                    if (cp.second == 2) has_two = true;
                }
                return has_two;
            case 2:
                for (const pair<int,int>& cp: count) {
                    if (cp.first == 0) continue;
                    if (cp.second == 3) has_three = true;
                    if (cp.second == 2) has_two = true;
                    if (cp.second == 1) has_two = true;
                }
                return (has_three || (has_two && has_one));
            case 1:
                for (const pair<int,int>& cp: count) {
                    if (cp.first == 0) continue;
                    if (cp.second == 2) has_two_count++;
                }
                return (has_two_count == 2);
            case 0:
                for (const pair<int,int>& cp: count) {
                    if (cp.first == 0) continue;
                    if (cp.second == 3) has_three = true;
                    if (cp.second == 2) has_two = true;
                }
                return has_three && has_two;
        }
        return false;
    }

    bool is_three_of_a_kind() {
        bool has_three = false, has_two = false;
        switch (count[0]) {
            case 3:
            case 2:
                return true;
            case 1:
                for (const pair<int,int>& cp: count) {
                    if (cp.first == 0) continue;
                    if (cp.second == 2) has_two = true;
                }
                return has_two;
            case 0:
                for (const pair<int,int>& cp: count) {
                    if (cp.first == 0) continue;
                    if (cp.second == 3) has_three = true;
                    if (cp.second == 2) has_two = true;
                }
                return has_three && !has_two;
        }
        return false;
    }

    bool is_two_pair() {
        bool has_two = false, has_one = false;
        int two_count = 0;
        switch (count[0]) {
            case 1:
                for (const pair<int,int>& cp: count) {
                    if (cp.first == 0) continue;
                    if (cp.second == 2) has_two = true;
                    if (cp.second == 1) has_one = true;
                }
                return has_two && has_one;
            case 0:
                for (const pair<int,int>& cp: count) {
                    if (cp.second == 2) two_count++;
                }
                return (two_count == 2);
        }
        return false;
    }

    bool is_one_pair() {
        switch (count[0]) {
            case 1:
                return true;
            case 0:
                int two_count = 0;
                for (const pair<int,int>& cp: count) {
                    if (cp.second == 2) two_count++;
                }
                return (two_count == 1);
        }
        return false;
    }
};

struct greater_hand {
    bool operator()(const Hand& lhs, const Hand& rhs) {
        return lhs > rhs;
    }
};

bool operator>(const Hand& lhs, const Hand& rhs) {
    if (lhs.type > rhs.type) return true;
    if (lhs.type < rhs.type) return false;
    return lhs.cards > rhs.cards;
}

bool operator==(const Hand& lhs, const Hand& rhs) {
    return (lhs.cards == rhs.cards);
}

bool operator<(const Hand& lhs, const Hand& rhs) {
    return !((lhs == rhs) || (lhs > rhs));
}

void parse_hands(const string& filename, priority_queue<Hand, vector<Hand>, greater_hand>& pq) {
    ifstream ifs(filename);
    if (!ifs) exit(1);
    
    string h;
    int b;
    while (ifs >> h >> b) {
        Hand new_hand(h, b);
        pq.push(new_hand);
    }

    ifs.close();
}

long find_winning(priority_queue<Hand, vector<Hand>, greater_hand>& pq) {
    int hand_rank = 1;
    long sum = 0;
    while (!pq.empty()) {
        int hand_bid = pq.top().get_bid();
        sum += hand_bid * hand_rank;
        hand_rank++;
        pq.pop();
    }
    return sum;
}

int main() {
    priority_queue<Hand, vector<Hand>, greater_hand> pq;
    parse_hands("input.txt", pq);
    cout <<find_winning(pq) << endl;
}

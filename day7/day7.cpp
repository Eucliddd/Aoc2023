#include <fstream>
#include <string>
#include <array>
#include <sstream>
#include <iostream>
#include <map>
#include <queue>
#include <algorithm>

using namespace std;

class Hand{
public:
    static map<char,int> orders;
    virtual int level(const string& s) = 0;
    Hand(string s, int b): cards(s), bid(b){};
    inline int get_bid() const noexcept{
        return bid;
    }
    inline string get_cards() const noexcept{
        return cards;
    }
protected:
    string cards;
    int _level;
    int bid;
};

class Hand1: public Hand{
public:
    Hand1(string s, int b): Hand(s, b){
        _level = level(s);
    };        
    int level(const string& s) override;
    static map<char,int> orders;
    friend bool operator<(const Hand1& h1, const Hand1& h2){
        if(h1._level != h2._level){
            return h1._level > h2._level;
        }
        for(int i = 0; i < 5; i++){
            if(orders[h1.cards[i]] != orders[h2.cards[i]]){
                return orders[h1.cards[i]] > orders[h2.cards[i]];
            }
        }
        return false;
    }
};

map<char,int> Hand1::orders = {
    {'2',0},
    {'3',1},
    {'4',2},
    {'5',3},
    {'6',4},
    {'7',5},
    {'8',6},
    {'9',7},
    {'T',8},
    {'J',9},
    {'Q',10},
    {'K',11},
    {'A',12}
};

int Hand1::level(const string& s){
    map<char,int> buckets;
    for(auto e: s){
        buckets[e]++;
    }
    vector<int> values;
    transform(buckets.begin(), buckets.end(), back_inserter(values), [](auto &e){return e.second;});
    sort(values.begin(), values.end(), greater<int>());
    if(values[0] == 5) return 6;
    if(values[0] == 4) return 5;
    if(values[0] == 3 && values[1] == 2) return 4;
    if(values[0] == 3 && values[1] == 1) return 3;
    if(values[0] == 2 && values[1] == 2) return 2;
    if(values[0] == 2 && values[1] == 1) return 1;
    return 0;
}


class Hand2: public Hand{
public:
    Hand2(string s, int b): Hand(s, b){
        _level = level(s);
    };        
    int level(const string& s) override;
    static map<char,int> orders;
    friend bool operator<(const Hand2& h1, const Hand2& h2){
        if(h1._level != h2._level){
            return h1._level > h2._level;
        }
        for(int i = 0; i < 5; i++){
            if(orders[h1.cards[i]] != orders[h2.cards[i]]){
                return orders[h1.cards[i]] > orders[h2.cards[i]];
            }
        }
        return false;
    }
};

map<char,int> Hand2::orders = {
    {'J',0},
    {'2',1},
    {'3',2},
    {'4',3},
    {'5',4},
    {'6',5},
    {'7',6},
    {'8',7},
    {'9',8},
    {'T',9},
    {'Q',10},
    {'K',11},
    {'A',12}
};

int Hand2::level(const string& s){
    map<char,int> buckets;
    for(auto e: s){
        buckets[e]++;
    }
    int max_freq = 0;
    char max_char = ' ';
    for(auto e: buckets){
        if(e.second > max_freq && e.first != 'J'){
            max_freq = e.second;
            max_char = e.first;
        }
    }
    if(buckets.find('J') != buckets.end()){
        buckets[max_char] += buckets['J'];
        buckets.erase('J');
    }
    vector<int> values;
    transform(buckets.begin(), buckets.end(), back_inserter(values), [](auto &e){return e.second;});
    sort(values.begin(), values.end(), greater<int>());
    if(values[0] == 5) return 6;
    if(values[0] == 4) return 5;
    if(values[0] == 3 && values[1] == 2) return 4;
    if(values[0] == 3 && values[1] == 1) return 3;
    if(values[0] == 2 && values[1] == 2) return 2;
    if(values[0] == 2 && values[1] == 1) return 1;
    return 0;
}

int puzzle1(){
    ifstream infile("input.txt");
    string line;
    priority_queue<Hand1> pq;
    while(getline(infile, line)){
        stringstream ss(line);
        string cards;
        int bid;
        ss >> cards >> bid;
        pq.push(Hand1(cards, bid));
    }
    infile.close();
    int sum = 0;
    int rank = 1;
    while(!pq.empty()){
        const Hand& h = pq.top();
        cout << h.get_cards() << " " << h.get_bid() << " " << rank << endl;
        sum += h.get_bid()*rank;
        pq.pop();
        rank++;
    }
    return sum;
}

int puzzle2(){
    ifstream infile("input.txt");
    string line;
    priority_queue<Hand2> pq;
    while(getline(infile, line)){
        stringstream ss(line);
        string cards;
        int bid;
        ss >> cards >> bid;
        pq.push(Hand2(cards, bid));
    }
    infile.close();
    int sum = 0;
    int rank = 1;
    while(!pq.empty()){
        const Hand& h = pq.top();
        cout << h.get_cards() << " " << h.get_bid() << " " << rank << endl;
        sum += h.get_bid()*rank;
        pq.pop();
        rank++;
    }
    return sum;
}

int main(){
    cout << puzzle2() << endl;
    return 0;
}
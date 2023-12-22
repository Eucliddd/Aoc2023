#include <fstream>
#include <string>
#include <array>
#include <sstream>
#include <iostream>
#include <map>
#include <queue>

using namespace std;




class Hand{
public:
    static map<char,int> orders;
    static int level(const string& s);
    Hand(string s, int b): cards(s), bid(b){}
    friend bool operator<(const Hand& h1, const Hand& h2){
        if(level(h1.cards) != level(h2.cards)){
            return level(h1.cards) > level(h2.cards);
        }
        for(int i = 0; i < 5; i++){
            if(orders[h1.cards[i]] != orders[h2.cards[i]]){
                return orders[h1.cards[i]] > orders[h2.cards[i]];
            }
        }
        return false;
    }
    inline int get_bid() const noexcept{
        return bid;
    }
    inline string get_cards() const noexcept{
        return cards;
    }
private:
    string cards;
    int bid;
};

map<char,int> Hand::orders = {
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

int Hand::level(const string& s){
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

int puzzle1(){
    ifstream infile("input.txt");
    string line;
    priority_queue<Hand> pq;
    while(getline(infile, line)){
        stringstream ss(line);
        string cards;
        int bid;
        ss >> cards >> bid;
        pq.push(Hand(cards, bid));
    }
    infile.close();
    int sum = 0;
    int rank = 1;
    while(!pq.empty()){
        Hand h = pq.top();
        pq.pop();
        cout << h.get_cards() << " " << h.get_bid() << " " << rank << endl;
        sum += h.get_bid()*rank;
        rank++;
    }
    return sum;
}

int main(){
    cout << puzzle1() << endl;
    return 0;
}
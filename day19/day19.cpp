#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <queue>

using namespace std;

map<string,vector<string>> rules;
vector<array<int, 4>> parts;
const string name = "xmas";

inline bool match(char key, char op , int val, const array<int,4>& arr){
    static map<char,int> m = {{'x',0},{'m',1},{'a',2},{'s',3}};
    switch(op){
        case '<':
            return arr[m[key]] < val;
        case '>':
            return arr[m[key]] > val;
    }
    return false;
}

int puzzle1(){
    int sum = 0;
    for(auto& arr : parts){
        auto cur_r = rules["in"];
        bool accepted = false;
        bool rejected = false;
        while(!accepted && !rejected){
            bool matched = false;
            for(int i = 0; i < cur_r.size()-1; i++){
                string rule = cur_r[i];
                string next_r;
                char key,op;
                int val;
                char carr[16];
                sscanf(rule.c_str(),"%c%c%d:%s",&key,&op,&val,carr);
                next_r.assign(carr);
                if(match(key,op,val,arr)){
                    matched = true;
                    if (next_r == "A") {
                        accepted = true;
                        break;
                    }
                    else if(next_r == "R") {
                        rejected = true;
                        break;
                    }
                    else cur_r = rules[next_r];
                    break;
                }
            }
            if(!matched){
                string rule = cur_r[cur_r.size()-1];
                if(rule == "A") {
                    accepted = true;
                    break;
                }
                else if(rule == "R") {
                    rejected = true;
                    break;
                }
                cur_r = rules[rule];
            }
        }
        if(accepted){
            for(int i = 0; i < 4; i++){
                sum += arr[i];
            }
        }
    }
    return sum;
}

using interval = array<pair<int,int>,4>;

struct Node{
    string rule;
    vector<Node*> children;
    interval i;
};

vector<Node*> accept_nodes;

Node* build_tree(string rule){
    static map<char,int> m = {{'x',0},{'m',1},{'a',2},{'s',3}};
    queue<Node*> q;
    Node* root = new Node();
    root->rule = rule;
    for(int i = 0; i < 4; i++)
        root->i[i] = {1,4000};
    q.push(root);
    while(!q.empty()){
        Node* cur = q.front();
        q.pop();
        auto cur_r = rules[cur->rule];
        interval cur_i = cur->i;
        for(int i = 0; i < cur_r.size()-1; i++){
            string rule = cur_r[i];
            string next_r;
            char key,op;
            int val;
            char carr[16];
            sscanf(rule.c_str(),"%c%c%d:%s",&key,&op,&val,carr);
            next_r.assign(carr); 

            Node* child = new Node();
            child->rule = next_r;
            for(int i = 0; i < 4; i++)
                child->i[i] = cur_i[i];
            switch (op)
            {
            case '<':
                child->i[m[key]].second = min(child->i[m[key]].second,val-1);
                cur_i[m[key]].first = max(cur_i[m[key]].first,val);
                break;
            case '>':
                child->i[m[key]].first = max(child->i[m[key]].first,val+1);
                cur_i[m[key]].second = min(cur_i[m[key]].second,val);
                break;
            }
            cur->children.push_back(child);
            if(next_r != "A" && next_r != "R")
                q.push(child);
            if(next_r == "A")
                accept_nodes.push_back(child);
        }
        string rule = cur_r[cur_r.size()-1];
        Node* child = new Node();
        child->rule = rule;
        for(int i = 0; i < 4; i++)
            child->i[i] = cur_i[i];
        cur->children.push_back(child);
        if(rule != "A" && rule != "R")
            q.push(child);
        if(rule == "A")
            accept_nodes.push_back(child);
    }
    return root;
} 

uint64_t puzzle2(){
    uint64_t sum = 0;
    Node* root = build_tree("in");
    for(auto n : accept_nodes){
        uint64_t nums = 1;
        for(int i = 0; i < 4; i++){
            nums *= (n->i[i].second - n->i[i].first + 1);
        }
        sum += nums;
    }
    return sum;
}

int main(){
    ifstream infile("input.txt");
    string line;
    while(getline(infile,line)){
        if(line == "")
            break;
        string key = line.substr(0,line.find("{"));
        line.erase(0,line.find("{")+1);
        line.erase(line.find("}"),1);
        vector<string> values;
        while(line.find(",") != string::npos){
            values.push_back(line.substr(0,line.find(",")));
            line.erase(0,line.find(",")+1);
        }
        values.push_back(line);
        rules[key] = values;
    }
    
    while(getline(infile,line)){
        line.erase(0,1);
        line.erase(line.find("}"),1);
        array<int,4> part;
        for(int i = 0; i < 4; i++){
            line.erase(0,line.find("=")+1);
            part[i] = stoi(line.substr(0,line.find(",")));
            line.erase(0,line.find(",")+1);
        }
        parts.push_back(part);
    }
    cout << puzzle2() << endl;
}
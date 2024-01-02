#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <map>

using namespace std;

map<string,vector<string>> rules;
vector<array<int, 4>> parts;
const string name = "xmas";

int puzzle1(){
    auto cur_r = rules["in"];
    for(auto& arr : parts){
        for(int i = 0; i < cur_r.size()-1; i++){
            string rule = cur_r[i];
            char key,op;
            string next_r;
            int val;
            sscanf(rule.c_str(),"%c%c%d:%s",key,op,val,next_r);
        }
    }
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
        rules[key] = std::move(values);
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
}
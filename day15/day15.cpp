#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>

using namespace std;

int HASH(const string& s){
    int result = 0;
    for(int i = 0; i < s.size(); i++){
        result += s[i];
        result = result * 17 % 256;
    }
    return result;
}

int puzzle1(){
    ifstream fin("input.txt");
    int sum = 0;
    string s = "";
    while(!fin.eof()){
        char c;
        fin >> c;
        if(c == ',' || c == '\n') {
            sum += HASH(s);
            s.clear();
            continue;
        }
        s.push_back(c);
    }
    s.erase(s.end()-1);
    sum += HASH(s);
    return sum;
}

int puzzle2(){
    ifstream fin("input.txt");
    int sum = 0;
    array<vector<pair<string,int>>,256> boxes;
    string s = "";
    while(!fin.eof()){
        char c;
        fin >> c;
        if(c == ',' || c == '\n') {
            if(s.find('=')!=string::npos){
                string key = s.substr(0,s.find('='));
                int value = stoi(s.substr(s.find('=')+1));
                int idx = HASH(key);
                auto& box = boxes[idx];
                auto it = find_if(box.begin(),box.end(),[&key](const pair<string,int>& p){return p.first == key;});
                if(it != box.end()){
                    it->second = value;
                }
                else{
                    box.push_back(make_pair(key,value));
                }
            }
            else if(s.find('-')!=string::npos){
                string key = s.substr(0,s.find('-'));
                int idx = HASH(key);
                auto& box = boxes[idx];
                auto it = find_if(box.begin(),box.end(),[&key](const pair<string,int>& p){return p.first == key;});
                if(it != box.end()){
                    box.erase(it);
                }
            }
            s.clear();
            continue;
        }
        s.push_back(c);
    }
    s.erase(s.end()-1);
    
    if(s.find('=')!=string::npos){
        string key = s.substr(0,s.find('='));
        int value = stoi(s.substr(s.find('=')+1));
        int idx = HASH(key);
        auto& box = boxes[idx];
        auto it = find_if(box.begin(),box.end(),[&key](const pair<string,int>& p){return p.first == key;});
        if(it != box.end()){
            it->second = value;
        }
        else{
            box.push_back(make_pair(key,value));
        }
    }
    else if(s.find('-')!=string::npos){
        string key = s.substr(0,s.find('-'));
        int idx = HASH(key);
        auto& box = boxes[idx];
        auto it = find_if(box.begin(),box.end(),[&key](const pair<string,int>& p){return p.first == key;});
        if(it != box.end()){
            box.erase(it);
        }
    }
    for(int i = 0 ; i < 256; i++){
        for (int j = 0; j < boxes[i].size(); j++){
            sum += (i + 1) * (j + 1) * boxes[i][j].second;
        }
    }
    return sum;
}

int main(){
    
    cout << puzzle2() << endl;
}
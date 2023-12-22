#include <fstream>
#include <string>
#include <array>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

const string start = "AAA";
const string dest = "ZZZ";

int puzzle1(const string& instructions, map<string,pair<string,string>>& maps){
    
    string current = start;
    int times = 0;
    while(true){
        for(auto e: instructions){
            if(e == 'L'){
                current = maps[current].first;
            }else{
                current = maps[current].second;
            }
        }
        times += instructions.size();
        if(current == dest){
            break;
        }
    }
    return times;
}

uint64_t puzzle2(const string& instructions, map<string,pair<string,string>>& maps){
    vector<string> starts;
    vector<int> times;
    for(auto e: maps){
        if(e.first[2] == 'A'){
            starts.emplace_back(e.first);
            times.emplace_back(0);
        }
    }
    for(int i = 0; i < starts.size(); i++){
        string current = starts[i];
        while(true){
            for(auto e: instructions){
                if(e == 'L'){
                    current = maps[current].first;
                }else{
                    current = maps[current].second;
                }
            }
            times[i] += instructions.size();
            if(current[2] == 'Z'){
                break;
            }
        }
    }
    for(int i = 0; i < starts.size(); i++){
        cout << starts[i] << " " << times[i] << endl;
    }

    auto lcm = [](uint64_t a, uint64_t b){
        uint64_t m = a;
        uint64_t n = b;
        if(m < n){
            swap(m,n);
        }
        while(m % n != 0){
            int temp = m % n;
            m = n;
            n = temp;
        }
        return a * b / n;
    };
    uint64_t result = times[0];
    for(int i = 1; i < times.size(); i++){
        result = lcm(result, times[i]);
    }
    return result;
}

int main(){
    string instructions;
    map<string,pair<string,string>> maps;
    ifstream file("input.txt");
    string line;
    getline(file,instructions);
    getline(file,line);
    while (getline(file,line))
    {
        string node = line.substr(0,3);
        string left = line.substr(7,3);
        string right = line.substr(12,3);
        maps[node] = make_pair(left,right);
    }
    file.close();
    cout << puzzle2(instructions, maps) << endl;
    return 0;
}
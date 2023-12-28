#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>

using namespace std;

const int rows = 100;
const int cols = 100;

using grid = array<string, rows>;

int puzzle1(grid& map){
    int sum = 0;
    for(int i = 1; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(map[i][j] == 'O'){
                int k = i-1;
                while(k >= 0 && map[k][j] == '.') k--;
                map[i][j] = '.';  
                map[k+1][j] = 'O';
            }
        }
    }

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(map[i][j] == 'O'){
                sum += rows - i;  
            }
        }
    }
    return sum;
}

void north(grid& map){
    for(int i = 1; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(map[i][j] == 'O'){
                int k = i-1;
                while(k >= 0 && map[k][j] == '.') k--;
                map[i][j] = '.';  
                map[k+1][j] = 'O';
            }
        }
    }
}

void west(grid& map){
    for(int i = 0; i < rows; i++){
        for(int j = 1; j < cols; j++){
            if(map[i][j] == 'O'){
                int k = j-1;
                while(k >= 0 && map[i][k] == '.') k--;
                map[i][j] = '.';  
                map[i][k+1] = 'O';
            }
        }
    }
}

void south(grid& map){
    for(int i = rows-2; i >= 0; i--){
        for(int j = 0; j < cols; j++){
            if(map[i][j] == 'O'){
                int k = i+1;
                while(k < rows && map[k][j] == '.') k++;
                map[i][j] = '.';  
                map[k-1][j] = 'O';
            }
        }
    }
}

void east(grid& map){
    for(int i = 0; i < rows; i++){
        for(int j = cols-2; j >= 0; j--){
            if(map[i][j] == 'O'){
                int k = j+1;
                while(k < cols && map[i][k] == '.') k++;
                map[i][j] = '.';  
                map[i][k-1] = 'O';
            }
        }
    }
}

int circle(grid& map){
    int sum = 0;
    north(map);
    west(map);
    south(map);
    east(map);
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(map[i][j] == 'O'){
                sum += rows - i;  
            }
        }
    }
    return sum;
}

// 构建next数组的下一项
void getNext(vector<int>& next, const vector<int>& s){
    if(s.size() == 1){
        next.push_back(0);
        return;
    }
    int j = next[next.size()-1];
    while(j > 0 && s[s.size()-1] != s[j]) j = next[j];
    if(s[s.size()-1] == s[j]) next.push_back(j+1);
    else next.push_back(0);
}

// 将results数组倒过来，构建next数组
pair<int,int> reverseNext(const vector<int>& s){
    vector<int> next;
    next.push_back(0);
    for(int i = s.size()-2, j = 0; i >= 0; i--){
        while(j > 0 && s[i] != s[s.size() - 1 - j]) j = next[j];
        if(s[i] == s[s.size() - 1 - j]) j++;
        next.push_back(j);
    }

    // for(int i = 0; i < next.size(); i++){
    //     cout << next[i] << " ";
    // }
    // cout << endl;

    // for(int i = 0; i < s.size(); i++){
    //     cout << s[i] << " ";
    // }
    // cout << endl;

    for(int i = 0; i < next.size(); i++){
        if (next[i] * 2 == i + 1){
            return make_pair(next.size() - i - 1, next[i]);
        }   
    }
    return make_pair(0, 0);
}

int puzzle2(grid& map){
    vector<int> results;
    int start = 0;
    int len = 0;
    while(true){
        for(int i = 0; i < 1000; i++){
            int sum = circle(map);
            results.push_back(sum);
        }

        auto p = reverseNext(results);
        if(p.first != 0 && p.second != 0) {
            start = p.first;
            len = p.second;
            break;
        }
    }

    // int n = results.size();
    // int p = 1000000000 % n;
    // return results[p];
    int n = (1000000000 - start - 1) % len + start;
    return results[n];
}

int main(){
    ifstream file("input.txt");
    string line;
    grid map;
    int i = 0;
    while(getline(file, line)){
        map[i] = line;
        i++;
    }
    cout << puzzle2(map) << endl;
    return 0;
}
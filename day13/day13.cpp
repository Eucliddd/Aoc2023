#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>

using namespace std;

int puzzle1(const vector<string>& records){
    int rows = records.size();
    int cols = records[0].size();
    // find vertical symmetry axis
    for(int i = 0; i < cols - 1; i++){
        int p = i,q = i+1;
        bool flag = true;
        while(p >= 0 && q < cols){
            for(int j = 0; j < rows; j++){
                if(records[j][p] != records[j][q]){
                    flag = false;
                    break;
                }
            }
            if(!flag){
                break;
            }
            p--;
            q++;
        }
        if(flag){
            cout << "Vertical symmetry axis: " << i + 1 << endl;
            return i + 1;
        }
    }
    // find horizontal symmetry axis
    for(int i = 0; i < rows - 1; i++){
        int p = i,q = i+1;
        bool flag = true;
        while(p >= 0 && q < rows){
            for(int j = 0; j < cols; j++){
                if(records[p][j] != records[q][j]){
                    flag = false;
                    break;
                }
            }
            if(!flag){
                break;
            }
            p--;
            q++;
        }
        if(flag){
            cout << "Horizontal symmetry axis: " << i + 1 << endl;
            return (i+1)*100;
        }
    }
    return 0;
}

int puzzle2(const vector<string>& records){
    int rows = records.size();
    int cols = records[0].size();
    bool vertical = false;
    bool horizontal = false;
    int vertical_axis = -1;
    int horizontal_axis = -1;
    int possible_vertical = -1;
    int possible_horizontal = -1;
    for(int i = 0; i < cols - 1; i++){
        int p = i,q = i+1;
        bool flag = true;
        int diff = 0;
        while(p >= 0 && q < cols){
            for(int j = 0; j < rows; j++){
                if(records[j][p] != records[j][q]){
                    flag = false;
                    diff++;
                }
            }
            p--;
            q++;
        }
        if(flag){
            vertical = true;
            vertical_axis = i + 1;
        }
        else if(diff == 1){
            possible_vertical = i + 1;
        }
    }
    for(int i = 0; i < rows - 1; i++){
        int p = i,q = i+1;
        bool flag = true;
        int diff = 0;
        while(p >= 0 && q < rows){
            for(int j = 0; j < cols; j++){
                if(records[p][j] != records[q][j]){
                    flag = false;
                    diff++;
                }
            }
            p--;
            q++;
        }
        if(flag){
            horizontal = true;
            horizontal_axis = i + 1;
        }
        else if(diff == 1){
            possible_horizontal = i + 1;
        }
    }

    // if(vertical) cout << "Vertical symmetry axis: " << vertical_axis << endl;
    // if(horizontal) cout << "Horizontal symmetry axis: " << horizontal_axis << endl;
    // if(possible_vertical != -1) cout << "Possible vertical symmetry axis: " << possible_vertical << endl;
    // if(possible_horizontal != -1) cout << "Possible horizontal symmetry axis: " << possible_horizontal << endl;
    if(possible_vertical != -1) return possible_vertical;
    if(possible_horizontal != -1) return possible_horizontal*100;
    return 0;
}

int main(){
    ifstream infile("input.txt");
    string line;
    int sum = 0;
    vector<string> records;
    while(getline(infile, line)){
        if(line.size() == 0){
            sum += puzzle2(records);
            records.clear();
        }
        else{
            records.push_back(line);
        }
    }
    sum += puzzle2(records);
    cout << sum << endl;
}
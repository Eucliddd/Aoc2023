#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>

using namespace std;

const int rows = 140;
const int cols = 140;

int puzzle1(array<array<char, cols>, rows> grid){
    vector<int> empty_rows;
    vector<int> empty_cols;
    for(int i = 0; i < rows; i++){
        bool empty = true;
        for(int j = 0; j < cols; j++){
            if(grid[i][j] == '#'){
                empty = false;
                break;
            }
        }
        if(empty) empty_rows.push_back(i);
    }
    for(int i = 0; i < cols; i++){
        bool empty = true;
        for(int j = 0; j < rows; j++){
            if(grid[j][i] == '#'){
                empty = false;
                break;
            }
        }
        if(empty) empty_cols.push_back(i);
    }
    vector<pair<int,int>> galaxies;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(grid[i][j] == '#'){
                auto it = lower_bound(empty_rows.begin(), empty_rows.end(), i);
                int row = it - empty_rows.begin();
                it = lower_bound(empty_cols.begin(), empty_cols.end(), j);
                int col = it - empty_cols.begin();
                galaxies.push_back(make_pair(i+row, j+col));
            }
        }
    }
    int sum = 0;
    for(int i = 0; i < galaxies.size(); i++){
        for(int j = i+1; j < galaxies.size(); j++){
            sum += abs(galaxies[i].first - galaxies[j].first) + abs(galaxies[i].second - galaxies[j].second);
        }
    }
    return sum;
}

uint64_t puzzle2(array<array<char, cols>, rows> grid){
    vector<int> empty_rows;
    vector<int> empty_cols;
    for(int i = 0; i < rows; i++){
        bool empty = true;
        for(int j = 0; j < cols; j++){
            if(grid[i][j] == '#'){
                empty = false;
                break;
            }
        }
        if(empty) empty_rows.push_back(i);
    }
    for(int i = 0; i < cols; i++){
        bool empty = true;
        for(int j = 0; j < rows; j++){
            if(grid[j][i] == '#'){
                empty = false;
                break;
            }
        }
        if(empty) empty_cols.push_back(i);
    }
    vector<pair<int64_t,int64_t>> galaxies;
    const uint64_t offset = 1000000;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(grid[i][j] == '#'){
                auto it = lower_bound(empty_rows.begin(), empty_rows.end(), i);
                int row = it - empty_rows.begin();
                it = lower_bound(empty_cols.begin(), empty_cols.end(), j);
                int col = it - empty_cols.begin();
                galaxies.push_back(make_pair(i+row*(offset - 1), j+col*(offset - 1)));
            }
        }
    }
    int64_t sum = 0;
    for(int i = 0; i < galaxies.size(); i++){
        for(int j = i+1; j < galaxies.size(); j++){
            sum += abs(galaxies[i].first - galaxies[j].first) + abs(galaxies[i].second - galaxies[j].second);
        }
    }
    return sum;
}

int main(){
    ifstream input("input.txt");
    string line;
    array<array<char, cols>, rows> grid;
    int row = 0;
    while(getline(input, line)){
        for(int col = 0; col < line.size(); col++){
            grid[row][col] = line[col];
        }
        row++;
    }
    cout << puzzle2(grid) << endl;
}
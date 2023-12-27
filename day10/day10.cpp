#include <fstream>
#include <iostream>
#include <array>
#include <string>
#include <bitset>
#include <vector>
#include <stack>

using namespace std;

const int rows = 140;
const int cols = 140;

enum class Direction {
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3
};

inline bool isConnected(char a, char b, Direction dir){
    if(a == 'S'){
        if(dir == Direction::UP && (b == '|' || b == '7' || b == 'F')) return true;
        if(dir == Direction::DOWN && (b == '|' || b == 'J' || b == 'L')) return true;
        if(dir == Direction::RIGHT && (b == '-' || b == '7' || b == 'J')) return true;
        if(dir == Direction::LEFT && (b == '-' || b == 'F' || b == 'L')) return true;
    }
    else if(a == '|'){
        if(dir == Direction::UP && (b == '|' || b == '7' || b == 'F' || b == 'S')) return true;
        if(dir == Direction::DOWN && (b == '|' || b == 'J' || b == 'L' || b == 'S')) return true;
    }
    else if(a == '-'){
        if(dir == Direction::RIGHT && (b == '-' || b == '7' || b == 'J' || b == 'S')) return true;
        if(dir == Direction::LEFT && (b == '-' || b == 'F' || b == 'L' || b == 'S')) return true;
    }
    else if(a == '7'){
        if(dir == Direction::DOWN && (b == '|' || b == 'J' || b == 'L' || b == 'S')) return true;
        if(dir == Direction::LEFT && (b == '-' || b == 'F' || b == 'L' || b == 'S')) return true;
    }
    else if(a == 'J'){
        if(dir == Direction::UP && (b == '|' || b == '7' || b == 'F' || b == 'S')) return true;
        if(dir == Direction::LEFT && (b == '-' || b == 'F' || b == 'L' || b == 'S')) return true;
    }
    else if(a == 'F'){
        if(dir == Direction::DOWN && (b == '|' || b == 'J' || b == 'L' || b == 'S')) return true;
        if(dir == Direction::RIGHT && (b == '-' || b == '7' || b == 'J' || b == 'S')) return true;
    }
    else if(a == 'L'){
        if(dir == Direction::UP && (b == '|' || b == '7' || b == 'F' || b == 'S')) return true;
        if(dir == Direction::RIGHT && (b == '-' || b == '7' || b == 'J' || b == 'S')) return true;
    }
    return false;
}


inline char getConnector(int s_row, int s_col, int row1, int col1, int row2, int cow2){
    int min_col = min(col1, cow2);
    int max_col = max(col1, cow2);
    int min_row = min(row1, s_row);
    int max_row = max(row1, s_row);
    if(min_col == max_col) return '|';
    if(min_row == max_row) return '-';
    if(min_col < s_col){
        if(min_row < s_row) return 'J';
        else return '7';
    }
    else{
        if(min_row < s_row) return 'L';
        else return 'F';
    }
    return 'S';
}

inline bool isWall(char a){
    if(a == '|' || a == '7' || a == 'F' ) return true;
    return false;

}

int DFS(int row, int col, array<array<char, cols>, rows>& grid, array<bitset<cols>,rows>& visited, bool& found, vector<int>& path){
    if (row < 0 || row >= rows || col < 0 || col >= cols) return 0;
    if(visited[row][col]){
        if(grid[row][col] == 'S' && path.size() > 2){
            found = true;
            return path.size();
        }
        return 0;
    }
    visited[row][col] = true;
    path.emplace_back(row * cols + col);
    if (grid[row][col] == '.') return 0;
    if (isConnected(grid[row][col], grid[row - 1][col], Direction::UP)) {
        int result = DFS(row - 1, col, grid, visited, found, path);
        if(found) return result;
    }
    if (isConnected(grid[row][col], grid[row + 1][col], Direction::DOWN)) {
        int result = DFS(row + 1, col, grid, visited, found, path);
        if(found) return result;
    }
    if (isConnected(grid[row][col], grid[row][col - 1], Direction::LEFT)) {
        int result = DFS(row, col - 1, grid, visited, found, path);
        if(found) return result;
    }
    if (isConnected(grid[row][col], grid[row][col + 1], Direction::RIGHT)) {
        int result = DFS(row, col + 1, grid, visited, found, path);
        if(found) return result;
    }
    path.pop_back();
    return 0;
}

int puzzle1(array<array<char, cols>, rows>& grid) {
    int result = 0;
    int s_row = 0;
    int s_col = 0;
    bool found = false;
    for(int row = 0; row < rows; row++) {
        for(int col = 0; col < cols; col++) {
            if (grid[row][col] == 'S')
            {
                s_row = row;
                s_col = col;
                found = true;
                break;
            }
        }
        if(found) break;
    }
    int row = s_row;
    int col = s_col;
    array<bitset<cols>,rows> visited;
    found = false;
    vector<int> path;
    result = DFS(row, col, grid, visited, found, path)/2;
   
    return result;
}

int puzzle2(array<array<char, cols>, rows>& grid) {
    int result = 0;
    int s_row = 0;
    int s_col = 0;
    bool found = false;
    for(int row = 0; row < rows; row++) {
        for(int col = 0; col < cols; col++) {
            if (grid[row][col] == 'S')
            {
                s_row = row;
                s_col = col;
                found = true;
                break;
            }
        }
        if(found) break;
    }
    int row = s_row;
    int col = s_col;
    array<bitset<cols>,rows> visited;
    found = false;
    vector<int> path;
    DFS(row, col, grid, visited, found, path);
    grid[s_row][s_col] = getConnector(s_row, s_col, path[1]/cols, path[1]%cols, path[path.size()-1]/cols, path[path.size()-1]%cols);
    cout << grid[s_row][s_col] << endl;
    sort(path.begin(), path.end());
    bool flag = false;
    row = path[0]/cols;
    col = path[0]%cols - 1;
    for(auto e: path){
        int c_r = e/cols;
        int c_c = e%cols;
        if(c_r != row){
            row = c_r;
            col = c_c - 1;
            flag = false;
        }
        if(flag) result += c_c - col - 1;
        if(isWall(grid[c_r][c_c])){
            flag = !flag;
        }
        col = c_c;
    }
    return result;
}

int main(){
    ifstream file("input.txt");
    string line;
    array<array<char, cols>, rows> grid;
    int row = 0;
    while (getline(file, line))
    {
        for (int col = 0; col < line.size(); col++)
        {
            grid[row][col] = line[col];
        }
        row++;
    }
    cout << "Puzzle 2: " << puzzle2(grid) << endl;
}
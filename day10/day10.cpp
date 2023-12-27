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

bool isConnected(char a, char b, Direction dir){
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

int DFS(int row, int col, array<array<char, cols>, rows>& grid, array<bitset<cols>,rows>& visited, bool& found, vector<int>& path){
    if (row < 0 || row >= rows || col < 0 || col >= cols) return 0;
    if(visited[row][col]){
        if(grid[row][col] == 'S' && path.size() > 2){
            found = true;
            return path.size()/2;
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
    result = DFS(row, col, grid, visited, found, path);
   
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
    cout << "Puzzle 1: " << puzzle1(grid) << endl;
}
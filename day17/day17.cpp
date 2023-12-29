#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <bitset>
#include <stack>

using namespace std;

const int rows = 5;
const int cols = 5;
using grid = array<array<uint16_t,cols>,rows>;
const int max_dist = 1000000;

enum class Direction: uint8_t{
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3
};

struct Node
{
    int x;
    int y;
    Direction dir;
    int len;
};

int _next[4][2] = {{0,1},{0,-1},{-1,0},{1,0}};


// WRONG!!!
int dijkstra(grid& g, int start_row = 0, int start_col = 0, int end_row = rows - 1, int end_col = cols - 1){
    int dist[rows][cols][4][4] = {max_dist};
    bool visited[rows][cols][4][4] = {0};
    Node prev[rows][cols][4][4] = {Node{-1,-1,Direction::UP,0}};
    dist[start_row][start_col][0][1] = 0;

    dist[start_row+1][start_col][static_cast<int>(Direction::RIGHT)][2] = g[start_row+1][start_col];
    prev[start_row+1][start_col][static_cast<int>(Direction::RIGHT)][2] = Node{start_row,start_col,Direction::UP,1};

    dist[start_row][start_col+1][static_cast<int>(Direction::DOWN)][2] = g[start_row][start_col+1];
    prev[start_row][start_col+1][static_cast<int>(Direction::DOWN)][2] = Node{start_row,start_col,Direction::UP,1};
    

    for(int i = 0; i < rows * cols * 16; i++){
        int min_dist = INT_MAX;
        Node min_node = Node{-1,-1,Direction::UP,0};
        for(int row = 0; row < rows; row++){
            for(int col = 0; col < cols; col++){
                for(int dir = 0; dir < 4; dir++){
                    for(int len = 1; len <= 3; len++){
                        if(!visited[row][col][dir][len] && dist[row][col][dir][len] < min_dist){
                            min_dist = dist[row][col][dir][len];
                            min_node = Node{row,col,static_cast<Direction>(dir),len};
                        }
                    }
                }
            }
        }
        if(min_dist == INT_MAX)
            break;
        visited[min_node.x][min_node.y][static_cast<int>(min_node.dir)][min_node.len] = 1;
        if(min_node.x == end_row && min_node.y == end_col){
            int min = INT_MAX;
            for(int dir = 0; dir < 4; dir++){
                for(int len = 1; len <= 3; len++){
                    if(dist[end_row][end_col][dir][len] < min)
                        min = dist[end_row][end_col][dir][len];
                }
            }
            return min;
        }
        
        for(int j = 0; j < 4; j++){
            int next_x = min_node.x + _next[j][0];
            int next_y = min_node.y + _next[j][1];
            if(next_x < 0 || next_x >= rows || next_y < 0 || next_y >= cols)
                continue;
            if(next_x == min_node.x && next_y == min_node.y)
                continue;
        }
    }

    return -1;
}



void DFS(grid& g, int x, int y, int step, int dist, array<bitset<cols>,rows>& visited, vector<int>& path, int& min_dist){
    if(x == rows - 1 && y == cols - 1){
        if(dist < min_dist) {
            min_dist = dist;
            for(int i = 0; i < step; i++){
                cout << path[i] << " ";
            }
            cout << endl;
            cout << min_dist << endl;
        }
        return;
    }
    for(int i = 0; i < 4; i++){
        int next_x = x + _next[i][0];
        int next_y = y + _next[i][1];
        if(next_x >= 0 && next_x < rows && next_y >= 0 && next_y < cols && !visited[next_x][next_y]){
            // TODO: check if the path is contiguous
            if(step >= 3){
                int prev_x = path[step-2] / cols;
                int prev_y = path[step-2] % cols;
                int prev_prev_x = path[step-3] / cols;
                int prev_prev_y = path[step-3] % cols;
                if(next_x == x && x == prev_x && prev_x == prev_prev_x){
                    if (step >= 4 && prev_prev_x == path[step-4] / cols)
                        continue;
                }
                if(next_y == y && y == prev_y && prev_y == prev_prev_y){
                    if (step >= 4 && prev_prev_y == path[step-4] % cols)
                        continue;
                }
            }
            if(dist + g[next_x][next_y] >= min_dist)
                continue;
            visited[next_x][next_y] = 1;
            path[step] = next_x * cols + next_y;
            DFS(g, next_x, next_y, step + 1, dist + g[next_x][next_y], visited, path, min_dist);
            visited[next_x][next_y] = 0;
        }
    }
    return;
}

int puzzle1(grid& g){
    // cannot use dijkstra, because it doesn't satisfy the optimal substructure property due to the restriction on the straight line
    // so we use DFS instead
    array<bitset<cols>,rows> visited;
    vector<int> path(rows * cols);
    int min_dist = INT_MAX;
    visited[0][0] = 1;
    path[0] = 0;
    DFS(g, 0, 0, 1, 0, visited, path, min_dist);
    return min_dist;
}

int main(){
    ifstream file("input.txt");
    string line;
    grid g;
    int row = 0;
    while(getline(file, line)){
        for(int col = 0; col < line.size(); col++){
            g[row][col] = line[col] - '0';
        }
        row++;
    }
    cout << puzzle1(g) << endl;
}
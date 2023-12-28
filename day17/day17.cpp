#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <bitset>
#include <stack>

using namespace std;

const int rows = 13;
const int cols = 13;
using grid = array<array<uint16_t,cols>,rows>;
const int max_dist = 1000000;

// WRONG!!!
int dijkstra(grid& g, int start_row = 0, int start_col = 0, int end_row = rows - 1, int end_col = cols - 1){
    vector<vector<int>> dist(rows, vector<int>(cols, max_dist));
    vector<bitset<cols>> visited(rows, bitset<cols>(0));
    vector<vector<int>> prev(rows, vector<int>(cols, -1));
    dist[start_row][start_col] = 0;
    dist[start_row+1][start_col] = g[start_row+1][start_col];
    dist[start_row][start_col+1] = g[start_row][start_col+1];
    prev[start_row][start_col] = start_row * cols + start_col;

    for(int i = 0; i < rows * cols; i++){
        int min_dist = INT_MAX;
        int min_row = -1;
        int min_col = -1;
        for(int row = 0; row < rows; row++){
            for(int col = 0; col < cols; col++){
                if(!visited[row][col] && dist[row][col] < min_dist){
                    min_dist = dist[row][col];
                    min_row = row;
                    min_col = col;
                }
            }
        }
        if(min_row == -1 || min_col == -1)
            break;
        visited[min_row][min_col] = 1;
        if(min_row == end_row && min_col == end_col){
            return dist[end_row][end_col];
        }

        bool horizontal_contiguous = false;
        bool vertical_contiguous = false;
        if(prev[min_row][min_col] != -1){
            int prev_row = prev[min_row][min_col] / cols;
            int prev_col = prev[min_row][min_col] % cols;
            if(prev[prev_row][prev_col] != -1){
                int prev_prev_row = prev[prev_row][prev_col] / cols;
                int prev_prev_col = prev[prev_row][prev_col] % cols;
                if(min_row == prev_row && prev_row == prev_prev_row){
                    horizontal_contiguous = true;
                }
                else if(min_col == prev_col && prev_col == prev_prev_col){
                    vertical_contiguous = true;
                }
                if (prev[prev_prev_row][prev_prev_col] != -1){
                    int prev_prev_prev_row = prev[prev_prev_row][prev_prev_col] / cols;
                    int prev_prev_prev_col = prev[prev_prev_row][prev_prev_col] % cols;
                    if(horizontal_contiguous && min_row != prev_prev_prev_row){
                        horizontal_contiguous = false;
                    }
                    if(vertical_contiguous && min_col != prev_prev_prev_col){
                        vertical_contiguous = false;
                    }
                }
            }
        }

        if(!vertical_contiguous && min_row > 0 && !visited[min_row-1][min_col] && dist[min_row-1][min_col] > min_dist + g[min_row-1][min_col]){
            dist[min_row-1][min_col] = min_dist + g[min_row-1][min_col];
            prev[min_row-1][min_col] = min_row * cols + min_col;
        }
        if(!vertical_contiguous && min_row < rows - 1 && !visited[min_row+1][min_col] && dist[min_row+1][min_col] > min_dist + g[min_row+1][min_col]){
            dist[min_row+1][min_col] = min_dist + g[min_row+1][min_col];
            prev[min_row+1][min_col] = min_row * cols + min_col;
        }
        if(!horizontal_contiguous && min_col > 0 && !visited[min_row][min_col-1] && dist[min_row][min_col-1] > min_dist + g[min_row][min_col-1]){
            dist[min_row][min_col-1] = min_dist + g[min_row][min_col-1];
            prev[min_row][min_col-1] = min_row * cols + min_col;
        }
        if(!horizontal_contiguous && min_col < cols - 1 && !visited[min_row][min_col+1] && dist[min_row][min_col+1] > min_dist + g[min_row][min_col+1]){
            dist[min_row][min_col+1] = min_dist + g[min_row][min_col+1];
            prev[min_row][min_col+1] = min_row * cols + min_col;
        }
    }
    return -1;
}

int _next[4][2] = {{0,1},{1,0},{0,-1},{-1,0}};

void DFS(grid& g, int x, int y, int step, int dist, array<bitset<cols>,rows>& visited, vector<int>& path, int& min_dist){
    if(x == rows - 1 && y == cols - 1){
        if(dist < min_dist) min_dist = dist;
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
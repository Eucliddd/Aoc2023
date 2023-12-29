#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <bitset>
#include <stack>
#include <cstring>
#include <queue>

using namespace std;

const int rows = 141;
const int cols = 141;
using grid = array<array<uint8_t,cols>,rows>;
const int max_dist = 1000000;
struct Node
{
    int x;
    int y;
    int dir;
    int len;
    Node():x(-1),y(-1),dir(0),len(0){}
    Node(int x_, int y_, int dir_, int len_):x(x_),y(y_),dir(dir_),len(len_){}
    Node operator=(const Node& nn){
        x = nn.x;
        y = nn.y;
        dir = nn.dir;
        len = nn.len;
        return *this;
    }
};

int _next[4][2] = {{-1,0},{0,-1},{1,0},{0,1}};
int dist[rows][cols][4][4];
bool visited[rows][cols][4][4];
Node _prev[rows][cols][4][4];

int dijkstra(const grid& g, int start_row = 0, int start_col = 0, int end_row = rows - 1, int end_col = cols - 1){
    
    for(int x = 0; x < rows; x++)
    for(int y = 0; y < cols; y++)
    for(int dir = 0; dir < 4; dir++)
    for(int len = 0; len < 4; len++)
        dist[x][y][dir][len] = max_dist;
        
    dist[start_row][start_col][3][0] = 0;
    using Element = pair<int,Node>;
    auto mycmp = [](const Element& a, const Element& b){return a.first > b.first;};
    priority_queue<Element, vector<Element>,decltype(mycmp)> q(mycmp);
    q.push({0,{0,0,3,0}});
    while(!q.empty()){
        int min_dist = q.top().first;
        Node min_node = q.top().second;
        q.pop(); 

        visited[min_node.x][min_node.y][min_node.dir][min_node.len] = 1;
        
        for(int j = 0; j < 4; j++){
            int next_x = min_node.x + _next[j][0];
            int next_y = min_node.y + _next[j][1];
            if(next_x < 0 || next_x >= rows || next_y < 0 || next_y >= cols)
                continue;
            if(abs(j - min_node.dir) == 2)
                continue;
            int new_len = j == min_node.dir ? min_node.len + 1 : 1;
            if(new_len > 3) continue;
            if(!visited[next_x][next_y][j][new_len] && 
                dist[next_x][next_y][j][new_len] > min_dist + g[next_x][next_y]){
                dist[next_x][next_y][j][new_len] = min_dist + g[next_x][next_y];
                _prev[next_x][next_y][j][new_len] = min_node;
                q.push({dist[next_x][next_y][j][new_len],{next_x,next_y,j,new_len}});
            }
        }
    }


    int min = INT_MAX;
    int end_dir;
    int end_len;
    for(int dir = 0; dir < 4; dir++){
        for(int len = 0; len < 4; len++){
            if(dist[end_row][end_col][dir][len] < min){
                min = dist[end_row][end_col][dir][len];
                end_dir = dir;
                end_len = len;
            }
        }
    }

    int r = end_row, c = end_col, d = end_dir, l = end_len;
    array<array<char,cols>,rows> temp;
    for(int ii = 0; ii < rows; ii++)
        for(int jj = 0; jj < cols; jj++)
            temp[ii][jj] = g[ii][jj] + '0';
    char pa[4] = {'^','<','v','>'};
    while(r!=-1&&c!=-1&&l!=0){
        temp[r][c] = pa[d];
        Node next_node = _prev[r][c][d][l];
        r = next_node.x;
        c = next_node.y;
        d = static_cast<int>(next_node.dir);
        l = next_node.len;
    }
    for(int ii = 0; ii < rows; ii++){
        for(int jj = 0; jj < cols; jj++)
            cout << temp[ii][jj];
        cout << endl;
    }
    return min;
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
    return dijkstra(g,0,0,rows-1,cols-1);
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
    file.close();
    cout << puzzle1(g) << endl;
}
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
int dist[rows][cols][4][11];
bool visited[rows][cols][4][11];

int dijkstra(const grid& g, int start_row = 0, int start_col = 0, int end_row = rows - 1, int end_col = cols - 1){
    
    for(int x = 0; x < rows; x++)
    for(int y = 0; y < cols; y++)
    for(int dir = 0; dir < 4; dir++)
    for(int len = 0; len < 11; len++)
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
            if(abs(j - min_node.dir) == 2)
                continue;
            int next_x = min_node.x;
            int next_y = min_node.y;
            int new_len = j == min_node.dir ? min_node.len : 0;
            int cost = 0;
            if(new_len < 4){
                for(int i = 0; i < 4; i++){
                    next_x += _next[j][0];
                    next_y += _next[j][1];
                    new_len++;
                    cost += g[next_x][next_y];
                }
            }
            else{
                next_x += _next[j][0];
                next_y += _next[j][1];
                new_len++;
                cost += g[next_x][next_y];
            }
            if(next_x < 0 || next_x >= rows || next_y < 0 || next_y >= cols)
                continue;
            
            if(new_len > 10) continue;
            if(!visited[next_x][next_y][j][new_len] && 
                dist[next_x][next_y][j][new_len] > min_dist + cost){
                dist[next_x][next_y][j][new_len] = min_dist + cost;
                q.push({dist[next_x][next_y][j][new_len],{next_x,next_y,j,new_len}});
            }
        }
    }


    int min = INT_MAX;
    for(int dir = 0; dir < 4; dir++){
        for(int len = 0; len < 11; len++){
            if(dist[end_row][end_col][dir][len] < min){
                min = dist[end_row][end_col][dir][len];
            }
        }
    }

    return min;
}

int puzzle2(grid& g){
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
    cout << puzzle2(g) << endl;
}
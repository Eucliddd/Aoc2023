#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <thread>
#include <future>
#include <chrono>
#include <mutex>
#include <bitset>
#include <stack>

using namespace std;

const int rows = 141;
const int cols = 141;
int start_x = 0,start_y,end_x = rows-1,end_y;

using grid = array<string,rows>;
grid map;
bool visited[rows][cols];
// 0: up, 1: left, 2: down, 3: right
array<array<int,2>,4> _next = {{{-1,0},{0,-1},{1,0},{0,1}}};


void DFS(int x, int y, int path, int& max_path){
    if(end_x == x && end_y == y){
        if(path > max_path){
            max_path = path;
        }
        return;
    }
    if(map[x][y] != '.'){
        int next_x = x;
        int next_y = y;
        if(map[x][y] == '>') next_y++;
        else if(map[x][y] == '<') next_y--;
        else if(map[x][y] == '^') next_x--;
        else if(map[x][y] == 'v') next_x++;
        if(next_x < 0 || next_x >= rows || next_y < 0 || next_y >= cols){
            return;
        }
        if(map[next_x][next_y] != '#' && !visited[next_x][next_y]){
            visited[next_x][next_y] = true;
            DFS(next_x,next_y,path+1,max_path);
            visited[next_x][next_y] = false;
        }
    }
    else{
        for(int i = 0; i < 4; i++){
            int next_x = x + _next[i][0];
            int next_y = y + _next[i][1];
            if(next_x < 0 || next_x >= rows || next_y < 0 || next_y >= cols){
                continue;
            }
            if(map[next_x][next_y] != '#' && !visited[next_x][next_y]){
                visited[next_x][next_y] = true;
                DFS(next_x,next_y,path+1,max_path);
                visited[next_x][next_y] = false;
            }
        }
    }
    return;
}

int puzzle1(){
    int max_path = 0;
    visited[start_x][start_y] = true;
    DFS(start_x,start_y,0,max_path);
    return max_path;
}

atomic<bool> cancel(false);

void asyncDFS(int x, int y, int ex, int ey,int path, int& max_path, grid& _map, array<bitset<cols>,rows>& _visited){
    static array<array<int,2>,4> _next_ = {{{-1,0},{0,-1},{1,0},{0,1}}};
    if (cancel) {
        return; // 如果设置了取消标志，直接返回
    }

    if (ex == x && ey == y) {
        max_path = max(max_path, path);
        return;
    }

    for (int i = 0; i < 4; i++) {
        int next_x = x + _next_[i][0];
        int next_y = y + _next_[i][1];

        if (next_x >= 0 && next_x < rows && next_y >= 0 && next_y < cols &&
            _map[next_x][next_y] != '#' && !_visited[next_x].test(next_y)) {
            
            _visited[next_x].set(next_y);
            asyncDFS(next_x, next_y, ex, ey, path + 1, max_path, _map, _visited);
            _visited[next_x].reset(next_y);
        }
    }
    return;
}

int puzzle2(){
    int max_path = 0;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(map[i][j] != '.' && map[i][j] != '#'){
                map[i][j] = '.';
            }
        }
    }
    array<bitset<cols>,rows> visited0;
    visited0[start_x].set(start_y);
    auto f = async(launch::async, asyncDFS, start_x, start_y, end_x, end_y, 0, ref(max_path), ref(map), ref(visited0));
    const auto timeout = chrono::seconds(100);
    if(f.wait_for(timeout) == future_status::timeout){
        cout << "Timeout!" << endl;
        cancel = true;
        return max_path;
    }
    return max_path;
}

int main(){
    ifstream input("input.txt");
    string line;
    int row = 0;
    while(getline(input,line)){
        map[row] = line;
        row++;
    }
    input.close();
    for(int i = 0; i < cols; i++){
        if(map[start_x][i] == '.'){
            start_y = i;
            break;
        }
    }
    for (int i = cols-1; i >= 0; i--){
        if(map[end_x][i] == '.'){
            end_y = i;
            break;
        }
    }
    // cout << "Puzzle 1: " << puzzle1() << endl;
    cout << "Puzzle 2: " << puzzle2() << endl;
}
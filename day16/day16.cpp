#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <tuple>
#include <queue>
#include <bitset>
#include <set>
using namespace std;

const int rows = 110;
const int cols = 110;

enum class direction{
    up,
    down,
    left,
    right
};

using light = tuple<int,int,direction>;

int puzzle1(const array<string, rows>& grid, light start = make_tuple(0,0,direction::right)){
    queue<light> q;
    set<light> has_travelled;
    array<bitset<cols>,rows> visited;
    q.push(start);
    has_travelled.insert(start);
    auto insert_light = [&has_travelled, &q](int x, int y, direction d){
        auto t = make_tuple(x,y,d);
        if(has_travelled.find(t) == has_travelled.end()){
            q.push(t);
            has_travelled.insert(t);
        }
    };
    while(!q.empty()){
        int x = get<0>(q.front());
        int y = get<1>(q.front());
        direction d = get<2>(q.front());
        q.pop();
        if(grid[x][y] == '.'){
            if(d == direction::right){
                while(y < cols && grid[x][y] == '.'){
                    visited[x].set(y);
                    y++;
                }
                if(y == cols) continue;
            }
            else if(d == direction::left){
                while(y >= 0 && grid[x][y] == '.'){
                    visited[x].set(y);
                    y--;
                }
                if(y < 0) continue;
            }
            else if(d == direction::up){
                while(x >= 0 && grid[x][y] == '.'){
                    visited[x].set(y);
                    x--;
                }
                if(x < 0) continue;
            }
            else if(d == direction::down){
                while(x < rows && grid[x][y] == '.'){
                    visited[x].set(y);
                    x++;
                }
                if(x == rows) continue;
            }
        }
        if(grid[x][y] == '|'){
            visited[x].set(y);
            if(d == direction::left || d == direction::right){
                if(x-1 >= 0) insert_light(x-1,y,direction::up);
                if(x+1 < rows) insert_light(x+1,y,direction::down);
            }
            else if(d == direction::up && x-1 >= 0) insert_light(x-1,y,direction::up); 
            else if(d == direction::down && x+1 < rows) insert_light(x+1,y,direction::down);
        }
        else if(grid[x][y] == '-'){
            visited[x].set(y);
            if(d == direction::up || d == direction::down){
                if(y-1 >= 0) insert_light(x,y-1,direction::left);
                if(y+1 < cols) insert_light(x,y+1,direction::right);
            }
            else if(d == direction::left && y-1 >= 0) insert_light(x,y-1,direction::left);
            else if(d == direction::right && y+1 < cols) insert_light(x,y+1,direction::right);
        }
        else if(grid[x][y] == '/'){
            visited[x].set(y);
            if(d == direction::right && x-1 >= 0) insert_light(x-1,y,direction::up);
            else if(d == direction::left && x+1 < rows) insert_light(x+1,y,direction::down);
            else if(d == direction::up && y+1 < cols) insert_light(x,y+1,direction::right);
            else if(d == direction::down && y-1 >= 0) insert_light(x,y-1,direction::left);
        }
        else if(grid[x][y] == '\\'){
            visited[x].set(y);
            if(d == direction::right && x+1 < rows) insert_light(x+1,y,direction::down);
            else if(d == direction::left && x-1 >= 0) insert_light(x-1,y,direction::up);
            else if(d == direction::up && y-1 >= 0) insert_light(x,y-1,direction::left);
            else if(d == direction::down && y+1 < cols) insert_light(x,y+1,direction::right);
        }
    }
    // count all the visited cells
    int sum = 0;
    for(int i = 0; i < rows; i++){
        sum += visited[i].count();
    }
    return sum;
}

int puzzle2(const array<string, rows>& grid){
    int max = 0;
    for(int i = 0; i < cols; i++){
        light start = make_tuple(0,i,direction::down);
        max = std::max(max, puzzle1(grid, start));
        start = make_tuple(rows-1,i,direction::up);
        max = std::max(max, puzzle1(grid, start));
    }
    for(int i = 0; i < rows; i++){
        light start = make_tuple(i,0,direction::right);
        max = std::max(max, puzzle1(grid, start));
        start = make_tuple(i,cols-1,direction::left);
        max = std::max(max, puzzle1(grid, start));
    }
    return max;
}

int main(){
    ifstream fin("input.txt");
    string line;
    array<string, rows> grid;
    int i = 0;
    while(getline(fin,line)){
        grid[i++] = line;
    }
    cout << puzzle2(grid) << endl;
    return 0;
}
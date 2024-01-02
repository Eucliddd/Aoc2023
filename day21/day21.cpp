#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;

const int rows = 131;
const int cols = 131;
const int steps = 64;

array<string,rows> grid;

int start_x = rows/2;
int start_y = cols/2;

struct node{
    int x;
    int y;
    int level;
};
// Up, Left, Down, Right
const array<array<int,2>,4> dirs = {{{-1,0},{0,-1},{1,0},{0,1}}};

bool visited[rows][cols][steps+1];

int puzzle1(){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            for(int k = 0; k < steps+1; k++){
                visited[i][j][k] = false;
            }
        }
    }
    queue<node> q;
    node start = {start_x, start_y, 0};
    q.push(start);
    while(!q.empty()){
        node cur = q.front();
        q.pop();
        visited[cur.x][cur.y][cur.level] = true;
        if(cur.level == steps)
            continue;
        for(int i = 0; i < 4; i++){
            int new_x = cur.x + dirs[i][0];
            int new_y = cur.y + dirs[i][1];
            if(new_x < 0 || new_x >= rows || new_y < 0 || new_y >= cols)
                continue;
            if(grid[new_x][new_y] == '#' || visited[new_x][new_y][cur.level+1])
                continue;
            q.push({new_x, new_y, cur.level+1});
            visited[new_x][new_y][cur.level+1] = true;
        }
    }
    int sum = 0;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(visited[i][j][steps]){
                sum++;
                cout << "O";
            }
            else
                cout << grid[i][j];
        }
        cout << endl;
    }
    // cout << endl;
    // int sum2 = 0;
    // for(int i = rows/2-5; i <= rows/2+5; i++){
    //     for(int j = cols/2-5; j <= cols/2+5; j++){
    //         if(visited[i][j][steps]){
    //             sum2++;
    //             cout << "O";
    //         }
    //         else
    //             cout << grid[i][j];
    //     }
    //     cout << endl;
    // }
    // cout << sum2 << endl;

    return sum;
}

uint64_t puzzle2(){
    const uint64_t max_steps = 26501365;
    uint64_t times = ceil((max_steps - rows/2) / rows);
    cout << "times: " << times << endl;
    int odd_area = 0;
    int even_area = 0;
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(grid[i][j] != '#'){
                if((i+j) % 2 == 0)
                    even_area++;
                else
                    odd_area++;
            }
        }
    }
    
    int triangle_area0 = 0;
    for(int i = 0; i <= rows/2; i++){
        for(int j = 0; j <= cols/2-i; j++){
            if(grid[i][j] != '#')
                if((i+j) % 2){
                    triangle_area0++;
                }
        }
    }
    int triangle_area1 = 0;
    for(int i = 0; i <= rows/2; i++){
        for(int j = cols/2+i; j < cols; j++){
            if(grid[i][j] != '#')
                if((i+j) % 2){
                    triangle_area1++;
                }
        }
    }
    int triangle_area2 = 0;
    for(int i = rows/2; i < rows; i++){
        for(int j = 0; j <= i - rows/2; j++){
            if(grid[i][j] != '#')
                if((i+j) % 2){
                    triangle_area2++;
                }
        }
    }
    int triangle_area3 = 0;
    for(int i = rows/2; i < rows; i++){
        for(int j = 3*(rows/2)-i; j < cols; j++){
            if(grid[i][j] != '#')
                if((i+j) % 2){
                    triangle_area3++;
                }
        }
    }

    int diagonal_area0 = 0;
    for(int i = 0; i <= rows/2; i++){
        if(grid[i][rows/2-i]!='#')
                diagonal_area0++;
    }
    int diagonal_area1 = 0;
    for(int i = 0; i <= rows/2; i++){
        if(grid[i][rows/2+i]!='#')
                diagonal_area1++;
    }
    int diagonal_area2 = 0;
    for(int i = rows/2; i < rows; i++){
        if(grid[i][i-rows/2]!='#')
                diagonal_area2++;
    }
    int diagonal_area3 = 0;
    for(int i = rows/2; i < rows; i++){
        if(grid[i][3*(rows/2)-i]!='#')
                diagonal_area3++;
    }

    int pentagon_area0 = odd_area - triangle_area0 + diagonal_area0;
    int pentagon_area1 = odd_area - triangle_area1 + diagonal_area1;
    int pentagon_area2 = odd_area - triangle_area2 + diagonal_area2;
    int pentagon_area3 = odd_area - triangle_area3 + diagonal_area3;

    int pentagon_areaU = odd_area - triangle_area0 - triangle_area1 + diagonal_area0 + diagonal_area1;
    int pentagon_areaD = odd_area - triangle_area2 - triangle_area3 + diagonal_area2 + diagonal_area3;
    int pentagon_areaL = odd_area - triangle_area0 - triangle_area2 + diagonal_area0 + diagonal_area2;
    int pentagon_areaR = odd_area - triangle_area1 - triangle_area3 + diagonal_area1 + diagonal_area3;

    cout << "even_area: " << even_area << endl;
    cout << "odd_area: " << odd_area << endl;
    cout << "triangle_area0: " << triangle_area0 << endl;
    cout << "triangle_area1: " << triangle_area1 << endl;
    cout << "triangle_area2: " << triangle_area2 << endl;
    cout << "triangle_area3: " << triangle_area3 << endl;
    cout << "pentagon_area0: " << pentagon_area0 << endl;
    cout << "pentagon_area1: " << pentagon_area1 << endl;
    cout << "pentagon_area2: " << pentagon_area2 << endl;
    cout << "pentagon_area3: " << pentagon_area3 << endl;
    cout << "pentagon_areaU: " << pentagon_areaU << endl;
    cout << "pentagon_areaD: " << pentagon_areaD << endl;
    cout << "pentagon_areaL: " << pentagon_areaL << endl;
    cout << "pentagon_areaR: " << pentagon_areaR << endl;

    uint64_t sum = pentagon_areaU + pentagon_areaD + pentagon_areaL + pentagon_areaR;
    sum += times*times*even_area + (times-1)*(times-1)*odd_area;
    sum += (times-1)*(pentagon_area0 + pentagon_area1 + pentagon_area2 + pentagon_area3);
    sum += times*(triangle_area0 + triangle_area1 + triangle_area2 + triangle_area3);

    return sum;
}

void test(){
    ofstream file("input.txt");
    for(int i = 0;i < rows*7; i++){
        for(int j = 0; j < cols*7; j++){
            if (grid[i%rows][j%cols] == 'S' && !(i == rows*7/2 && j == cols*7/2))
                file << '.';
            else
                file << grid[i%rows][j%cols];
        }
        file << endl;
    }
    file.close();
}

int main(){
    ifstream file("input.txt");
    string line;
    int row = 0;
    while(getline(file, line)){
        grid[row] = line;
        row++;
    }
    file.close();
    
    // test();
    cout << puzzle2() << endl;
    return 0;
}
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <queue>
#include <set>

using namespace std;

const int nums = 1456;

struct brick{
    int xl,xr;
    int yl,yr;
    int zd,zu;
    bool operator<(const brick& rhs) const{
        return zd > rhs.zd;
    }
};

priority_queue<brick> bricks;

int puzzle1(){
    int count = 0;
    vector<set<int>> supported;
    int grid[10][10][372];
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            for(int k=0;k<372;k++){
                grid[i][j][k] = -1;
            }
        }
    }
    int index = 0;
    while(!bricks.empty()){
        brick b = bricks.top();
        bricks.pop();
        int z = b.zd;
        int zz = b.zu;
        bool end = false;
        set<int> support;
        while(z > 1){
            for(int i=b.xl;i<=b.xr;i++){
                for(int j=b.yl;j<=b.yr;j++){
                    if(grid[i][j][z-1]!=-1){
                        end = true;
                        support.insert(grid[i][j][z-1]);
                    }
                }
            }
            if(end){
                break;
            }
            z--;
            zz--;
        }
        for(int i=b.xl;i<=b.xr;i++){
            for(int j=b.yl;j<=b.yr;j++){
                for(int k = z; k <= zz; k++){
                    grid[i][j][k] = index;
                }
            }
        }
        supported.push_back(support);
        index++;
    }
    for(int i=0;i<supported.size();i++){
        cout <<  i << ": ";
        for(auto e:supported[i]){
            cout << e << ",";
        }
        cout << endl;
    }
    set<int> exclude;
    for(int i=0;i<supported.size();i++){
        if(supported[i].size()==1){
            exclude.insert(*supported[i].begin());
        }
    }
    return supported.size()-exclude.size();
}

int puzzle2(){
   int count = 0;
    vector<set<int>> supported;
    vector<set<int>> supporting(nums);
    int grid[10][10][372];
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            for(int k=0;k<372;k++){
                grid[i][j][k] = -1;
            }
        }
    }
    int index = 0;
    while(!bricks.empty()){
        brick b = bricks.top();
        bricks.pop();
        int z = b.zd;
        int zz = b.zu;
        bool end = false;
        set<int> support;
        while(z > 1){
            for(int i=b.xl;i<=b.xr;i++){
                for(int j=b.yl;j<=b.yr;j++){
                    if(grid[i][j][z-1]!=-1){
                        end = true;
                        support.insert(grid[i][j][z-1]);
                        supporting[grid[i][j][z-1]].insert(index);
                    }
                }
            }
            if(end){
                break;
            }
            z--;
            zz--;
        }
        for(int i=b.xl;i<=b.xr;i++){
            for(int j=b.yl;j<=b.yr;j++){
                for(int k = z; k <= zz; k++){
                    grid[i][j][k] = index;
                }
            }
        }
        supported.push_back(support);
        index++;
    }

    // cout << "supporting" << endl;
    // for(int i = 0; i < nums ; i++){
    //     cout << (char)(i + 'A') << ": ";
    //     for(auto e:supporting[i]){
    //         cout << (char)(e + 'A') << ",";
    //     }
    //     cout << endl;
    // }

    // cout << "supported" << endl;
    // for(int i=0;i<supported.size();i++){
    //     cout << (char)(i + 'A') << ": ";
    //     for(auto e:supported[i]){
    //         cout << (char)(e + 'A') << ",";
    //     }
    //     cout << endl;
    // }

    set<int> exclude;
    for(int i=0;i<supported.size();i++){
        if(supported[i].size()==1){
            exclude.insert(*supported[i].begin());
        }
        else if(supported[i].size() == 0)
            supported[i].insert(-1);
    }

    for(auto e:exclude){
        queue<int> q;
        q.push(e);
        auto temp = supported;
        while(!q.empty()){
            int i = q.front();
            q.pop();
            for(auto f:supporting[i]){
                if(temp[f].size() > 0){
                    temp[f].erase(i);
                    if(temp[f].size() == 0){
                        q.push(f);
                        count++;
                    }
                }
            }
        }
    }
    return count;
}

int main(){
    ifstream infile("input.txt");
    string line;
    while(getline(infile,line)){
        int xl,xr,yl,yr,zd,zu;
        sscanf(line.c_str(),"%d,%d,%d~%d,%d,%d",&xl,&yl,&zd,&xr,&yr,&zu);
        bricks.push({xl,xr,yl,yr,zd,zu});
    }
    cout << puzzle2() << endl;
}


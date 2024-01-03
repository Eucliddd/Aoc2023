#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <set>

using namespace std;

const uint64_t _left = 200000000000000, _right = 400000000000000;

// y = ax + b
struct line {
    double a,b;
    uint64_t x,y;
    int vx,vy;
};

struct point {
    long double x,y;
    bool operator<(const point& p) const {
        if(x < p.x){
            return true;
        }
        if(x == p.x){
            return y < p.y;
        }
        return false;
    }
};

int puzzle1(vector<line>& lines){
    set<point> points;
    for(int i = 0; i < lines.size() - 1; i++){
        for(int j = i + 1; j < lines.size(); j++){
            if(lines[i].a == lines[j].a){
                continue;
            }
            line l1 = lines[i], l2 = lines[j];
            long double x = (l2.b - l1.b)/(l1.a - l2.a);
            long double y = l1.a*x + l1.b;
            if(x >= _left && x <= _right && y >= _left && y <= _right){
                if((l1.vx > 0 && x < l1.x) || (l1.vx < 0 && x > l1.x) || (l1.vy > 0 && y < l1.y) || (l1.vy < 0 && y > l1.y)){
                    continue;
                }
                if((l2.vx > 0 && x < l2.x) || (l2.vx < 0 && x > l2.x) || (l2.vy > 0 && y < l2.y) || (l2.vy < 0 && y > l2.y)){
                    continue;
                }
                point p;
                p.x = x;
                p.y = y;
                points.insert(p);
            }
        }
    }
    return points.size();
}

int main(){
    ifstream file("input.txt");
    string str;
    vector<line> lines;
    while(getline(file, str)){
        uint64_t x,y,z;
        int vx,vy,vz;
        sscanf(str.c_str(),"%lld, %lld, %lld @ %d, %d, %d",&x,&y,&z,&vx,&vy,&vz);
        line l;
        l.x = x;
        l.y = y;
        l.vx = vx;
        l.vy = vy;
        l.a = (double)vy/(double)vx;
        l.b = y - l.a*x;
        lines.push_back(l);
    }
    cout << puzzle1(lines) << endl;
}
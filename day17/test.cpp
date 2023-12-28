#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>

using namespace std;

const int rows = 13;
const int cols = 13;
using grid = array<array<uint16_t,cols>,rows>;

int main(){
    ifstream file("input.txt");
    string line;
    grid g1,g2;
    int row = 0;
    while(getline(file, line)){
        for(int col = 0; col < line.size(); col++){
            g1[row][col] = line[col] - '0';
        }
        row++;
    }
    file.close();
    file.open("input2.txt");
    row = 0;
    while(getline(file, line)){
        for(int col = 0; col < line.size(); col++){
            g2[row][col] = line[col] - '0';
        }
        row++;
    }
    file.close();
    int sum = 0;
    for(int row = 0; row < rows; row++){
        for(int col = 0; col < cols; col++){
            if(g1[row][col] != g2[row][col])
                sum += g1[row][col];
        }
    }
    cout << sum << endl;
}
    
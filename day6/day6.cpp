#include <fstream>
#include <string>
#include <array>
#include <sstream>
#include <iostream>
#include <cmath>

using namespace std;

const int race_num = 4;

int puzzle1(){
    array<int,race_num> times;
    array<int,race_num> distances;
    ifstream infile("input.txt");
    string line;
    getline(infile, line);
    stringstream ss(line.substr(line.find(":")+1));
    for(int i = 0; i < race_num; i++){
        ss >> times[i];
    }
    getline(infile, line);
    stringstream ss2(line.substr(line.find(":")+1));
    for(int i = 0; i < race_num; i++){
        ss2 >> distances[i];
    }
    int multiply = 1;
    for (int i=0; i<race_num; i++){
        double half_time = times[i]/2.0;
        double root = sqrt(half_time*half_time-distances[i]);
        double left = half_time-root+1e-9;
        double right = half_time+root-1e-9;
        int left_int = ceil(left);
        int right_int = floor(right);
        int count = right_int-left_int+1;
        cout << left << " " << right << " " << count << endl;
        multiply *= count;
    }
    return multiply;
}

int puzzle2(){
    uint64_t total_time = 0;
    uint64_t total_distance = 0;
    ifstream infile("input.txt");
    string line;
    getline(infile, line);
    stringstream ss(line.substr(line.find(":")+1));
    // concatenate each number to a string
    string times="";
    int num;
    while(ss >> num){
        times.append(to_string(num));
    }
    getline(infile, line);
    stringstream ss2(line.substr(line.find(":")+1));
    string distances="";
    while(ss2 >> num){
        distances.append(to_string(num));
    }
    total_time = stoull(times);
    total_distance = stoull(distances);
    cout << total_time << " " << total_distance << endl;
    double half_time = total_time/2.0;
    double root = sqrt(half_time*half_time-total_distance);
    double left = half_time-root;
    double right = half_time+root;
    int left_int = floor(left)+1;
    int right_int = ceil(right)-1;
    int count = right_int-left_int+1;
    return count;
}

int main(){

    cout << puzzle2() << endl;
}

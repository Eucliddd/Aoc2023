#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

long long puzzle1() {
    ifstream file("input.txt");
    string line;
    long long result = 0;
    while(getline(file, line)) {
        vector<long long> numbers;
        stringstream ss(line);
        long long number;
        while(ss >> number) {
            numbers.push_back(number);
        }
        vector<vector<long long>> diffs;
        vector<long long> curr = numbers;
        while(true) {
            vector<long long> diff;
            bool found = true;
            for(int i = 0; i < curr.size() - 1; i++) {
                long long n = curr[i + 1] - curr[i];
                if(n!=0) found = false;
                diff.push_back(n);
            }
            diffs.push_back(diff);
            if(found) {
                break;
            }
            curr = diff;
        }
        long long next = numbers[numbers.size() - 1];
        for(int i = diffs.size() - 1; i >= 0; i--) {
            next += diffs[i][diffs[i].size() - 1];
        }
        cout << next << endl;
        result += next;
    }
    return result;
}

int puzzle2() {
    ifstream file("input.txt");
    string line;
    long long result = 0;
    while(getline(file, line)) {
        vector<long long> numbers;
        stringstream ss(line);
        long long number;
        while(ss >> number) {
            numbers.push_back(number);
        }
        vector<vector<long long>> diffs;
        vector<long long> curr = numbers;
        while(true) {
            vector<long long> diff;
            bool found = true;
            for(int i = 0; i < curr.size() - 1; i++) {
                long long n = curr[i + 1] - curr[i];
                if(n!=0) found = false;
                diff.push_back(n);
            }
            diffs.push_back(diff);
            if(found) {
                break;
            }
            curr = diff;
        }
        long long next = 0;
        for(int i = diffs.size() - 2; i >= 0; i--) {
            next = diffs[i][0] - next;
        }
        next = numbers[0] - next;
        cout << next << endl;
        result += next;
    }
    return result;
}

int main() {
    cout << puzzle2() << endl;
}
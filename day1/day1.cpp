#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include <unordered_map>
using namespace std;


int main() {
    ifstream file("input1.txt");
    string line;
    unordered_map<string,int> wordToNum = {
        {"one",1}, {"two",2}, {"three",3}, {"four",4}, {"five",5}, {"six",6}, {"seven",7}, {"eight",8}, {"nine",9},
        {"eno",1}, {"owt",2}, {"eerht",3}, {"ruof",4}, {"evif",5}, {"xis",6}, {"neves",7}, {"thgie",8}, {"enin",9}
    };
    regex digitRegex("\\d|one|two|three|four|five|six|seven|eight|nine");
    regex reverseDigitRegex("\\d|eno|owt|eerht|ruof|evif|xis|neves|thgie|enin");
    int sum = 0;
    while (getline(file, line)) {
        int firstNum = 0;
        auto firstNumIt = sregex_iterator(line.begin(), line.end(), digitRegex);
        if(isdigit(firstNumIt->str()[0])) {
            firstNum = stoi(firstNumIt->str());
        }
        else {
            firstNum = wordToNum[firstNumIt->str()];
        }
        int lastNum = 0;
        reverse(line.begin(), line.end());
        auto lastNumIt = sregex_iterator(line.begin(), line.end(), reverseDigitRegex);
        if(isdigit(lastNumIt->str()[0])) {
            lastNum = stoi(lastNumIt->str());
        }
        else {
            lastNum = wordToNum[lastNumIt->str()];
        }
        int num = firstNum * 10 + lastNum;
        sum += num;
    }
    cout << sum << endl;
}
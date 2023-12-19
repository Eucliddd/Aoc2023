#include <string>
#include <iostream>
#include <fstream>
#include <regex>
using namespace std;

// enum class color: int{
//     red = 12,
//     green = 13,
//     blue = 14
// };

int main(){
    ifstream file("input.txt");
    string line;
    int sum = 0;
    int index = 0;
    regex numberRegex("\\d+");
    smatch match;
    while(getline(file, line)){
        ++index;
        int maxRed = 0, maxGreen = 0, maxBlue = 0;
        auto it = sregex_iterator(line.begin() + 8, line.end(), numberRegex);
        while (it != sregex_iterator()) {
            match = *it;
            int num = stoi(match.str());
            int position = match.position() + match.length() + 9;
            if(line[position] == 'r' && num > maxRed) {
                maxRed = num;
            }
            else if(line[position] == 'g' && num > maxGreen) {
                maxGreen = num;
            }
            else if(line[position] == 'b' && num > maxBlue) {
                maxBlue = num;
            }
            ++it;
        }
        sum += maxRed * maxGreen * maxBlue;
    }
    cout << sum << endl;
}
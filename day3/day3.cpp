#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <regex>
#include <cmath>

using namespace std;

const int dim = 140;

inline bool isSymbol(char c){
    return c!='.' && !isdigit(c);
}


int puzzle1(const array<string,dim>& map){
    int sum = 0;
    regex digitRegex("[0-9]+");
    for(int k = 0; k < dim; k++){
        auto& e = map[k];
        auto words_begin = sregex_iterator(e.begin(), e.end(), digitRegex);
        auto words_end = sregex_iterator();
        for(auto i = words_begin; i != words_end; i++){
            smatch match = *i;
            int length = match.length();
            int position = match.position();
            string num = match.str();
            for(int j = 0; j < length; j++){
                if(j == 0){
                    if((position>1 && isSymbol(map[k][position-1]))
                     ||(position>1 && k>1 && isSymbol(map[k-1][position-1]))
                     ||(position>1 && k+1<dim && isSymbol(map[k+1][position-1]))){
                        sum += stoi(num);
                        cout << num << endl;
                        break;
                    }
                }
                if(j == length-1){
                    if((position+j+1<dim && isSymbol(map[k][position+j+1]))
                     ||(position+j+1<dim && k>1 && isSymbol(map[k-1][position+j+1]))
                     ||(position+j+1<dim && k+1<dim && isSymbol(map[k+1][position+j+1]))){
                        sum += stoi(num);
                        cout << num << endl;
                        break;
                    }
                }
                if((k>1 && isSymbol(map[k-1][position+j]))
                    ||(k+1<dim && isSymbol(map[k+1][position+j]))){
                    sum += stoi(num);
                    cout << num << endl;
                    break;
                }
            }
        }
    }
    return sum;
}

int puzzle2(const array<string,dim>& map){
    int sum = 0;
    // match '*'
    regex gearRegex("\\*");
    for(int k = 0; k < dim; k++){
        auto& e = map[k];
        auto words_begin = sregex_iterator(e.begin(), e.end(), gearRegex);
        auto words_end = sregex_iterator();
        for(auto i = words_begin; i != words_end; i++){
            smatch match = *i;
            int count = 0;
            int position = match.position();
            int gear_ratio = 1;
            // left
            if(position > 1 && isdigit(map[k][position-1])){
                ++count;
                int p = position - 1;
                int num = 0;
                int times = 1;
                while(p >= 0 && isdigit(map[k][p])){
                    num += (map[k][p]-'0')*times;
                    times *= 10;
                    --p;
                }
                cout << "left: " << num << endl;
                gear_ratio *= num;
            }
            // right
            if(position + 1 < dim && isdigit(map[k][position+1])){
                ++count;
                int p = position + 1;
                int num = 0;
                while(p < dim && isdigit(map[k][p])){
                    num = num * 10 + (map[k][p]-'0');
                    ++p;
                }
                cout << "right: " << num << endl;
                gear_ratio *= num;
            }
            // up
            if(k > 0){
                int p = position - 1;
                int upper_num = 0;
                int timer = 1;
                while(p >= 0 && isdigit(map[k-1][p])){
                    upper_num += (map[k-1][p]-'0')*timer;
                    timer *= 10;
                    --p;
                }
                bool flag = false;
                if(isdigit(map[k-1][position])){
                    upper_num = upper_num*10 + (map[k-1][position]-'0');
                    flag = true;
                }
                int upper_num2 = 0;
                p = position + 1;
                while(p < dim && isdigit(map[k-1][p])){
                    upper_num2 = upper_num2*10 + (map[k-1][p]-'0');
                    ++p;
                }
                if(flag){
                    upper_num = upper_num*pow(10, p - position - 1) + upper_num2;
                    cout << "up: " << upper_num << endl;
                    gear_ratio *= upper_num;
                    count++;
                }
                else {
                    if(upper_num != 0 && upper_num2 != 0){
                        cout << "up: " << upper_num << " " << upper_num2 << endl;
                        gear_ratio *= upper_num * upper_num2;
                        count += 2;
                    }
                    else if(upper_num != 0){
                        cout << "up: " << upper_num << endl;
                        gear_ratio *= upper_num;
                        count++;
                    }
                    else if(upper_num2 != 0){
                        cout << "up: " << upper_num2 << endl;
                        gear_ratio *= upper_num2;
                        count++;
                    }
                }
            }
            // down
            if(k + 1 < dim){
                int p = position - 1;
                int down_num = 0;
                int timer = 1;
                while(p >= 0 && isdigit(map[k+1][p])){
                    down_num += (map[k+1][p]-'0')*timer;
                    timer *= 10;
                    --p;
                }
                bool flag = false;
                if(isdigit(map[k+1][position])){
                    down_num = down_num*10 + (map[k+1][position]-'0');
                    flag = true;
                }
                int down_num2 = 0;
                p = position + 1;
                while(p < dim && isdigit(map[k+1][p])){
                    down_num2 = down_num2*10 + (map[k+1][p]-'0');
                    ++p;
                }
                if(flag){
                    down_num = down_num*pow(10, p - position - 1) + down_num2;
                    cout << "down: " << down_num << endl;
                    gear_ratio *= down_num;
                    count++;
                }
                else {
                    if(down_num != 0 && down_num2 != 0){
                        cout << "down: " << down_num << " " << down_num2 << endl;
                        gear_ratio *= down_num * down_num2;
                        count += 2;
                    }
                    else if(down_num != 0){
                        cout << "down: " << down_num << endl;
                        gear_ratio *= down_num;
                        count++;
                    }
                    else if(down_num2 != 0){
                        cout << "down: " << down_num2 << endl;
                        gear_ratio *= down_num2;
                        count++;
                    }
                }
            }
            if(count == 2) sum += gear_ratio;
        }
    }
    return sum;
}

int main(){
    ifstream file("input.txt");
    string line;
    array<string,dim> map;
    for(int i = 0; i < dim; i++){
        getline(file, line);
        map[i] = line;
    }

    cout << puzzle2(map) << endl;
}
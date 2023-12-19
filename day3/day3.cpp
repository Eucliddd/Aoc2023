#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <regex>

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
    regex gearRegex("\*");
    for(int k = 0; k < dim; k++){
        auto& e = map[k];
        auto words_begin = sregex_iterator(e.begin(), e.end(), gearRegex);
        auto words_end = sregex_iterator();
        for(auto i = words_begin; i != words_end; i++){
            smatch match = *i;
            
        }
    }
}

int main(){
    ifstream file("input.txt");
    string line;
    array<string,dim> map;
    for(int i = 0; i < dim; i++){
        getline(file, line);
        map[i] = line;
    }

    cout << puzzle1(map) << endl;
}
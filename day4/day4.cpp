#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <array>

using namespace std;

const int winCardNum = 10;
const int cardNum = 25;
const int rows = 215;
using WinnigCards = array<int,winCardNum>;
using Cards = array<int,cardNum>;


template<typename T>
int partition(T arr[],int low, int high){
    T pivot = arr[high - 1];
    int i = low;
    int j = high - 1;
    while(i < j){
        while(i < j && arr[i]<pivot) i++;
        arr[j] = arr[i];
        while(i < j && arr[j]>pivot) j--;
        arr[i] = arr[j];
    }
    arr[i] = pivot;
    return i;
}

template<typename T>
void quick_sort(T arr[], int low, int high){
    if(low < high){
        int pivot = partition(arr, low, high);
        quick_sort(arr, low, pivot);
        quick_sort(arr, pivot+1, high);
    }
}

int puzzle1(){
    ifstream file("input.txt");
    string line;
    int sum = 0;
    while(getline(file, line)){
        WinnigCards winningCards;
        Cards cards;
        line.erase(0, line.find(':') + 2);
        istringstream iss(line);
        for (int i = 0; i < winCardNum; i++){
            iss >> winningCards[i];
        }
        line.erase(0, line.find('|') + 2);
        iss.str(line);
        for (int i = 0; i < cardNum; i++){
            iss >> cards[i];
        }
        quick_sort(winningCards.data(), 0, winCardNum);
        quick_sort(cards.data(), 0, cardNum);
        int i = 0, j = 0;
        int points = 0;
        while(i < winCardNum && j < cardNum){
            if(winningCards[i] == cards[j]){
                points = points == 0 ? 1 : points << 1;
                i++;
                j++;
            }
            else if(winningCards[i] < cards[j]){
                i++;
            }
            else{
                j++;
            }
        }
        sum += points;
    }
    return sum;
}

int puzzle2(){
   ifstream file("input.txt");
    string line;
    int sum = 0;
    array<int,rows> copies;
    copies.fill(1);
    int index = 0;
    while(getline(file, line)){
        WinnigCards winningCards;
        Cards cards;
        line.erase(0, line.find(':') + 2);
        istringstream iss(line);
        for (int i = 0; i < winCardNum; i++){
            iss >> winningCards[i];
        }
        line.erase(0, line.find('|') + 2);
        iss.str(line);
        for (int i = 0; i < cardNum; i++){
            iss >> cards[i];
        }
        quick_sort(winningCards.data(), 0, winCardNum);
        quick_sort(cards.data(), 0, cardNum);
        int i = 0, j = 0;
        int nums = 0;
        while(i < winCardNum && j < cardNum){
            if(winningCards[i] == cards[j]){
                nums++;
                i++;
                j++;
            }
            else if(winningCards[i] < cards[j]){
                i++;
            }
            else{
                j++;
            }
        }
        for(int i = 1; i < nums + 1; i++){
            copies[index + i] += copies[index];
        }
        index++;
    }
    for(auto n : copies){
        sum += n;
    }
    return sum;
}

int main(){
    cout << puzzle2() << endl;
}

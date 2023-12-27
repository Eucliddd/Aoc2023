#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>

using namespace std;


int64_t puzzle1(string& springs, const vector<int>& contiguous_group){
    springs.insert(0, ".");
    auto n = springs.size();
    auto m = contiguous_group.size();
    vector<vector<int64_t>> dp(n+1, vector<int64_t>(m+1, 0));
    dp[0][0] = 1;
    for (int i = 1; i <= m; i++) dp[0][i] = 0;
    bool flag = false;
    for (int i = 1; i <= n; i++) {
        if (springs[i-1] == '#') flag = true;
        if (flag) dp[i][0] = 0;
        else dp[i][0] = 1;
    }
    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= m; j++){
            if(springs[i-1] == '.') dp[i][j] = dp[i-1][j];
            else if(springs[i-1] == '#'){
                int k = i-1;
                while(k > 0 && springs[k] != '.') k--;
                int temp = i - 1 - k; 
                if (temp >= contiguous_group[j-1]){
                    int s = i - 1 - contiguous_group[j-1];
                    if (springs[s] != '#')
                        dp[i][j] = dp[s][j-1];
                    else dp[i][j] = 0;
                } 
                else dp[i][j] = 0;
            }
            else{
                dp[i][j] = dp[i-1][j];
                int k = i-1;
                while(k > 0 && springs[k] != '.') k--;
                int temp = i - 1 - k; 
                if (temp >= contiguous_group[j-1]) {
                    int s = i - 1 - contiguous_group[j-1];
                    if (springs[s] != '#') dp[i][j] += dp[s][j-1];
                } 
            }
        }
    }
    // cout << dp[n][m] << endl;
    return dp[n][m];
}

int64_t puzzle2(const string& springs, const vector<int>& contiguous_group){
    string s = springs;
    vector<int> cg = contiguous_group;
    for(int i = 0; i < 4; i++){
        s.append("?"+springs);
        for(auto e: contiguous_group) cg.push_back(e);
    }
    return puzzle1(s, cg);
}

int main(){
    ifstream input("input.txt");
    string line;
    int64_t result = 0;
    int lines = 0;
    while(getline(input, line)){
        string springs = line.substr(0, line.find(' '));
        // springs.insert(0, ".");
        vector<int> contiguous_group;
        line.erase(0, line.find(' ')+1);
        while(line.find(',') != string::npos){
            contiguous_group.push_back(stoi(line.substr(0, line.find(','))));
            line.erase(0, line.find(',')+1);
        }
        contiguous_group.push_back(stoi(line));
        lines++;
        result += puzzle2(springs, contiguous_group);
    }
    cout << result << endl;
    return 0;
}
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <bitset>
#include <map>
#include <cmath>

using namespace std;

const int N = 1443;
struct graph {
    array<array<int,N>,N> edges{};
    vector<string> nodes;
    map<string,int> node_ids;
    bitset<N> combined;
    vector<int> connected;

    int STCut(int& s, int& t){
        bitset<N> visited;
        visited.reset();
        array<int,N> dist{};
        int mincut = 0;
        for(int i = 0; i < nodes.size(); i++){
            int k = -1;
            int maxc = -1;
            for(int j = 0; j < nodes.size(); j++){
                if(!combined[j] && !visited[j] && dist[j] > maxc){
                    maxc = dist[j];
                    k = j;
                }
            }
            if(k == -1) 
                return mincut;
            s = t;
            t = k;
            mincut = maxc;
            visited[k] = true;
            for(int j = 0; j < nodes.size(); j++){
                if(!combined[j] && !visited[j] && edges[k][j] > 0){
                    dist[j] += edges[k][j];
                }
            }
        }
        return mincut;
    }

    int Stoer_Wagner(){
        int mincut = INT32_MAX;
        for(int i = 0; i < nodes.size() - 1; i++){
            int s = -1, t = -1;
            int cut = STCut(s, t);
            combined[t] = true;
            mincut = min(mincut, cut);
            for(int j = 0; j < nodes.size(); j++){
                if(!combined[j] && edges[j][t] > 0){
                    edges[j][s] += edges[j][t];
                    edges[s][j] += edges[t][j];
                }
            }
        }
        return mincut;
    }

    void merge_nodes(int u, int v){
        for(int i = 0; i < nodes.size(); i++){
            if(combined[i]){
                continue;
            }
            edges[u][i] += edges[v][i];
            edges[v][i] = 0;
            edges[i][u] += edges[i][v];
            edges[i][v] = 0;
        }
        combined[v] = true;
        connected[u] += connected[v];
    }

    void remove_self_loops(){
        for(int i = 0; i < nodes.size(); i++){
            edges[i][i] = 0;
        }
    }

    int Karger(){
        remove_self_loops();
        while(nodes.size() - combined.count() > 2){
            int u = -1, v = -1;
            do{
                u = rand() % nodes.size();
                v = rand() % nodes.size();
            }while(combined[u] || combined[v] || edges[u][v] == 0 || u == v);
            merge_nodes(u, v);
            remove_self_loops();
        }
        int u = -1, v = -1;
        for(int i = 0; i < nodes.size(); i++){
            if(!combined[i]){
                if(u == -1){
                    u = i;
                }
                else{
                    v = i;
                }
            }
        }
        return edges[u][v];
    }
};

graph g;
graph g_copy;

int puzzle1(){
    int n = g.nodes.size();
    double ln = log(double(n));
    double runs = n*n*ln;
    int mincut = INT32_MAX;
    for(long i = 0; i < runs; i++){
        g_copy = g;
        g_copy.combined.reset();
        int cut = g_copy.Karger();
        mincut = min(mincut, cut);
        if(i % 1000 == 0)
            cout << i << " " << mincut << endl;
        if(mincut == 3)
            break;
    }
    int u = -1, v = -1;
    for(int i = 0; i < n; i++){
        if(!g_copy.combined[i]){
            if(u == -1){
                u = i;
            }
            else{
                v = i;
            }
        }
    }
    cout << g_copy.nodes[u] << " " << g_copy.nodes[v] << endl;
    cout << g_copy.connected[u] << " " << g_copy.connected[v] << endl;
    return g_copy.connected[u] * g_copy.connected[v]; 
}

int main(){
    g.combined.reset();
    ifstream input("input.txt");
    string line;
    while(getline(input, line)){
        string node = line.substr(0, line.find(':'));
        line.erase(0, line.find(':') + 2);
        int node_id = -1;
        if(g.node_ids.find(node) == g.node_ids.end()){
            node_id = g.nodes.size();
            g.node_ids[node] = node_id;
            g.nodes.push_back(node);
        }
        else{
            node_id = g.node_ids[node];
        }
        stringstream ss(line);
        string neighbor;
        while(ss >> neighbor){
            int neighbor_id = -1;
            if(g.node_ids.find(neighbor) == g.node_ids.end()){
                neighbor_id = g.nodes.size();
                g.node_ids[neighbor] = neighbor_id;
                g.nodes.push_back(neighbor);
            }
            else{
                neighbor_id = g.node_ids[neighbor];
            }
            g.edges[node_id][neighbor_id] = 1;
            g.edges[neighbor_id][node_id] = 1;
        }
    }
    cout << g.nodes.size() << endl;
    g.connected.resize(g.nodes.size(), 1);
    cout << puzzle1() << endl;

    return 0;
}
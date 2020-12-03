#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

class Graph{
public:
    vector<int> nodeColor; // -1 means this node is already contracted with others
    vector<vector<int>> neighbours; // adjacency list
    int n; // number of nodes, won't change after creation
    int cost;
    vector<pair<int, int>> path;
    Graph(): n(0), cost(0){}
    Graph(int n): n(n), cost(0){
        nodeColor = vector<int>(n, -1);
        neighbours = vector<vector<int>>(n);
    }

    void printGraph(){
        cout << "Graph:" << endl;
        cout << "Number of nodes: " << n << endl;
        cout << "Color of each node: ";
        for(int i = 0; i < n; i++){
            cout << "node " << i << ": " << nodeColor[i] << "; ";
        }
        cout << endl;
        cout << "Neighbour of each node: ";
        for(int i = 0; i < n; i++){
            cout << "node " << i << ": " << "[";
            for(int j : neighbours[i]){
                cout << j << " ";
            }
            cout << "]; ";
        }
        cout << endl;
        cout << "Cost: " << cost << endl;
        // path
    }
};

vector<string> splitStr(const string& str, char c){
    int prev = 0;
    vector<string> res;
    for(int i = 0; i <= str.size(); i++){
        if(i == str.size() || str[i] == c){
            res.push_back(str.substr(prev, i - prev));
            prev = i + 1;
        }
    }
    return res;
}

void readTestCaseFromFile(const string& fileName, Graph& graph, int& n, int& stepCnt, int& colorCnt){
    ifstream file(fileName);
    string str;
    getline(file, str);
    n = stoi(str);
    getline(file, str);
    stepCnt = stoi(str);
    getline(file, str);
    colorCnt = stoi(str);
    graph = Graph(n);
    for(int i = 0; i < n; i++){
        getline(file, str);
        graph.nodeColor[i] = stoi(str);
    }
    for(int i = 0; i < n; i++){
        getline(file, str);
        vector<string> splits = splitStr(str, ' ');
        for(const string& idStr : splits){
            graph.neighbours[i].push_back(stoi(idStr));
        }
    }
}


vector<pair<int, int>> solve(Graph* graph, int limit){
    return vector<pair<int, int>>();
}
int main(){
    cout << "Hello CSE 202" << endl;
    Graph graph;
    int n, stepCnt, colorCnt;
    readTestCaseFromFile("./data/sample1.txt", graph, n, stepCnt, colorCnt);
    graph.printGraph();
}
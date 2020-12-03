#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

struct Node{
    int id;
    int color;
    vector<int> neighbours;
    Node(){}
    Node(int id, int color): id(id), color(color){}
};

class Graph{
public:
    vector<Node*> nodes;
    int cost;
    vector<pair<int, int>> path;
    Graph(): cost(0){}
    Graph(int n){
        nodes = vector<Node*>(n, NULL);
        cost = 0;
    }

    void printGraph(){
        cout << "Graph:" << endl;
        cout << "Number of nodes: " << nodes.size() << endl;
        cout << "Color of each node: ";
        for(int i = 0; i < nodes.size(); i++){
            cout << "node " << i << ": " << nodes[i]->color << "; ";
        }
        cout << endl;
        cout << "Neighbour of each node: ";
        for(int i = 0; i < nodes.size(); i++){
            cout << "node " << i << ": " << "[";
            for(int j : nodes[i]->neighbours){
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
        int color = stoi(str);
        graph.nodes[i] = new Node(i, color);
    }
    for(int i = 0; i < n; i++){
        getline(file, str);
        vector<string> splits = splitStr(str, ' ');
        for(const string& idStr : splits){
            graph.nodes[i]->neighbours.push_back(stoi(idStr));
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
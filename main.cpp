#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <unordered_set>
using namespace std;

class Graph{
public:
    vector<int> nodeColor; // -1 means this node is already contracted with others
    vector<unordered_set<int>> neighbours; // adjacency list
    int n; // total number of nodes, won't change after creation
    int curNodeCnt; // current number of nodes
    int cost;
    vector<pair<int, int>> path;
    Graph(): n(0), curNodeCnt(0), cost(0){}
    Graph(int n): n(n), curNodeCnt(n), cost(0){
        nodeColor = vector<int>(n, -1);
        neighbours = vector<unordered_set<int>>(n);
    }

    void printGraph(){
        // cout << "Graph:" << endl;
        // cout << "Total number of nodes: " << n << endl;
        cout << "Current number of nodes: " << curNodeCnt << endl;
        cout << "Color of each node: ";
        for(int i = 0; i < n; i++){
            if(nodeColor[i] != -1){
                cout << "node " << i << ": " << nodeColor[i] << "; ";
            }
        }
        cout << endl;
        cout << "Neighbour of each node: ";
        for(int i = 0; i < n; i++){
            if(nodeColor[i] == -1){
                continue;
            }
            cout << "node " << i << ": " << "[";
            for(int j : neighbours[i]){
                cout << j << " ";
            }
            cout << "]; ";
        }
        cout << endl;
        cout << "Cost: " << cost << endl;
        cout << "Path: ";
        for(int i = 0; i < path.size(); i++){
            cout << "(" << path[i].first << ", " << path[i].second << "); ";
        }
        cout << endl;
    }

    /*
        Get color of the neighbours of the given node
    */
    vector<int> getNeighbourColors(int nodeId){
        if(nodeId < 0 || nodeId >= n || nodeColor[nodeId] == -1){
            return vector<int>();
        }
        unordered_set<int> res;
        for(int j : neighbours[nodeId]){
            if(nodeColor[j] != -1){
                res.insert(nodeColor[j]);
            }
        }
        return vector<int>(res.begin(), res.end());
    }

    /*
        Change the node color of the given nodeId to newColor
        Perform edge contraction and node merge if necessary
    */
    void changeNodeColor(int nodeId, int newColor){
        if(nodeId < 0 || nodeId >= n || nodeColor[nodeId] == -1){
            return;
        }
        nodeColor[nodeId] = newColor;
        cost++;
        path.emplace_back(nodeId, newColor);
        contract(nodeId);
    }
private:
    /*
        Contract this node with its neighbours with the same color
        The id of the merged node is the min id of these merged nodes
    */
    void contract(int nodeId){
        if(nodeId < 0 || nodeId >= n || nodeColor[nodeId] == -1){
            return;
        }
        unordered_set<int> nodeToMerge;
        int newNodeId = nodeId; // node id of the merged node
        for(int j : neighbours[nodeId]){
            if(nodeColor[nodeId] == nodeColor[j]){
                nodeToMerge.insert(j);
                newNodeId = min(newNodeId, j);
            }
        }
        if(nodeToMerge.empty()){
            // no need to merge
            return;
        }
        nodeToMerge.insert(nodeId);
        nodeToMerge.erase(newNodeId);
        neighbours[newNodeId].clear();
        for(int j : nodeToMerge){
            nodeColor[j] = -1; // remove node from the graph
        }
        curNodeCnt -= nodeToMerge.size();
        // therotically better way is to only consider the neighbours of {nodeToMerge + newNodeId}
        for(int i = 0; i < n; i++){
            if(nodeColor[i] == -1){
                continue;
            }
            for(int j : nodeToMerge){
                if(neighbours[i].find(j) != neighbours[i].end()){
                    neighbours[i].erase(j);
                    neighbours[i].insert(newNodeId);
                    neighbours[newNodeId].insert(i);
                }
            }
            if(neighbours[i].find(newNodeId) != neighbours[i].end()){
                neighbours[newNodeId].insert(i);
            }
        }
        for(int j : nodeToMerge){
            neighbours[j].clear(); // remove node from the graph
        }
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
            graph.neighbours[i].insert(stoi(idStr));
        }
    }
}

/*
    If you know the solution, you can use this function to print the graph of each step
*/
void manualVerify(Graph graph, const vector<pair<int, int>>& moves){
    cout << "Original Graph:" << endl;
    graph.printGraph();
    cout << endl;
    for(int i = 0; i < moves.size(); i++){
        cout << "Move " << i + 1 << ": " << endl;
        graph.changeNodeColor(moves[i].first, moves[i].second);
        graph.printGraph();
        cout << endl;
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
    vector<pair<int, int>> moves = {{4, 0}, {0, 1}, {0, 2}};
    manualVerify(graph, moves);
}
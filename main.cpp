#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <unordered_map>
using namespace std;

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

class Graph{
public:

    Graph(): n(0), curNodeCnt(0), cost(0), curColorCnt(0), hashVal(0), diameter(0), maxSameNeighbour(0){}

    // Graph(int n): n(0), curNodeCnt(0), cost(0), curColorCnt(0), hashVal(0), diameter(0), maxSameNeighbour(0){
    //     nodeColor = vector<int>(n, -1);
    //     neighbours = vector<unordered_set<int>>(n);
    // }

    Graph(const string& fileName): cost(0){
        ifstream file(fileName);
        string str;
        getline(file, str);
        n = curNodeCnt = stoi(str);
        nodeColor = vector<int>(n, -1);
        neighbours = vector<unordered_set<int>>(n);
        getline(file, str);
        // stepCnt = stoi(str);
        getline(file, str);
        // colorCnt = stoi(str);
        for(int i = 0; i < n; i++){
            getline(file, str);
            nodeColor[i] = stoi(str);
        }
        for(int i = 0; i < n; i++){
            getline(file, str);
            vector<string> splits = splitStr(str, ' ');
            for(const string& idStr : splits){
                neighbours[i].insert(stoi(idStr));
            }
        }
        updateGraphConstant();
    }

    // getters
    int getN() const{
        return n;
    }
    int getCurNodeCnt()const{
        return curNodeCnt;
    }
    int getCost() const{
        return cost;
    }
    int getCurColorCnt() const{
        return curColorCnt;
    }
    int getNodeColor(int id) const{
        if(id < 0 || id >= n){
            return -1;
        }
        return nodeColor[id];
    }
    size_t getHashVal() const{
        return hashVal;
    }
    int getDiameter() const{
        return diameter;
    }
    int getMaxSameNeighbour() const{
        return maxSameNeighbour;
    }
    vector<pair<int,int>> getPath() const{
        return path;
    }
    vector<int> getNodeColor() const{
        return nodeColor;
    }
    vector<unordered_set<int>> getNeighbours() const{
        return neighbours;
    }

    bool operator==(const Graph& graph) const{
        return nodeColor == graph.nodeColor && neighbours == graph.neighbours;
    }


    void printGraph() const{
        // cout << "Graph:" << endl;
        // cout << "Total number of nodes: " << n << endl;
        cout << "Current number of nodes: " << curNodeCnt << endl;
        cout << "Current number of colors: " << curColorCnt << endl;
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
            res.insert(nodeColor[j]);
        }
        res.erase(-1);
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
        updateGraphConstant();
    }

    /*
        Generate all possible next graphs
    */
    vector<Graph> getNextGraphs(){
        vector<Graph> ans;
        for(int i = 0; i < n; i++){
            if(nodeColor[i] == -1){
                continue;
            }
            vector<int> colors = getNeighbourColors(i);
            for(int c : colors){
                Graph graph(*this); // copy current graph
                graph.changeNodeColor(i, c);
                ans.push_back(graph);
            }
        }
        return ans;
    }

private:
    int n; // total number of nodes, won't change after creation
    int curNodeCnt; // current number of nodes
    int cost;
    int curColorCnt; // remaining colors
    size_t hashVal; // hashValue of current graph
    int diameter; // defined as the longest distance(length of shortest path) between two nodes
    int maxSameNeighbour; // max number of neighbours with the same color
    vector<int> nodeColor; // -1 means this node is already contracted with others
    vector<unordered_set<int>> neighbours; // adjacency list
    vector<pair<int, int>> path;

    /*
        Update the attributes of this graph, only need to be called after the graph is changed (in constructor() and changeNodeColor())
        Then we can return these attributes in O(1) time when queried
    */
    void updateGraphConstant(){
        updateCurColorCnt();
        updateHashVal();
        updateDiameter();
        updateMaxSameNeighbour();
    }

    void updateCurColorCnt(){
        unordered_set<int> seen(nodeColor.begin(), nodeColor.end());
        seen.erase(-1);
        curColorCnt = seen.size();
    }

    /*
        Return the hash value calculated by converting graph to string and return hash value of the string
    */
    size_t getStrHash() const{
        string res = "[";
        for(int i : nodeColor){
            res += to_string(i);
            res.push_back(',');
        }
        res += "];[";
        for(int i = 0; i < neighbours.size(); i++){
            vector<int> nums(neighbours[i].begin(), neighbours[i].end());
            sort(nums.begin(), nums.end());
            res.push_back('(');
            for(int num : nums){
                res += to_string(num);
            }
            res.push_back(')');
        }
        res.push_back(']');
        return hash<string>{}(res);
    }

    void updateHashVal(){
        hashVal = getStrHash(); // may change to other hash implementation
    }

    void updateDiameter(){
        // TODO, Floyd-Warshall?
    }

    void updateMaxSameNeighbour(){
        maxSameNeighbour = 0;
        for(int i = 0; i < n; i++){
            unordered_map<int, int> neighbourColorCnt;
            for(int j : neighbours[i]){
                neighbourColorCnt[nodeColor[j]]++;
                maxSameNeighbour = max(maxSameNeighbour, neighbourColorCnt[nodeColor[j]]);
            }
        }
    }

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

struct GraphHashStr{
    size_t operator()(const Graph& g) const{
        return g.getHashVal();
    }
};


void readTestCaseFromFile(const string& fileName, Graph& graph, int& n, int& stepCnt, int& colorCnt){
    ifstream file(fileName);
    string str;
    getline(file, str);
    n = stoi(str);
    getline(file, str);
    stepCnt = stoi(str);
    getline(file, str);
    colorCnt = stoi(str);
    graph = Graph(fileName);
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

/*
    Input a graph, print all possible next graphs
*/
void testGetNextGraphs(Graph graph){
    cout << "Original Graph:" << endl;
    graph.printGraph();
    cout << endl;
    vector<Graph> graphs = graph.getNextGraphs();
    cout << "Next graphs cnt = " << graphs.size() << endl;
    for(const Graph& g : graphs){
        g.printGraph();
        cout << endl;
    }
}

/*
    Return true if this graph is impossible to get to the end
    @param limit: total steps allowed (same as the input of solve())
*/
bool prune(const Graph& graph, int limit){
    int leftMoves = limit - graph.getCost();
    return leftMoves + 1 < graph.getCurColorCnt(); // TODO diameter
}

vector<pair<int, int>> solve(Graph graph, int limit){
    // 3 heuristic functions
    auto comp1 = [](const Graph& g1, const Graph& g2){return g1.getCurColorCnt() + g1.getCost() > g2.getCurColorCnt() + g2.getCost();};
    auto comp2 = [](const Graph& g1, const Graph& g2){return g1.getCurNodeCnt() + g1.getCost() > g2.getCurNodeCnt() + g2.getCost();};
    auto comp3 = [](const Graph& g1, const Graph& g2){return g1.getMaxSameNeighbour() < g2.getMaxSameNeighbour();}; // minimum on the top
    priority_queue<Graph, vector<Graph>, decltype(comp1)> pq(comp1);
    unordered_set<Graph, GraphHashStr> seen;
    pq.push(graph);
    seen.insert(graph);
    while(!pq.empty()){
        Graph curG = pq.top();
        pq.pop();
        vector<Graph> nextGraphs = curG.getNextGraphs();
        for(const Graph& g : nextGraphs){
            if(g.getCurNodeCnt() == 1){
                // succeed
                return g.getPath();
            }
            if(!prune(g, limit) && seen.find(g) == seen.end()){
                pq.push(g);
                seen.insert(g);
            }
        }
    }
    return vector<pair<int, int>>();
}

int main(){
    cout << "Hello CSE 202" << endl;
    Graph graph;
    int n, stepCnt, colorCnt;
    string fileName = "./data/sample1.txt";// "./data/game5-24-7-4.txt";
    readTestCaseFromFile(fileName, graph, n, stepCnt, colorCnt);
    vector<pair<int, int>> moves = {{4, 0}, {0, 1}, {0, 2}};
    // manualVerify(graph, moves);
    // testGetNextGraphs(graph);
    auto path = solve(graph, stepCnt);
    manualVerify(graph, path);
}
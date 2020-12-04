#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <unordered_set>
#include <queue>
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

    Graph(): n(0), curNodeCnt(0), cost(0), curColorCnt(0){}

    Graph(int n): n(n), curNodeCnt(n), cost(0), curColorCnt(0){
        nodeColor = vector<int>(n, -1);
        neighbours = vector<unordered_set<int>>(n);
    }

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
        updateColorNum();
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
    vector<pair<int,int>> getPath() const{
        return path;
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
        updateColorNum();
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

    /*
        Return the diameter of current graph, which is defined as the longest distance between two nodes
    */
    int getDiameter(){
        // TODO
        return 0;
    }
private:
    int n; // total number of nodes, won't change after creation
    int curNodeCnt; // current number of nodes
    int cost;
    int curColorCnt; // remaining colors
    vector<int> nodeColor; // -1 means this node is already contracted with others
    vector<unordered_set<int>> neighbours; // adjacency list
    vector<pair<int, int>> path;

    void updateColorNum(){
        unordered_set<int> seen(nodeColor.begin(), nodeColor.end());
        seen.erase(-1);
        curColorCnt = seen.size();
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
    auto comp = [](const Graph& g1, const Graph& g2){return g1.getCurColorCnt() + g1.getCost() > g2.getCurColorCnt() + g2.getCost();};
    priority_queue<Graph, vector<Graph>, decltype(comp)> pq(comp);
    // unordered_set<Graph> seen; // TODO, need to override hash and == comparator
    pq.push(graph);
    // seen.insert(graph);
    while(!pq.empty()){
        Graph curG = pq.top();
        pq.pop();
        vector<Graph> nextGraphs = curG.getNextGraphs();
        for(const Graph& g : nextGraphs){
            if(g.getCurNodeCnt() == 1){
                // succeed
                return g.getPath();
            }
            if(!prune(g, limit) /* && seen.find(g) == seen.end()*/){
                pq.push(g);
                // seen.insert(g);
            }
        }
    }
    return vector<pair<int, int>>();
}

int main(){
    cout << "Hello CSE 202" << endl;
    Graph graph;
    int n, stepCnt, colorCnt;
    readTestCaseFromFile("./data/sample1.txt", graph, n, stepCnt, colorCnt);
    vector<pair<int, int>> moves = {{4, 0}, {0, 1}, {0, 2}};
    // manualVerify(graph, moves);
    // testGetNextGraphs(graph);
    auto path = solve(graph, stepCnt);
    manualVerify(graph, path);
}
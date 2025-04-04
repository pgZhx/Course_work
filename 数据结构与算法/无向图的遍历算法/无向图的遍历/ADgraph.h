//
// Created by 26618 on 2023/12/10.
//

#ifndef PROJECT_6_ADGRAPH_H
#define PROJECT_6_ADGRAPH_H
#include<iostream>
#include<fstream>
#include<vector>
#include<stack>
#include<queue>
using namespace std;
struct vex{
    string data;
    vector<int>adj;
    vector<vex*> children;
};
struct ADgraph{
    int vnum;
    vector<vex>list;
    void addEdge(int v,int w){
        list[v].adj.push_back(w);
        list[w].adj.push_back(v);
    };
};
void Create_ADgraph(ADgraph& g){
    ifstream file("AD_input.txt");
    int v;
    file >> v;
    g.vnum = v;
    g.list.resize(v);
    for(int i = 0;i < v;i++){
        string str;
        file >> str;
        g.list[i].data = str;
    }
    for(int i = 0;i < v;i++){
        int m,n;
        file >> m >> n;
        g.addEdge(m,n);
    }
}
void DFS_AD(ADgraph& g,vector<bool>& visited,int s,vector<int>& parent,int depth){
    for(int i = 0;i < depth;i++){
        cout << "   ";
    }
    cout << g.list[s].data << endl;
    visited[s] = true;
    for(int i:g.list[s].adj){
        if(!visited[i]) {
            parent[i] = s;
            DFS_AD(g, visited, i, parent, depth + 1);
        }
    }
}
void BFS_AD(ADgraph&g,int s){
    vector<bool>visited(g.vnum,false);
    queue<int> q;
    q.push(s);
    visited[s] = true;
    while(!q.empty()) {
        int x = q.front();
        q.pop();
        for (int i: g.list[x].adj) {
            if (!visited[i]) {
                q.push(i);
                g.list[x].children.push_back(&g.list[i]);
                visited[i] = true;
            }
        }
    }
}
void BFSTree(ADgraph&g,vex* s,int depth){
    for(int i = 0;i < depth;i++){
        cout << "     ";
    }
    cout << s->data << endl;
    for(auto it:s->children) {
        BFSTree(g,it,depth+1);
    }
}

#endif //PROJECT_6_ADGRAPH_H

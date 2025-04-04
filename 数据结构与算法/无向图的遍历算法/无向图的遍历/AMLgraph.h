//
// Created by 26618 on 2023/12/9.
//

#ifndef PROJECT_6_AMLGRAPH_H
#define PROJECT_6_AMLGRAPH_H


#include<iostream>
#include<fstream>
#include<vector>
#include"Mystack.h"
#include<queue>
#include<iomanip>
using namespace std;
class Edge{
public:
    int vexi;
    int vexj;
    Edge* linki;
    Edge* linkj;
    int weight;
    Edge(int i,int j,int wei):vexi(i),vexj(j),weight(wei),linki(nullptr),linkj(nullptr){}
};
class Vex{
public:
    string place;
    Edge* Fout;
    Vex(string p):place(p),Fout(nullptr){}
};
struct AMLGraph{
public:
    vector<Vex> adj;
    int vexnum;//顶点数
    int edgenum;//边数
};
int PosVex(AMLGraph&g, string s){
    int num;
    for(num = 0;num < g.vexnum;num++){
        if(s == g.adj[num].place){
            return num;
        }
    }
    return -1;
}
void Create_AMLgraph(AMLGraph& g){
    ifstream file("AML_input.txt");
    int vnum,ednum,m,n;
    file >> vnum >> ednum;
    g.vexnum = vnum;
    g.edgenum = ednum;
    for(int i = 0;i < g.vexnum;i++){
        string p;
        file >> p;
        Vex v(p);
        g.adj.push_back(v);
    }
    for(int i = 0;i < g.edgenum;i++){
        int weight;
        string vm,vn;
        file >> vm >> vn >> weight;
        m = PosVex(g,vm);
        n = PosVex(g,vn);
        Edge* e = new Edge(m,n,weight);
        e -> linki = g.adj[m].Fout;
        e -> linkj = g.adj[n].Fout;
        g.adj[m].Fout = e;
        g.adj[n].Fout = e;
    }
}
void DFS_AML(AMLGraph &g, int s) {
    int n = g.vexnum;
    bool visited[n];
    for(int i = 0;i < n;i++){
        visited[i] = false;
    }
    MyStack sta(30); // 存储顶点索引的栈
    sta.push(s); // 将起始顶点入栈
    visited[s] = true;
    cout << setw(10) <<g.adj[s].place << " ";   int i =1;
    while(!sta.stackEmpty()){
        int idx = sta.m_pBuffer[sta.m_iTop-1];
        Edge* e = g.adj[idx].Fout;
        bool flag = false;
        while(e){
            int nextIdx = (e->vexi == idx) ? e->vexj : e->vexi; // 获取邻接顶点的索引
            if(!visited[nextIdx]){
                cout << "第" << i++ << "条边为： " << g.adj[e -> vexi].place << " - " << g.adj[e -> vexj].place<< " 它的权重为： "<< e->weight << endl;
                cout << setw(10) << g.adj[nextIdx].place << " "; // 访问顶点
                sta.push(nextIdx); // 将未访问的邻接顶点入栈
                visited[nextIdx] = true; // 标记为已访问
                flag = true;
                break;
            }
            e = (e->vexi == idx) ? e->linki : e->linkj; // 获取下一条边
        }
        if(!flag){
            sta.pop(); // 如果当前顶点的所有邻接顶点都已访问过，出栈
        }
    }
    cout << endl;
}
void BFS_AML(AMLGraph &g, int s) {
    int n = g.vexnum;
    bool visited[n];
    for(int i = 0;i < n;i++){
        visited[i] = false;
    }
    queue<int>q;
    q.push(s);
    visited[s] = true;
    cout << setw(10) << g.adj[s].place << ' ';
    int i = 1;
    while(!q.empty()){
        int idx = q.front();
        q.pop();
        Edge* e = g.adj[idx].Fout;
        while(e){
            int nextIdx = (e->vexi == idx) ? e->vexj : e->vexi; // 获取邻接顶点的索引
            if(!visited[nextIdx]){
                cout << "第" << i++ << "条边为： " << g.adj[e -> vexi].place << " - " << g.adj[e -> vexj].place<< " 它的权重为： "<< e->weight << endl;
                cout << setw(10) << g.adj[nextIdx].place<< " ";
                visited[nextIdx] = true;
                q.push(nextIdx);
            }
            e = (e->vexi == idx) ? e->linki : e->linkj; // 获取下一条边
        }
    }
    cout << endl;
}


#endif //PROJECT_6_AMLGRAPH_H

#include <iostream>
#include "AMLgraph.h"
#include "ADgraph.h"
int main() {
    system("chcp 65001");

    AMLGraph g;
    ADgraph G;
    Create_AMLgraph(g);
    Create_ADgraph(G);
    while(1){
        system("cls");
        cout << "   MENU:"<< endl;
        cout << "   0.退出程序"<< endl;
        cout << "   1.以邻接多重表为存储结构，进行深度优先搜索"<< endl;
        cout << "   2.以邻接多重表为存储结构，进行广度优先搜索"<< endl;
        cout << "   3.以邻接表为存储结构，树形输出深度优先生成树"<< endl;
        cout << "   4.以邻接表为存储结构，树形输出广度优先生成树"<< endl;
        int instr;
        cin >> instr;
        cin.clear();
        cin.sync();
        switch (instr) {
            case 0: return 0;
            case 1:{
                system("cls");
                cout << "请输入开始的节点"<<endl;
                string place;
                cin >> place;
                cin.clear();
                cin.sync();
                int s = PosVex(g,place);
                cout << "以"<< place << "为起点按照深度优先搜索的节点访问顺序输出的节点序列和生成树的边集如下："<<endl;
                DFS_AML(g,s);
                system("pause");
                break;
            }
            case 2:{
                system("cls");
                cout << "请输入开始的节点"<<endl;
                string place;
                cin >> place;
                cin.clear();
                cin.sync();
                int s = PosVex(g,place);
                cout << "以"<< place << "为起点按照广度优先搜索的节点访问顺序输出的节点序列和生成树的边集如下："<<endl;
                BFS_AML(g,s);
                system("pause");
                break;
            }
            case 3:{
                system("cls");
                cout << "树形输出深度优先生成树（缩进形式）如下："<< endl;
                vector<bool>visited(G.vnum,false);
                vector<int>parent(G.vnum,-1);
                parent.resize(G.vnum);
                DFS_AD(G,visited,0,parent,0);
                system("pause");
                break;
            }
            case 4:{
                system("cls");
                cout << "树形输出广度优先生成树（缩进形式）如下："<< endl;
                BFS_AD(G,0);
                BFSTree(G,&G.list[0],0);
                system("pause");
                break;
            }
            default:{
                cout << "输入非法,重新输入"<< endl;
                system("pause");
            }
        }
    }
}

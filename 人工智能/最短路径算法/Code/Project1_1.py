#Dijkstra
in_file = open("D:\Code\Python\Lab1\input.txt", "r")
mn = in_file.readline().split()
m = int(mn[0])
n = int(mn[1])
Max = float('Inf')
import numpy as np
# 创建一个m行m列的矩阵，所有元素的值都是无穷大
Graph = np.full((m, m), np.inf)
i = 0
while i < m:
    Graph[i][i] = 0
    i += 1
i = 0
j = 0
dict = {}#字典用于存放各节点及其编号
while i < n:
    s = in_file.readline()
    s = s.split()
    a = s[0]
    b = s[1]
    #ab是两节点名称，a1b1是两节点对应的编号
    weight = int(s[2])
    if a in dict:
        a1 = dict[a]
    else:
        dict[a] = j
        a1 = j
        j += 1
    if b in dict:
        b1 = dict[b]
    else:
        dict[b] = j
        b1 = j
        j += 1
    Graph[a1][b1] = weight
    Graph[b1][a1] = weight
    i += 1  #建立邻接矩阵
import copy
while True:
    s = in_file.readline()
    s = s.split()
    start = s[0]
    if start == "Stop":
        break
    sta = dict[start]#start的编号
    end = s[1]
    en = dict[end]#end的编号
    dist = copy.copy(Graph[sta])
    open = []#open中存储编号和距离起点的int二元组
    close = []#close中存储open中已经确定最短路径的int二元组
    parent = []#存储每个节点最短路径上的前驱，起点的parent为None
    open.append((sta,0))
    while True:
        if len(open) == 0:
            break
        min_dist = Max
        min = -1 # min为open中距离起点最近的点的编号
        for i in range(len(open)):
            if open[i][1] < min_dist:
                min_dist = open[i][1]
                min = i
        cur, cur_dist = open.pop(min)
        #cur为该轮能确定最短路径的点
        if cur == en:
            break
        for j in range(len(Graph[cur])):#所有和min相邻的节点更新距离
            if (Graph[cur][j] < Max) and (j != cur):
                new_dist = cur_dist + Graph[cur][j]
                if new_dist <= dist[j]:
                    dist[j] = new_dist
                    open.append((j, new_dist))
                    flag = False
                    for i in parent:
                        if i[0] == j:
                            flag = True
                            parent.remove(i)
                            parent.append((j,cur))
                            break
                    if flag == False:
                        if j == sta:
                            parent.append((j,None))
                        else:
                            parent.append((j,cur))
        close.append((cur, cur_dist))
    path = []
    path.append(en)
    temp = en
    while temp != sta:
        for i in parent:
            if i[0] == temp:
                temp = i[1]#将temp更新为它的前驱
                path.append(i[1])
                break
    path.reverse()
    print(start,"到",end,"的最短路径为：")
    for i in path:
        for key, value in dict.items():
            if value == i:
                if value == en:
                    print(key)
                else:
                    print(key, end="->")
                break
    print("长度为",int(dist[en]))
    input("请按任意键继续...")

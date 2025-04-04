#FLoyd
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
dict = {}
#字典用于存放各节点及其编号
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
    i += 1   
#建立邻接矩阵
parents = np.full((m,m),np.inf)
for i in range(m):
    for j in range(m):
        parents[i][j] = i
for k in range(m):
        for i in range(m):
            for j in range(m):
                if Graph[i][k] + Graph[k][j] < Graph[i][j]:
                    Graph[i][j] = Graph[i][k] + Graph[k][j]
                    parents[i][j] = parents[k][j]
while True:
    s = in_file.readline()
    s = s.split()
    start = s[0]
    if start == "Stop":
        break
    sta = int(dict[start])#start的编号
    end = s[1]
    en = int(dict[end])#end的编号
    path = []
    temp = en
    while temp != sta:
        if parents[sta][temp] < Max:
            path.append(temp)
            temp = parents[sta][temp]
            temp = int(temp)
        else:
            break
    path.append(temp)
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
    print("长度为",Graph[sta][en])
    input("请按任意键继续...")


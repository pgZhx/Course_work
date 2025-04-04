import time
import numpy as np
global index
index = 1
end = [[1,2,3,4],[5,6,7,8],[9,10,11,12],[13,14,15,0]]
def calculate_h1(matrix):#启发函数1：错牌数量
    h = 0
    for i in range(4):
        for j in range(4):
            if matrix[i][j] != 0:
                continue
            if matrix[i][j] != end[i][j]:
                h += 1
    return h
def calculate_h2(matrix):#启发式函数2：曼哈顿距离
    distance = 0
    for i in range(4):
        for j in range(4):
            if matrix[i][j] != 0:
               target_row = (matrix[i][j]-1) // 4
               target_col = (matrix[i][j]-1) % 4
               distance += abs(i-target_row)+abs(j-target_col)
    return distance
def find_zero(matrix):
    for i in range(4):
        for j in range(4): 
            if matrix[i][j] == 0:
                return (i,j)
def if_exist(close,matrix):
    for i in close:
        if np.all(i[0] == matrix):
            return True
    return False
def expand(open,close):
    cur = open.pop(0)
    close.append(cur)
    position = find_zero(cur[0])
    i = position[0]
    j = position[1]
    neighbors = [(i+1,j),(i,j+1),(i-1,j),(i,j-1)]
    for neighbor in neighbors:
        if neighbor[0] < 0 or neighbor[0] > 3 or neighbor[1] < 0 or neighbor[1] > 3:
            continue
        m_new = np.copy(cur[0])
        temp = m_new[neighbor[0]][neighbor[1]]
        m_new[neighbor[0]][neighbor[1]] = 0
        m_new[i][j] = temp
        if if_exist(close,m_new) == True:
            continue
        else: 
            h = calculate_h2(m_new)
            step = cur[5]+1
            global index
            open.append([m_new, h+step, temp,index,cur[3],step])
            index += 1
def A_star(matrix):
    start = time.time()
    h = calculate_h2(matrix)
    open = [[matrix,h,':',0,None,0]]#1为矩阵2为启发值3为动作4为自己编号5为父母编号
    close = []
    print("动作序列如下",end = '')
    while True:
        open.sort(key=lambda x: x[1])
        if calculate_h2(open[0][0]) == 0:
            temp = open[0]
            action = []
            while True:
                parent = temp[4]
                action.append(temp[2])
                if temp[2] == ':'or temp[4] == None:break
                for i in close:
                    if i[3] == parent:
                        temp = i
                        break
            action.reverse()
            for i in action:
                print(i, end = ' ')
            print("\n 步数为：", len(action)-1)
            end = time.time()
            print( "所花时间为：",end - start,"s",sep = '')
            return time
        expand(open, close)
test1 = [[1,15,7,10],[9,14,4,11],[8,5,0,6],[13,3,2,12]]
test2 = [[1,7,8,10],[6,9,15,14],[13,3,0,4],[11,5,12,2]]
test3 = [[5,6,4,12],[11,14,9,1],[0,3,8,15],[10,7,2,13]]
test4 = [[14,2,8,1],[7,10,4,0],[6,15,11,5],[9,3,13,12]]
print("test1")
A_star(test1)
print("test2")
A_star(test2)
print("test3")
A_star(test3)
print("test4")
A_star(test4)
input("按任意键继续")
from collections import deque
import time
import numpy as np
global index
index = 1
end = [[1,2,3,4],[5,6,7,8],[9,10,11,12],[13,14,15,0]]
def calculate_h1(matrix): # 启发函数1：错牌数量
    h = 0
    for i in range(4):
        for j in range(4):
            if matrix[i][j] != 0 and matrix[i][j] != end[i][j]:
                h += 1
    return h
def calculate_h2(matrix): # 启发式函数2：曼哈顿距离
    distance = 0
    for i in range(4):
        for j in range(4):
            if matrix[i][j] != 0:
                target_row = (matrix[i][j]-1) // 4
                target_col = (matrix[i][j]-1) % 4
                distance += abs(i-target_row) + abs(j-target_col)
    return distance
def find_x(matrix,x):
    for i in range(4):
        for j in range(4):
            if matrix[i][j] == x:
                return (i,j)
def if_exist(close, matrix):
    matrix_tuple = tuple(map(tuple, matrix))
    for i in close:
        if i[1] == matrix_tuple:
            return True
    return False
import heapq
def swap_elements (original, pos1, pos2):
    # 创建一个新的元组列表，避免修改原始元组
    new_tuple_list = [None] * len(original)
    for i, row in enumerate(original):
        new_tuple_list[i] = [None] * len(row)
        for j, element in enumerate(row):
            new_tuple_list[i][j] = element
    # 交换元素
    new_tuple_list[pos1[0]][pos1[1]], new_tuple_list[pos2[0]][pos2[1]] = new_tuple_list[pos2[0]][pos2[1]], new_tuple_list[pos1[0]][pos1[1]]
    # 将新的元组列表转换为元组的元组
    new_tuple = tuple(tuple(row) for row in new_tuple_list)
    return new_tuple
def expand(open:heapq, close:set):
    cur = heapq.heappop(open)
    close.add(cur)
    position = find_x(cur[1],0)
    i = position[0]
    j = position[1]
    neighbors = [(i+1,j), (i,j+1), (i-1,j), (i,j-1)]
    for neighbor in neighbors:
        if neighbor[0] < 0 or neighbor[0] > 3 or neighbor[1] < 0 or neighbor[1] > 3:
            continue
        temp = cur[1][neighbor[0]][neighbor[1]]
        m_new = swap_elements(cur[1],(i,j),neighbor)
        if if_exist(close, m_new) == True:
            continue
        else:
            h = calculate_h2(m_new)
            step = cur[5]+1
            global index
            heapq.heappush(open,(h+step,m_new,temp,index,cur[3],step))
            index += 1
def A_star(matrix):
    start = time.time()
    h = calculate_h2(matrix)
    open = [(h,tuple(map(tuple,matrix)),':',0,None,0)] # 1为启发值2为矩阵3为动作4为自己编号5为父母编号
    heapq.heapify(open)
    close = set()
    print("动作序列如下:")
    while True:
        if calculate_h2(open[0][1]) == 0:
            temp = open[0]
            action = []
            while True:
                parent = temp[4]
                #action.append((temp[2],temp[1]))
                action.append(temp[2])
                if temp[2] == ':' or temp[4] == None:
                    break
                for i in close:
                    if i[3] == parent:
                        temp = i
                        break
            action.reverse()
            for i in action:
                if i == ":":
                    continue
                print(i,end=' ')
                # print("----------------------------------------------")
                # if i[0] == ":":
                #     print("初始状态为：")
                # else:
                #     print("将0与",i[0],"交换得：",sep = '')
                # for m in range(4):
                #     for n in range(4):
                #         print(f"{str(i[1][m][n]):<3}", end=' ')
                #     print()
            print()
            print("步数为：", len(action)-1)
            end = time.time()
            print("所花时间为：", end - start, "s", sep='')
            return time
        expand(open, close)
def IDA_star(matrix):
    matrix = tuple(map(tuple, matrix))
    start = time.time()
    limit = calculate_h2(matrix)
    stack = deque()
    flag = False
    close = set()
    while True:
        stack.append((matrix,limit,0,tuple()))#0为节点状态1为当前f值2为g值3为动作序列
        limi = float('inf')
        close.clear()
        close.add(matrix)
        while len(stack) != 0:
            cur = stack.pop()
            if cur[1] - cur[2] == 0:
                new_path = cur[3]
                flag = True
                break
            else:
                position = find_x(cur[0],0)
                i = position[0]
                j = position[1]
                neighbors = [(i+1,j), (i,j+1), (i-1,j), (i,j-1)]
                for neighbor in neighbors:
                    if neighbor[0] < 0 or neighbor[0] > 3 or neighbor[1] < 0 or neighbor[1] > 3:
                        continue
                    temp = cur[0][neighbor[0]][neighbor[1]]
                    m_new = swap_elements(cur[0], (i, j), neighbor)
                    if m_new in close:
                        continue
                    h = calculate_h2(m_new)
                    g = cur[2] + 1
                    f = g + h
                    if f <= limit:
                        stack.append((m_new,f,g,cur[3] + (temp,)))
                        close.add(m_new)
                    else:
                        limi = min(limi,f)
                close.remove(cur[0])
        if flag == True:
            break
        else:
            limit = limi
    end = time.time()
    print("动作序列为")
    curr = matrix
    for i in new_path:
        # print("将0与",i,"交换得：",sep = '')
        # position = find_x(curr, 0)
        # i1 = position[0]
        # j1 = position[1]
        # position = find_x(curr, i)
        # i2 = position[0]
        # j2 = position[1]
        # curr = swap_elements(curr,(i1,j1),(i2,j2))
        # print("----------------------------------------------")
        # for m in range(4):
        #     for n in range(4):
        #         print(f"{str(curr[m][n]):<3}", end=' ')
        #     print()
        print(i,end = ' ')
    print()

    print("步数为:",len(new_path))
    print("所花时间为:",end - start,'s',sep='')
zip_test1 = [[1,15,7,10],[9,14,4,11],[8,5,0,6],[13,3,2,12]]
zip_test2 = [[1,7,8,10],[6,9,15,14],[13,3,0,4],[11,5,12,2]]
zip_test3 = [[5,6,4,12],[11,14,9,1],[0,3,8,15],[10,7,2,13]]
zip_test4 = [[14,2,8,1],[7,10,4,0],[6,15,11,5],[9,3,13,12]]
pdf_test_1 = [[14,10,6,0],[4,9,1,8],[2,3,5,11],[12,13,7,15]]
pdf_test_2 = [[6,10,3,15],[14,8,7,11],[5,1,0,2],[13,12,9,4]]
pdf_test_3 = [[11,3,1,7],[4,6,8,2],[15,9,10,13],[14,12,5,0]]
pdf_test_4 = [[0,5,15,14],[7,9,6,13],[1,2,12,10],[8,11,4,3]]
print("zip_test1(A*):")
A_star(zip_test1)
print("zip_test2(A*):")
A_star(zip_test2)
print("zip_test3(A*):")
A_star(zip_test3)
print("zip_test4(A*):")
A_star(zip_test4)
print("zip_test1(迭代加深A*)")
IDA_star(zip_test1)
print("zip_test2(迭代加深A*)")
IDA_star(zip_test2)
print("zip_test3(迭代加深A*)")
IDA_star(zip_test3)
print("zip_test4(迭代加深A*)")
IDA_star(zip_test4)
# print("pdf_test1(A*):")
# A_star(pdf_test_1)
# print("pdf_test2(A*):")
# A_star(pdf_test_2)
# print("pdf_test3(A*):")
# A_star(pdf_test_3)
# print("pdf_test4(A*):")
# A_star(pdf_test_4)
# print("pdf_test1(IDA*)")
# IDA_star(pdf_test_1)
# print("pdf_test2(IDA*)")
# IDA_star(pdf_test_2)
# print("pdf_test3(IDA*)")
# IDA_star(pdf_test_3)
# print("pdf_test4(IDA*)")
# IDA_star(pdf_test_4)
input("按任意键继续...")
# print("请选择要进行逻辑推理的示例：")
# print("1.Alpine Club")
# print("2.Block World")
with open('D:\Code\Python\Lab2\input2.txt', 'r', encoding='utf-8') as in_file:
    #打开文件读取输入，并指定编码格式便于打印'¬'
    n = int(in_file.readline())
    s = []
    import re
    for i in range(n):
        s.append(re.findall(r'¬?\w+\(\w+\,*\w*\)',in_file.readline()))
        #通过正则表达式识别文件中的子句并存储成字符串
        for j  in range (len(s[i])):
            s[i][j] = s[i][j].replace('(',',')
            s[i][j] = s[i][j].replace(')','')
            s[i][j] = s[i][j].split(',')
    #利用replace函数将每个字符串形式转换为类似于[On,aa,bb]的格式
def opposite(str):
    if str[0] == '¬':
        return str[1:]
    else:
        return '¬'+str
    #取反函数，以得到一个谓词的取反
def unique(s_list):
    u_list = list(set(map(tuple, s_list)))
    return u_list
    #去重函数，用于去掉集合中的重复元素
import queue
variable = []#存储子句中所有出现的变量，用于后续辨别项是变量或常量
for i in range(n):
    for j in range(len(s[i])):
        for k in range(1,len(s[i][j])):
            if len(s[i][j][k]) == 1:
                variable.append(s[i][j][k])
parent = []#parent列表用于存储得到某个子句的两个前驱的子句及其对应的原子公式编号
assignment = []#assignment用于存储得到某个子句需要进行的替换操作
import copy
def solution(s:list,assignment:list,parent:list):
    for i,si in enumerate(s):#遍历第i个子句
        for j,ji in enumerate(s):#遍历第j个子句
            if i == j: 
                continue
            for p in range(len(s[i])):#遍历第i个子句的第p个原子公式
                for q in range(len(s[j])):#遍历第j个子句的第q个原子公式
                    if s[i][p][0] == opposite(s[j][q][0]) and len(s[i][p]) == len(s[j][q]):
                        #if谓词相反且项数相等
                        if s[i][p][1:] == s[j][q][1:]:#if除了谓词外的其他所有项都相等
                            temp1 = copy.deepcopy(s[i])
                            temp2 = copy.deepcopy(s[j])#深拷贝
                            del temp1[p]
                            del temp2[q]#去除谓词
                            temp = unique(temp1 + temp2)#合并得到归结后的子句
                            parent.append([i,p,j,q])
                            assignment.append([])#更新parent和assignment
                            s.append(temp)#更新s列表
                            if temp == []:
                                return #得到空子句则return
                        else:#仅谓词相反，需进行进一步判断
                            judge = [] #二元组前一个为常量，后一个为变量
                            for m in range(1,len(s[i][p])):
                                if s[i][p][m] not in variable and s[j][q][m] not in variable and s[i][p][m] == s[j][q][m]:
                                    continue #都为常量且相同
                                elif s[i][p][m] not in variable and s[j][q][m] in variable:
                                    judge.append((s[i][p][m],s[j][q][m]))#前一个为常量，后一个为变量，则可合一，记录置换操作
                                else:
                                    judge = False#无法合一
                                    break
                            if judge == False:
                                continue
                            else:
                                temp1 = copy.deepcopy(s[i])
                                temp2 = copy.deepcopy(s[j])
                                del temp1[p]
                                del temp2[q]
                                for m in range(len(judge)):#对每一次置换操作
                                    for k in range(len(temp2)):#遍历整个子句
                                        while judge[m][1] in temp2[k]:
                                            index = temp2[k].index(judge[m][1])
                                            temp2[k] = list(temp2[k])
                                            temp2[k].remove(judge[m][1])
                                            temp2[k].insert(index,judge[m][0])
                                            temp2[k] = tuple(temp2[k])
                                temp = unique(temp1 + temp2)
                                parent.append([i,p,j,q])
                                assignment.append(judge)
                                s.append(temp)
                                if temp == []:
                                    return
def useful_senten(assignment:list,parent:list):#回溯挑选有用的子句，并返回相关的所有信息
    useful = []
    q = queue.Queue()
    q.put(parent[-1])
    useful.append([s[-1],parent[-1],assignment[-1]])#将得到空子句的前驱和置换操作存储
    while not q.empty():
        pre = q.get()
        if pre[0] >= n:#若得到此子句的第一个子句不是原来的子句，则需要存储
            useful.append([s[pre[0]],parent[pre[0]-n],assignment[pre[0]-n]])
            q.put(parent[pre[0]-n])
        if pre[2] >= n:#若得到此子句的第二个子句不是原来的子句，则需要存储
            useful.append([s[pre[2]],parent[pre[2]-n],assignment[pre[2]-n]])
            q.put(parent[pre[2]-n])
    return useful
solution(s,assignment,parent)#通过solution求得归结过程中的所有信息
uu = useful_senten(assignment,parent)#简化solution中得到的信息，挑选有用的
reindex = dict()#对这些有用的子句进行重新编号
for i in range(n):
    reindex[i] = None#文件中提供的原先有的子句
for i,ui in enumerate(uu):
    if ui[1][0] not in reindex:#第一个前驱子句还没进入字典
        reindex[ui[1][0]] = None
    if ui[1][2] not in reindex:#第二个前驱子句还没进入字典
        reindex[ui[1][2]] = None    
reindex = sorted(reindex.keys())
reindex = {x:reindex.index(x)+1 for x in reindex}
def takeindex(u:list, s:list):
    return s.index(u[0])  # 返回u中第一个元素在s中的索引位置
uu.sort(key=lambda x: takeindex(x, s))#利用索引排序，先归结得到的子句放置在前
for i in uu:#按照输出限制打印输出
    par = i[1]
    ass = i[2]
    ss = i[0]
    index1 = reindex[par[0]]
    index2 = reindex[par[2]]
    ind1 = chr(par[1] + 97) if len(s[par[0]]) > 1 else '' 
    ind2 = chr(par[3] + 97) if len(s[par[2]]) > 1 else ''
    print('R[',index1,ind1,',',index2,ind2,']',sep = '',end = '')
    for j in ass:
        if len(j) != 0:
            print('(',j[1],'=',j[0],')',sep = '',end = '')
    print(' =',end = ' ')
    if len(ss) == 0: 
        print("[]")#归结结束打印空
    for j in ss:
        for k in range(len(j)):
            if k == 0:
                print(j[0],'(',sep='',end = '')
            elif k == len(j)-1:
                print(j[k],')',sep='',end = ',')
            else:
                print(j[k],',',sep='',end = '')
    print()
input("请按下任意键继续...")
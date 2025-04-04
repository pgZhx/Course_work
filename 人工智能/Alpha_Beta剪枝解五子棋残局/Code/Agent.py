global EMPTY
EMPTY = -1
global ROWS
ROWS = 15
alpha = float('-inf')
beta = float('inf')

def Search(board,isblack):
    # 目前 AI 的行为是随机落子，请实现 AlphaBetaSearch 函数后注释掉现在的 return
    # 语句，让函数调用你实现的 alpha-beta 剪枝
    matrix = [[0 for _ in range(15)] for _ in range(15)]
    for i in range(15):
        for j in range(15):
            if board[i][j] != EMPTY:
                 matrix[i][j] = 1
    return AlphaBetaSearch(board,isblack,alpha,beta,0,2,matrix)
def AlphaBetaSearch(board, isblack, alpha, beta, depth, max_depth,matrix):
    # 如果达到搜索深度上限或者棋盘已满，则返回评估值
    if depth == max_depth or is_board_full(board):
        return -1, -1, evaluate(board),matrix
    # 初始化最佳落子位置和评分
    best_move = (-1, -1)
    best_score = float('-inf') if isblack else float('inf')
    # 生成所有可能的后继状态
    for x, y, next_board in get_successors(board, 1 if isblack else 0):
        # 递归调用 AlphaBetaSearch
        _, _, move_score,_ = AlphaBetaSearch(next_board, not isblack, alpha, beta, depth + 1, max_depth,matrix)
        # 如果当前评分更好，更新最佳落子位置和评分
        if isblack and move_score > best_score:
            best_score = move_score
            best_move = (x, y)
            alpha = max(alpha, best_score)
            # matrix[x][y] = best_score
            if alpha >= beta:  # 剪枝条件
                break
        elif not isblack and move_score < best_score:
            best_score = move_score
            best_move = (x, y)
            beta = min(beta, best_score)
            # matrix[x][y] = best_score
            if beta <= alpha:  # 剪枝条件
                break
    matrix[best_move[0]][best_move[1]] = best_score
    return best_move[0], best_move[1], best_score,matrix
# 你可能还需要定义评价函数或者别的什么
# =============你的代码=============
# 辅助函数 - 检查棋盘是否已满
def is_board_full(board):
    for row in board:
        if EMPTY in row:
            return False
    return True
value = [[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
         [0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0],
         [0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0],
         [0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0],
         [0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0],
         [0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0],
         [0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0],
         [0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0],
         [0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0],
         [0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0],
         [0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0],
         [0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0],
         [0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0],
         [0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0],
         [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]]
#获取位置权值
def get_value(coordinate):
    x, y = coordinate[0], coordinate[1]
    return value[x][y]
#评价函数
def generate_pattern(color):
    p1 = (-1,color,color,-1)
    p2 = (-1,color,-1,color,-1)
    p3 = (color,color,-1,color,-1)
    p4 = (color,color,color,-1)
    p5 = (-1,color,color,color)
    p6 = (-1,color,color,color,-1)
    p7 = (-1,color,-1,color,color,-1)
    p8 = (-1,color,color,-1,color,-1)
    p9 = (color,color,color,-1,color,color)
    p10 = (color,color,-1,color,color)
    p11 = (color,-1,color,color,color)
    p12 = (color,color,color,color,-1)
    p13 = (-1,color,color,color,color)
    p14 = (-1,color,color,color,color,-1)
    p15 = (color,color,color,color,color)
    #字典的键为某种特殊棋型，值为一个元组，元组第一个元素为黑棋对应得分，第二个元素为白棋对应得分
    pattern = {p1:(50,30),p2:(50,30),p3:(200,100),p4:(500,300),p5:(500,300),
               p6:(5000,3000),p7:(5000,3000),p8:(5000,3000),p9:(6000,4000),p10:(6000,4000),
               p11:(6000,4000),p12:(6000,4000),p13:(6000,4000),p14:(100000,80000),p15:(999999,899999)}
    return pattern
def evaluate(board):
    color = 1
    other = 0
    # 对黑白棋各生成一套pattern
    pattern_c = generate_pattern(color)
    pattern_o = generate_pattern(other)
    Score = 0
    judge = 0 #用于判定特殊胜利条件
    #匹配黑棋各种特殊棋型并加分
    for pattern,score in pattern_c.items():
        if score[0] >= 3000:
            judge += 1
        Score += count_pattern(board,pattern) * score[0]
    #匹配白棋的各种特殊棋型并减分
    for pattern,score in pattern_o.items():
        if score[1] >= 3000:
            judge -= 1
        Score -= count_pattern(board,pattern) * score[1]
    # 位置权重评分
    for m in range(15):
        for n in range(15):
            if board[m][n] == color:
                Score += get_value((m,n))
            elif board[m][n] == other:
                Score -= get_value((m,n))
    if abs(judge) >= 2:
        return judge * 20000 + Score
    return Score
# 以下为编写搜索和评价函数时可能会用到的函数，请看情况使用、修改和优化
# =============辅助函数=============
#得到所有后继棋盘
#生成了所有可能的坐标和产生的棋盘，放在生成器里面
def get_successors(board, color, priority=get_value, EMPTY=-1):
    #以get_value作为搜索的优先级，实现从中间到四周的搜索顺序
    # 注意：生成器返回的所有 next_board 是同一个 list！
    from copy import deepcopy
    next_board = deepcopy(board)
    idx_list = [(x, y) for x in range(15) for y in range(15)]
    idx_list.sort(key=priority, reverse=True)
    for x, y in idx_list:
        if board[x][y] == EMPTY:
            next_board[x][y] = color
            yield (x, y, next_board) # 生成器
            next_board[x][y] = EMPTY
def get_pattern_locations(board, pattern):
    '''
    获取给定的棋子排列所在的位置
    ---------------参数---------------
    board       当前的局面，是 15×15 的二维 list，表示棋盘
    pattern     代表需要找的排列的 tuple
    ---------------返回---------------
    一个由 tuple 组成的 list，每个 tuple 代表在棋盘中找到的一个棋子排列
        tuple 的第 0 维     棋子排列的初始 x 坐标（行数/第一维）
        tuple 的第 1 维     棋子排列的初始 y 坐标（列数/第二维）
        tuple 的第 2 维     棋子排列的方向，0 为向下，1 为向右，2 为右下，3 为左下；    6 4 7
                            仅对不对称排列：4 为向上，5 为向左，6 为左上，7 为右上；   5   1
                            仅对长度为 1 的排列: 方向默认为0                      3 0 2
    '''
    ROWS = len(board)
    DIRE = [(1, 0), (0, 1), (1, 1), (1, -1)]
    pattern_list = []
    palindrome = True if tuple(reversed(pattern)) == pattern else False
    for x in range(ROWS):
        for y in range(ROWS):
            if pattern[0] == board[x][y]:
                if len(pattern) == 1:
                    pattern_list.append((x, y, 0))
                else:
                    for dire_flag, dire in enumerate(DIRE):
                        if _check_pattern(board, ROWS, x, y, pattern, dire[0], dire[1]):
                            pattern_list.append((x, y, dire_flag))
                    if not palindrome:
                        for dire_flag, dire in enumerate(DIRE):
                            if _check_pattern(board, ROWS, x, y, pattern, -dire[0], -dire[1]):
                                pattern_list.append((x, y, dire_flag + 4))
    return pattern_list
#检查是否匹配某种pattern
def _check_pattern(board, ROWS, x, y, pattern, dx, dy):
    for goal in pattern[1:]:#第一个元素为起始位置棋子颜色
        x, y = x + dx, y + dy
        if x < 0 or y < 0 or x >= ROWS or y >= ROWS or board[x][y] != goal:
            return False
    return True
#某个pattern的数量
def count_pattern(board, pattern):
    # 获取给定的棋子排列的个数
    return len(get_pattern_locations(board, pattern))
#判当前color是否胜
def is_win(board, color, EMPTY=-1):
    # 检查在当前 board 中 color 是否胜利
    pattern1 = (color, color, color, color, color)          # 检查五子相连
    # pattern2 = (EMPTY, color, color, color, color, EMPTY)   # 检查「活四」
    return count_pattern(board, pattern1)# + count_pattern(board, pattern2)


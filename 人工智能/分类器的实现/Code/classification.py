from sklearn.feature_extraction.text import TfidfVectorizer
import numpy as np

def preprocess(text):
    # Basic preprocessing to lowercase text
    return text.lower()
#将文本数据转化为小写字母

# Read and preprocess data
with open('train.txt', 'r') as file:
    lines = file.readlines()
    train_data = [preprocess(line.strip().split(' ', 2)[-1]) for line in lines]
    #存储训练数据
    labels = [line.strip().split(' ')[1] for line in lines]
    #存储感情标签，用于训练分类器
with open('test.txt', 'r') as file:
    lines = file.readlines()
    test_data = [preprocess(line.strip().split(' ', 3)[-1]) for line in lines]
    #存储测试数据
    test_labels = [line.strip().split(' ')[1] for line in lines]
    #存储测试数据的情感标签，用于比对预测结果是否正确

#使用TdidfVectorizer将文本数据为TF-IDF特征矩阵
vectorizer = TfidfVectorizer(ngram_range=(1, 2), max_features=500)
#ngram_range=(1, 2) 表示考虑从单个词（unigram）到两个词（bigram）的n-gram
#max_features=500 表示选择最多的特征数为500。
X_train = vectorizer.fit_transform(train_data).toarray()
X_test = vectorizer.transform(test_data).toarray()
# X_train 是转换后的训练数据特征矩阵。
# X_test 是转换后的测试数据特征矩阵

#K-NN情感分类器
def knn_predict(X_test, k=5):
    num_correct = 0 #计算正确预测的次数
    for i, test_vector in enumerate(X_test):
        distances = np.linalg.norm(X_train - test_vector, axis=1)
        # 计算训练集与测试向量之间的欧氏距离。
        nearest_indices = np.argsort(distances)[:k]
        #获取最近k个训练样本的索引
        nearest_labels = [labels[idx] for idx in nearest_indices]
        #存储这些的邻居的标签
        predicted_label = max(set(nearest_labels), key=nearest_labels.count)
        #根据多数投票原则预测的标签
        if predicted_label == test_labels[i]:
            print('第',i+1,'组数据，结果正确',"预测标签为",predicted_label,'正确标签为',test_labels[i],'数据为',test_data[i])
            num_correct += 1
        else: print('第',i+1,'组数据，结果错误',"预测标签为",predicted_label,'正确标签为',test_labels[i],'数据为',test_data[i])
    return num_correct / len(X_test)
# Evaluate model
accuracy = knn_predict(X_test,7)
print(f'正确率: {accuracy * 100:.2f}%')
input("按任意键继续...")
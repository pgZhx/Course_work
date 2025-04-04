import torch
import torch.nn as nn
import torch.optim as optim
from torch.utils.data import DataLoader, Dataset
from sklearn.metrics import accuracy_score
from nltk.tokenize import word_tokenize
from gensim.models import Word2Vec
import numpy as np
from tqdm import tqdm
import re
import pandas as pd


# 数据预处理和词嵌入部分同上...

# 定义带有Attention机制的LSTM模型
class LSTMWithAttention(nn.Module):
    def __init__(self, input_size, hidden_size, num_layers, num_classes):
        super(LSTMWithAttention, self).__init__()
        self.lstm = nn.LSTM(input_size, hidden_size, num_layers, batch_first=True)
        self.attention = nn.Linear(hidden_size, 1)
        self.fc = nn.Linear(hidden_size, num_classes)

    def forward(self, x):
        h0 = torch.zeros(num_layers, x.size(0), hidden_size).to(device)
        c0 = torch.zeros(num_layers, x.size(0), hidden_size).to(device)

        out, _ = self.lstm(x, (h0, c0))

        # Apply attention
        attn_weights = torch.softmax(self.attention(out), dim=1)
        attn_applied = torch.bmm(attn_weights.transpose(1, 2), out)

        out = self.fc(attn_applied.squeeze(1))
        return out


# 定义设备、超参数等
device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
input_size = 300
hidden_size = 128
num_layers = 2
num_classes = 2
num_epochs = 10
learning_rate = 0.001

model = LSTMWithAttention(input_size, hidden_size, num_layers, num_classes).to(device)
criterion = nn.CrossEntropyLoss()
optimizer = optim.Adam(model.parameters(), lr=learning_rate)
scheduler = optim.lr_scheduler.StepLR(optimizer, step_size=5, gamma=0.5)  # 每5个epoch后学习率减半

# 模型训练和评估部分同上...

# 创建数据加载器
class QNLIDataset(Dataset):
    def __init__(self, data, word2vec, max_len):
        self.data = data
        self.word2vec = word2vec
        self.max_len = max_len

    def __len__(self):
        return len(self.data)

    def __getitem__(self, idx):
        row = self.data.iloc[idx]
        premise = self.process_sentence(row['sentence'])
        hypothesis = self.process_sentence(row['question'])
        label = 1 if row['label'] == 'entailment' else 0
        return premise, hypothesis, label

    def process_sentence(self, sentence):
        tokens = word_tokenize(sentence.lower())
        vecs = [self.word2vec.wv[token] if token in self.word2vec.wv else np.zeros(300) for token in tokens]
        vecs = vecs[:self.max_len] + [np.zeros(300)] * (self.max_len - len(vecs))
        return np.array(vecs)


# 训练Word2Vec模型
train_data = pd.read_csv('C:\\Users\\26618\\Desktop\\人工智能实验\\实验9\\QNLI\\train_40_clean.tsv', sep='\t',
                         names=["index", "sentence", "question", "label"], on_bad_lines='skip')
dev_data = pd.read_csv('C:\\Users\\26618\\Desktop\\人工智能实验\\实验9\\QNLI\\dev_40_clean.tsv', sep='\t',
                       names=["index", "sentence", "question", "label"], on_bad_lines='skip')

sentences = [word_tokenize(sentence.lower()) for sentence in train_data['sentence']] + \
            [word_tokenize(sentence.lower()) for sentence in train_data['question']]
word2vec = Word2Vec(sentences, vector_size=300, window=5, min_count=1, workers=4)

max_len = 50
train_dataset = QNLIDataset(train_data, word2vec, max_len)
dev_dataset = QNLIDataset(dev_data, word2vec, max_len)

train_loader = DataLoader(train_dataset, batch_size=32, shuffle=True)
dev_loader = DataLoader(dev_dataset, batch_size=32, shuffle=False)

# 训练模型
for epoch in range(num_epochs):
    model.train()
    for premises, hypotheses, labels in tqdm(train_loader):
        premises = premises.clone().detach().float().to(device)
        hypotheses = hypotheses.clone().detach().float().to(device)
        labels = labels.clone().detach().long().to(device)

        outputs = model(torch.cat((premises, hypotheses), dim=1))
        loss = criterion(outputs, labels)

        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

    model.eval()
    all_preds = []
    all_labels = []
    with torch.no_grad():
        for premises, hypotheses, labels in dev_loader:
            premises = premises.clone().detach().float().to(device)
            hypotheses = hypotheses.clone().detach().float().to(device)
            labels = labels.clone().detach().long().to(device)

            outputs = model(torch.cat((premises, hypotheses), dim=1))
            _, predicted = torch.max(outputs.data, 1)

            all_preds.extend(predicted.cpu().numpy())
            all_labels.extend(labels.cpu().numpy())

    accuracy = accuracy_score(all_labels, all_preds)
    print(f'Epoch [{epoch + 1}/{num_epochs}], Accuracy: {accuracy:.2f}')

print("Training complete!")

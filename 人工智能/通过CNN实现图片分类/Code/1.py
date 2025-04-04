import torch
from torchvision import datasets, transforms
from torch.utils.data import DataLoader
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F
import matplotlib.pyplot as plt
# 定义数据预处理
transform = transforms.Compose([
    transforms.Resize((224, 224)),  # 调整图片大小为224x224
    transforms.ToTensor(),  # 转换为Tensor
    transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])  # 归一化
])
# 假设训练数据和测试数据的文件夹路径分别为：
train_data_dir = 'C:\\Users\\26618\\Desktop\\人工智能实验\\实验7\\cnn_data\\train'
test_data_dir = 'C:\\Users\\26618\\Desktop\\人工智能实验\\实验7\\cnn_data\\test'
# test_data_dir = 'C:\\Users\\26618\\Desktop\\人工智能实验\\实验7\\cnn_data\\test1\\test'
# 加载训练数据集
train_dataset = datasets.ImageFolder(root=train_data_dir, transform=transform)
# 创建训练数据加载器
train_loader = DataLoader(dataset=train_dataset, batch_size=32, shuffle=True)
# 加载测试数据集
test_dataset = datasets.ImageFolder(root=test_data_dir, transform=transform)
# 创建测试数据加载器
test_loader = DataLoader(dataset=test_dataset, batch_size=32, shuffle=False)
class SimpleCNN(nn.Module):
    def __init__(self, num_classes=5):
        super(SimpleCNN, self).__init__()
        # 定义第一个卷积层，修改in_channels为3以处理RGB图片
        self.conv1 = nn.Sequential(
            nn.Conv2d(3, 16, kernel_size=5, stride=1, padding=2),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=2),
        )
        # 第二个卷积层
        self.conv2 = nn.Sequential(
            nn.Conv2d(16, 32, kernel_size=5, stride=1, padding=2),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=2),
        )
        # 假设经过conv1和pool1后，特征图变为 (224-5+2*2)/2+1 = 112
        # 经过conv2和pool2后，特征图变为 (112-5+2*2)/2+1 = 56
        # 因此，全连接层的输入特征数为32*56*56
        self.fc = nn.Linear(32*56*56, num_classes)

    def forward(self, x):
        x = self.conv1(x)
        x = self.conv2(x)
        # 展平操作
        x = x.view(x.size(0), -1)
        x = self.fc(x)
        return x

def calculate_accuracy(model, test_loader):
    model.eval()
    correct = 0
    total = 0
    with torch.no_grad():
        for images, labels in test_loader:
            images = images.to(device)
            labels = labels.to(device)
            outputs = model(images)
            _, predicted = torch.max(outputs.data, 1)
            total += labels.size(0)
            correct += (predicted == labels).sum().item()
            # print("真实值：",labels,"预测值：",predicted)
    accuracy = 100 * correct / total
    return accuracy


# 实例化模型
num_classes = 5  # 假设有10个类别
model = SimpleCNN(num_classes)
#确定使用的设备，并将模型移到设备上
device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
model.to(device)

lr = 0.01
# 定义损失函数和优化器
criterion = nn.CrossEntropyLoss()
optimizer = optim.SGD(model.parameters(),lr=lr)
# 训练模型
num_epochs = 10
losses, accuracies = [], []
for epoch in range(num_epochs):
    model.train()  # 设置模型为训练模式
    running_loss = 0.0
    for images, labels in train_loader:
        images = images.to(device)
        labels = labels.to(device)
        # 清除优化器的梯度
        optimizer.zero_grad()
        # 前向传播
        outputs = model(images)
        # 计算损失
        loss = criterion(outputs, labels)
        # 反向传播和优化
        loss.backward()
        optimizer.step()
        running_loss += loss.item()
    # 计算epoch的平均损失
    epoch_loss = running_loss / len(train_loader)
    losses.append(epoch_loss)
    # 在测试集上计算准确率
    accuracy = calculate_accuracy(model, test_loader)
    accuracies.append(accuracy)
    print(f'Epoch [{epoch + 1}/{num_epochs}], Loss: {epoch_loss:.4f}, Accuracy: {accuracy:.2f}%')
# 绘制训练损失和测试准确率曲线
plt.figure(figsize=(12, 4))
plt.subplot(1, 2, 1)
plt.plot(losses, label='Training Loss')
plt.xlabel('Epoch')
plt.ylabel('Loss')
plt.title('Training Loss Over Epochs')
plt.legend()
plt.subplot(1, 2, 2)
plt.plot(accuracies, label='Test Accuracy')
plt.xlabel('Epoch')
plt.ylabel('Accuracy (%)')
plt.ylim(0,110)
plt.title('Test Accuracy Over Epochs')
plt.legend()
plt.tight_layout()
plt.show()
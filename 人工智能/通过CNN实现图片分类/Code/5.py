import torch
from torchvision import datasets, transforms
from torch.utils.data import DataLoader
import torch.nn as nn
import torch.optim as optim
import matplotlib.pyplot as plt

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
        self.droppout = nn.Dropout(0.1)
    def forward(self, x):
        x = self.conv1(x)
        x = self.conv2(x)
        # 展平操作
        x = x.view(x.size(0), -1)
        x = self.droppout(x)
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
num_classes = 5  # 假设有5个类别
model = SimpleCNN(num_classes)
device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

model.to(device)

# 定义数据预处理
transform1 = transforms.Compose([
    transforms.Resize((224, 224)),  # 调整图片大小为224x224
    transforms.RandomRotation(10), #正负45度的随机旋转
    transforms.RandomHorizontalFlip(p = 0.5),#随机的水平翻转
    transforms.RandomVerticalFlip(p = 0.5),#随机的垂直翻转
    transforms.ToTensor(),  # 转换为Tensor
    transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])  # 归一化
])
transform2 = transforms.Compose([
    transforms.Resize((224, 224)),  # 调整图片大小为224x224
    transforms.ToTensor(),  # 转换为Tensor
    transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])  # 归一化
])
# 假设训练数据和测试数据的文件夹路径分别为：
train_data_dir = 'C:\\Users\\26618\\Desktop\\人工智能实验\\实验7\\cnn_data\\train'
# test_data_dir = 'C:\\Users\\26618\\Desktop\\人工智能实验\\实验7\\cnn_data\\test1\\test'
test_data_dir = 'C:\\Users\\26618\\Desktop\\人工智能实验\\实验7\\cnn_data\\test'

# 加载训练数据集
train_dataset = datasets.ImageFolder(root=train_data_dir, transform=transform1)
# 创建训练数据加载器
train_loader = DataLoader(dataset=train_dataset, batch_size=32, shuffle=True)
# 加载测试数据集
test_dataset = datasets.ImageFolder(root=test_data_dir, transform=transform2)
# 创建测试数据加载器
test_loader = DataLoader(dataset=test_dataset, batch_size=32, shuffle=False)


# 定义损失函数和优化器
criterion = nn.CrossEntropyLoss()
# 训练模型
num_epochs = 10
losses, accuracies = [], []
optimizer = optim.Adam(model.parameters(), lr=0.001)
scheduler = optim.lr_scheduler.StepLR(optimizer, step_size=7, gamma=0.1)
for epoch in range(num_epochs):
    model.train()
    running_loss = 0.0
    for images, labels in train_loader:
        images = images.to(device)
        labels = labels.to(device)
        optimizer.zero_grad()
        outputs = model(images)
        loss = criterion(outputs, labels)
        loss.backward()
        optimizer.step()
        running_loss += loss.item()
    scheduler.step()
    epoch_loss = running_loss / len(train_loader)
    losses.append(epoch_loss)
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
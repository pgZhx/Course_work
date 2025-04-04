import collections
import gym
import argparse
import numpy as np
import torch
import torch.nn.functional as F
from torch import nn, optim
import random
import matplotlib.pyplot as plt


class QNet(nn.Module):
    def __init__(self, input_size, hidden_size, output_size):
        super(QNet, self).__init__()
        self.fc1 = nn.Linear(input_size, hidden_size)
        self.fc2 = nn.Linear(hidden_size, output_size)
        #两层全连接层，参数为输入输出的大小
    def forward(self, x):
        x = torch.Tensor(x)
        x = F.relu(self.fc1(x))#激活函数，小于0时输出0，大于0时输出原本的值
        x = self.fc2(x)
        return x

class ReplayBuffer:#经验回放缓冲区
    def __init__(self, capacity):
        self.buffer = collections.deque(maxlen=capacity)
    #capacity为缓冲区的最大容量，存储的最大转换数量
    #buffer是用collection.deque创建的双端队列，新元素加入时如果队满，将会自动删除最早加入的元素
    def len(self):
        return len(self.buffer)
    def push(self, *transition):
        self.buffer.append(transition)
    #将新的transition转入到缓冲区的末端
    def sample(self, batch_size):
        transitions = random.sample(self.buffer, batch_size)
        obs, actions, rewards, next_obs, dones = zip(*transitions)
        return np.array(obs), actions, rewards, np.array(next_obs), dones
    #从缓冲区中随机采样batch_size个transition，并最终转化为Numpy数组
    def clean(self):
        self.buffer.clear()
    #清空缓冲区中的所有元素
class DQN:
    def __init__(self, env, input_size, hidden_size, output_size):
        self.env = env
        self.eval_net = QNet(input_size, hidden_size, output_size) #用于价值评估和动作选择
        self.target_net = QNet(input_size, hidden_size, output_size)#目标网络，定期从eval_net中复制权重
        self.optim = optim.Adam(self.eval_net.parameters(), lr=args.lr) #使用Adam优化器
        self.gamma = args.gamma #折扣因子
        self.buffer = ReplayBuffer(args.capacity)
        self.loss_fn = nn.MSELoss() #损失函数
        self.learn_step = 0 #计数器，记录更新次数
        self.eps_decay = args.eps_decay
        self.eps = args.eps  # 初始探索率
        self.eps_min = args.eps_min  # 最小探索率
    def decay_eps(self):
        # 衰减探索率ε的值
        if self.eps > self.eps_min:
            self.eps *= self.eps_decay
            self.eps = max(self.eps, self.eps_min)
    def choose_action(self, obs):#动作选择函数
        self.decay_eps()  # 衰减ε值
        if random.random() > self.eps:
            with torch.no_grad():
                return self.eval_net(obs).argmax().item()
        else:
            return self.env.action_space.sample()
        #if-else语句的解释：有ε的可能性随机探索动作，1-ε可能性会计算Q值后选取最佳动作

    def store_transition(self, *transition):
        self.buffer.push(*transition)
        #存储transition到经验回放缓冲区
    def learn(self):#训练网络
        if self.buffer.len() < args.batch_size:
            return
        obs, actions, rewards, next_obs, dones = self.buffer.sample(args.batch_size)
        obs = torch.FloatTensor(obs)
        actions = torch.LongTensor(actions)
        rewards = torch.FloatTensor(rewards)
        next_obs = torch.FloatTensor(next_obs)
        dones = torch.FloatTensor(dones)
        #采样数据并转换为torch张量
        q_eval = self.eval_net(obs).gather(1, actions.view(-1, 1)).squeeze(1)
        #计算当前状态Q值
        q_next = self.target_net(next_obs).max(1)[0].detach()
        q_target = rewards + self.gamma * q_next * (1 - dones)
        #计算目标Q值
        loss = self.loss_fn(q_eval, q_target)
        self.optim.zero_grad()
        loss.backward()
        self.optim.step()
        #计算损失并反向传播
        if self.learn_step % args.update_target == 0:
            self.target_net.load_state_dict(self.eval_net.state_dict())
            #定期用eval网络的参数来更新target网络的参数
        self.learn_step += 1


def main():
    env = gym.make(args.env)
    o_dim = env.observation_space.shape[0] #环境的观察空间维度
    a_dim = env.action_space.n #动作空间的数量
    agent = DQN(env, o_dim, args.hidden, a_dim) #初始化智能体
    episode_rewards = []
    average_rewards = []
    #两个用于存储回报的列表
    for i_episode in range(args.n_episodes):#对于每个训练周期
        obs = env.reset() #重置环境
        episode_reward = 0
        done = False #该轮是否结束的标志
        step_cnt = 0 #记录该轮步数
        while not done and step_cnt < 500:
            step_cnt += 1 #每做一个动作步数会自增，超过500时游戏胜利
            env.render() #渲染环境
            action = agent.choose_action(obs) #选择动作
            next_obs, reward, done, info = env.step(action)
            agent.store_transition(obs, action, reward, next_obs, done)
            #将信息存储到经验回放缓冲区
            episode_reward += reward
            obs = next_obs #更新reward和obs
            if agent.buffer.len() >= args.batch_size:
                agent.learn()
                #经验回放区中数据足够时进行学习
        episode_rewards.append(episode_reward)
        if len(episode_rewards) >= 100:
            average_reward = np.mean(episode_rewards[-100:])
            average_rewards.append(average_reward)
        else:
            average_rewards.append(np.mean(episode_rewards))
        #计算每一百局内的平均reward
        print(f"Episode: {i_episode}, Reward: {episode_reward}, Average Reward (last 100): {average_rewards[-1]}")
        #每次训练周期结束后打印信息
    # 绘制奖励曲线
    plt.figure(figsize=(12, 5))
    plt.subplot(121)
    plt.plot(episode_rewards, label='Episode Reward')
    plt.xlabel('Episode')
    plt.ylabel('Reward')
    plt.legend()

    plt.subplot(122)
    plt.plot(average_rewards, label='Average Reward (last 100 episodes)')
    plt.xlabel('Episode')
    plt.ylabel('Average Reward')
    plt.legend()

    plt.tight_layout()
    plt.show()



if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--env",        default="CartPole-v1",  type=str,   help="environment name")
    parser.add_argument("--lr",             default=1e-3,       type=float, help="learning rate")
    parser.add_argument("--hidden",         default=64,         type=int,   help="dimension of hidden layer")
    parser.add_argument("--n_episodes",     default=500,        type=int,   help="number of episodes")
    parser.add_argument("--gamma",          default=0.99,       type=float, help="discount factor")
    # parser.add_argument("--log_freq",       default=100,        type=int)
    parser.add_argument("--capacity",       default=10000,      type=int,   help="capacity of replay buffer")
    parser.add_argument("--eps",            default=0.2,        type=float, help="epsilon of ε-greedy")
    parser.add_argument("--eps_min",        default=0.001,       type=float)
    parser.add_argument("--batch_size",     default=128,        type=int)
    parser.add_argument("--eps_decay",      default=0.95,      type=float)
    parser.add_argument("--update_target",  default=100,        type=int,   help="frequency to update target network")
    args = parser.parse_args()
    #深度强化学习中用到的超参数
    main()
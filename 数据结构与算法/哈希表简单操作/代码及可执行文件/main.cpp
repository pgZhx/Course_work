#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib> // 添加此行
using namespace std;
class HashTable {
public:
    static const int TABLE_SIZE = 100;
    string table[TABLE_SIZE];
    int random[TABLE_SIZE];
    double ASL;
    HashTable():ASL(0){
        for(int i = 0;i < TABLE_SIZE;i++){
            table[i] = "";
        }
        for(int i = 0;i < TABLE_SIZE;i++){
            random[i] = randomStep();
        }
    }
    int randomStep() {
        return rand() % TABLE_SIZE;  // 生成 0 到 TABLE_SIZE-1 之间的随机数
    }
    int hash(string key) {
        int sum = 0;
        for (int i = 0; i < key.length(); i++) {
            sum += key[i]; // 将姓名转换成ASCII码并求和
        }
        return sum % TABLE_SIZE; // 使用除留余数法哈希函数
    }
    void insert(string key) {
        int index = hash(key);
        ASL++;
        int rindx = 0;
        while (table[index] != "") {
            //index = (index + randomStep()) % TABLE_SIZE; // 冲突处理采用伪随机探测再散列法
            index = (index + random[rindx++]) % TABLE_SIZE;
            ASL++;
        }
        table[index] = key;
    }
    int search(string key,int&cnt) {
        int index = hash(key);
        int count = 0;
        while (table[index] != key && count < TABLE_SIZE) {
            if(table[index] == "")return -1;
            index = (index + random[count]) % TABLE_SIZE; // 冲突处理采用伪随机探测再散列法
            count++;cnt++;
        }
        if (count == TABLE_SIZE) {
            return -1; // 未找到
        }
        else {
            return index; // 返回找到的位置
        }
    }

};
int main() {
    system("chcp 65001");
    srand(time(0));  // 设置随机数种子
    HashTable ht;
    ifstream file("input.txt");
    if (file.is_open()) {
        for (int i = 0; i < 30; i++) {
            string NAME;
            file >> NAME;
            ht.insert(NAME);
        }
        file.close();
    }
    else {
        cout << "Failed to open the file." << endl;
    }
    while(1){
        system("cls");
        cin.clear();
        cin.sync();
        char choice;
        cout << "功能选单" <<endl;
        cout << "0.退出程序" <<endl;
        cout << "1.查看平均查找长度" <<endl;
        cout << "2.哈希表查找" <<endl;
        cin >> choice;
        switch (choice) {
            case '0' :return 0;
            case '1' :{
                cout << "哈希表的平均查找长度为: " << ht.ASL / 30 << endl;
                system("pause");
                break;
            }
            case '2' :{
                string searchName;
                cout << "请输入要搜索的姓名："<< endl;
                cin >> searchName;
                int count = 1;
                int result = ht.search(searchName,count);
                if (result != -1) {
                    cout << "姓名 " << searchName << " 在哈希表中的下标为： " << result << endl;
                    cout << "查找的次数为： "<< count << endl;
                } else {
                    cout << "姓名 " << searchName << " 不存在于哈希表中" << endl;
                    cout << "查找的次数为： "<< count << endl;
                }
                system("pause");
                break;
            }
            default:{ cout<< "非法输入"<< endl;system("pause");break;}
        }
    }
}
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include<algorithm>
#include<string>
#include <set>
#include<vector>
#include<conio.h>
#include<limits>
#include<iomanip>
#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif
using namespace std;
void pressAnyKeyToContinue() {
    cout << "Press any key to continue..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // 忽略剩余的字符，直到遇到换行符
    _getch(); // 使用_getch()函数等待用户按下任意键 // 等待用户按下任意键
}
bool cmp(pair<string,int> a,pair<string,int> b){//用于比较词频
    return a.second > b.second;
}
void print(map<string,int>& wordCount,map<string,set<int>>& wordLines,int K,int total){
    vector<pair<string,int>> vec;
    int num = 0;
    for(auto it = wordCount.begin();it != wordCount.end();it++){
        vec.push_back(pair<string,int>(it -> first,it -> second));
        num++;
    }
    sort(vec.begin(),vec.end(),cmp);
    if(num <= K) {cout << "这是一个无效的K值" << endl;return;}
    int i = 1; int x;int n = 0;
    for(auto& it : vec){
        if(i <= K){
            if(i == K) x = it.second;
            cout << "Top " << i << " word: "<< setw(5)<<it.first<< " | Count: " << it.second <<" | Line Numbers: ";
            for (const auto& line : wordLines[it.first]) {
                cout << line << " ";
            }
            n++;
            cout << endl;
            i++;
        }        
        else{
            if(it.second < x) break;
            else{
                cout << "Top " << i-1 << " word: "<< setw(5)<<it.first<< " | Count: " << it.second <<" | Line Numbers: ";
                for (const auto& line : wordLines[it.first]) {
                    cout << line << " ";
                }
                cout << endl;
                n++; 
            }
        }        
    }
    cout << "如下图所示："<< endl;
    for(auto& it : vec){
        n--;
        cout << setw(15) << it.first << " | ";
        int f = ((double )it.second / total) * 200;
        while(f--){
            cout << "*";
        }
        cout << endl;
        if(n == 0) break;
    }
}
void txt(map<string,int>& wordCount,map<string,set<int>>& wordLines,int K){//统计英语短文词频TOP-K单词词频
    ifstream file("input.txt");
    if (!file.is_open()) {
        cout << "Fail to open file." << endl;//检查文件是否正常打开
        return ;
    }
    string line;
    int lineNumber = 1;int total = 0;
    while(getline(file,line)){
    stringstream str(line);
    string word;
    int judge = 0;
    while(str >> word){
        if(!judge)  {word[0] += 32;judge++;}
        int len = word.length();
        if(word[len-1] < 97||word[len-1] > 122){
            if(word[len-1] == '.') judge = 0;
            word = word.substr(0,len-1);//去除单词的标点符号
        }        
        wordCount[word]++;//统计词频
        total++;
        wordLines[word].insert(lineNumber);//当前对应行数加入       
    }
    lineNumber++;
   }
    cout << "本文的总词数是："<< total << " 本文的总行数是"<< lineNumber <<endl;
    file.close();
    print(wordCount,wordLines,K,total);
}
void getnext(int*next,string s){//得到next数组
    int i = 1,j = 0,len = s.size();
    next[0] = 0;
    for(;i < len;i++){
        while(s[i] != s[j] && j>0) j = next[j-1];
        if(s[i] == s[j]){
            j++;
            next[i] = j;
        }
    }
}
void cpp(map<string,int>& wordCount,map<string,set<int>>& wordLines,int K){//统计c++代码中的保留字符集的单词词频
    ifstream file("input.cpp");
    if (!file.is_open()) {
        cout << "Fail to open file." << endl;//检查文件是否正常打开
        return;
    }
    string line;
    string storestr[9] = {"if","else","for","while","return","void","int","char","struct"};
    int linenumber = 1;int total = 0;
    while(getline(file,line)){
        for(int i = 0;i < 9;i++){
            string s = storestr[i];
            int n = s.length();
            int next[n] = {0};
            getnext(next,s);
            int len = line.size();
            int m = 0,j = 0;
            while(m < len){    
                if(j == 0 || s[j] == line[m]) {m++;j++;}
                else j = next[j-1];        
                if(j >= n) {
                    wordCount[s]++;total++;
                    wordLines[s].insert(linenumber);
                    j = 0;}        
            }
            
        }
        linenumber++;
    }
    file.close();
        cout  << "该代码的总行数是"<< linenumber <<endl;
    print(wordCount,wordLines,K,total);
}
int main() {
    system("chcp 65001");
    do{
        system(CLEAR_SCREEN); // 清屏
        cout << "--请输入你希望识别的文件的格式类型(键入0以退出程序)--"<< endl;
        cout <<"---1.txt文件--------------------------------------"<< endl;
        cout <<"---2.cpp文件--------------------------------------"<< endl;
        int choice;
        cin >> choice;
        if(choice == 0) break;
        if(choice == 1){
            int K;
            cout << "输入K以显示txt文件中词频为Top K的单词" << endl;
            cin >> K;
            map<string, int> wordCount;//string记录不重复的单词，int记录词频
            map<string, set<int>> wordLines;//string是单词，set存储所在的行
            txt(wordCount,wordLines,K);
            cout << "请输入你想获取词频的单词" <<  endl;
            string s;
            cin >> s;
            if(wordCount[s] != 0)cout << s << "的词频是"<< wordCount[s] << endl;
            else cout << s << "不在文件中出现过" << endl; 
            pressAnyKeyToContinue();       
        }
        if(choice == 2){
            int K;
            cout << "输入K以显示cpp文件中保留字符集中的词频为Top K的词汇" << endl;
            cout << "(保留字符集包含\"if\",\"else\",\"for\",\"while\",\"return\",\"void\",\"int\",\"char\",\"struct\")"<< endl;
            cin >> K;
            map<string, int> wordCount;//string记录不重复的单词，int记录词频
            map<string, set<int>> wordLines;//string是单词，set存储所在的行
            cpp(wordCount,wordLines,K);
            cout << "请输入你想获取词频的保留字符集的词汇" <<  endl;
            string s;
            cin >> s;
            if(wordCount[s] != 0)cout << s << "的词频是"<< wordCount[s] << endl;
            else cout << s << "不在文件中出现过" << endl;
            pressAnyKeyToContinue();
        }    
    }while(1);
    
    return 0;
} 
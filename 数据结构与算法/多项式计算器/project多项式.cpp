#define _CRTDBG_MAP_ALLOC
#include<iostream>
#include<unordered_set>
#include<math.h>
#include <stdlib.h>
#include <crtdbg.h>
#include<sstream>
#include<string>
#include<limits>
using namespace std;
struct Node
{
    double xi;//存放系数
    int zhi;//存放指数
    Node* next;
    Node(double a,int b):xi(a),zhi(b),next(nullptr){}
    Node():xi(0),zhi(0),next(nullptr){}
};
Node* Built(int n){//n为项数
    cout << "请输入多项式各项的系数和指数(示例 1 2 1 0表示x^2+1): "<< endl; 
    Node* head = new Node(0,0);
    head -> next = nullptr;
    while(n){
        double xi;
        int zhi;
        if(!(cin >> xi)){
            cerr << "非法输入！请重新输入" << endl;
            // cin.clear();
            // cin.sync();
            cin.ignore(numeric_limits<streamsize>::max(),'\n'); 
            Node* del = head -> next;
                while(del){
                    Node*x = del;
                    del = del -> next;
                    delete x;
                }
                return head;
        }
        if(!(cin >> zhi)){
            cerr << "非法输入！请重新输入" << endl;
            // cin.clear();
            // cin.sync();
            cin.ignore(numeric_limits<streamsize>::max(),'\n'); 
            Node* del = head -> next;
                while(del){
                    Node*x = del;
                    del = del -> next;
                    delete x;
                }
                return head;
        }
        Node* temp = new Node(xi,zhi);
        temp -> next = head -> next;
        head -> next = temp;
        n--;
    }
    return head;
}//新建多项式
void Hebin(Node* head){
    Node* temp = head ->next;
    Node* pre = head;
    unordered_set<int> s;
    while(temp){
        if(s.count(temp -> zhi) == 1){
            Node* add = head -> next;
            while(add != temp){
                if(add -> zhi == temp -> zhi) {
                    add -> xi += temp -> xi;
                    break;
                }
                add = add -> next;
            }
            add = temp;
            temp = temp -> next;
            pre -> next = temp;
            delete(add);
        }
        else{
            s.insert(temp -> zhi);
            pre = pre -> next;
            temp = temp -> next;
        }        
    }
}//合并指数相同的项
void Paixu(Node* head){
    Node* temp = head -> next;
    while(temp){
        Node* temp1 = temp -> next;
        while(temp1){
            if(temp1 -> zhi > temp -> zhi){
                double x;int z;
                z = temp -> zhi;
                x = temp -> xi;
                temp -> zhi = temp1 -> zhi;
                temp -> xi = temp1 -> xi;
                temp1 -> zhi = z;
                temp1 -> xi = x;
            }
            temp1 = temp1 -> next; 
        }
        temp = temp -> next;
    }
}//按指数大小降序排列
void Print(Node* head){
    Hebin(head);
    Paixu(head);
    if(head -> next == nullptr){
         cout << "空" << endl;
         return;
    }
    Node* temp = head -> next;
    Node*temp1 = head -> next;
    while(temp1){
        if(temp1 -> xi == 0) temp1 = temp1-> next;
        else break;
    }
    if(temp1 == nullptr){
        cout << 0 << endl;
        return;
        }//如果全部项都为0 则得输出0；
    else{
        while(temp){
            if(temp -> xi > 0 && temp != head -> next) cout << '+';
            else {}
        if(temp -> xi == 0) {
            temp = temp -> next;
            continue;
        }
        if(temp -> zhi == 0){
            cout << temp -> xi;     
        }//区分指数为0的情况
        else if(temp -> zhi == 1){
            if(temp-> xi == 1){
                cout << "x";
            }
            else if(temp -> xi == -1){
                cout << "-x";
            }
            else cout << temp -> xi << "x";
            
        }//指数为1的情况
        else if(temp -> xi == -1){
            cout << "-x^" <<temp -> zhi;
        }//区分系数为-1的情况
        else if(temp -> xi == 1){
            cout << "x^" <<temp -> zhi;
        }//区分系数为1的情况
        
        else{
            cout << temp -> xi << "x^" << temp -> zhi;
        }
        
        temp = temp -> next;
        }
        cout << endl;
    }
    
}//打印多项式
Node* Add(Node* head1,Node* head2){
    Node* ans = head1;
    Node* temp = head1;
    while(temp -> next){
        temp = temp -> next;
    }
    temp -> next = head2 -> next;
    //Node*del = head2;
    Hebin(ans);Paixu(ans);
    head2 -> next = ans -> next;
    return ans;
}
Node* Sub(Node* head1,Node* head2){
    Node* fu = new Node();//存系数为减数多项式的相反数的多项式
    Node* temp = head2 -> next;
    Node* sub = fu;
    while(temp){
        Node* x = new Node(-(temp -> xi),temp -> zhi);
        fu -> next = x;
        fu = fu -> next;
        temp = temp -> next;
    }
    Node* del = fu -> next;
    Node* ans = Add(head1,sub);
    while(del){
        Node* x = del;
        del = del -> next;
        delete x;
    }
    return ans;
}
Node* Mult(Node* head1,Node* head2){
    Node* a = new Node();
    Node* mult = a;
    Node*temp1 = head1 -> next;
    while(temp1){
        Node*temp2 = head2 -> next;
        while(temp2){
            Node* x = new Node((temp1 -> xi*temp2 -> xi),(temp1 -> zhi + temp2 -> zhi));
            a -> next = x;
            a = a -> next;
            temp2 = temp2 -> next;
        }
        temp1 = temp1 -> next;
    }
    a -> next = nullptr;
    Hebin(mult);Paixu(mult);
    return mult;
}
double Calcu(Node* head,double x){
    double sum = 0;
    Node* temp = head -> next;
    while(temp){
        sum += (temp -> xi) * pow(x,temp -> zhi);
        temp = temp -> next;
    }
    return sum;
}
Node* Deriv(Node* head){
    Hebin(head);Paixu(head);
    Node*temp = head -> next;
    Node* a = new Node();
    Node* ans = a; 
    while (temp)
    {   
        if(temp -> zhi != 0){
            int y = temp -> zhi - 1;
            Node* x = new Node(((temp -> xi)*(temp -> zhi)),y);
            a -> next = x;
            a = a -> next;
            temp = temp -> next;
        }
        else{
            temp = temp -> next;
        }
        a -> next = nullptr;
    }
    return ans;
}
int main(){
    system("chcp 65001");
    Node*Polynomial[1000];//计算器最大容纳的多项式数目
    int n = 0;//当前多项式的数目
    cout << "欢迎使用一元稀疏多项式计算器，请认真对照参考与示例进行输入" << endl;
    cout << "请注意输入的合法性以保护程序，谢谢" << endl;
    while(1){
        
        cout << "   -*****     -*****          Menu            -*****      -*****   " << endl;
        cout << "-*********************    -1.新建多项式      -********************* " << endl;
        cout << "  -*****************      -2.多项式赋值        -*****************   " << endl;
        cout << "    -*************        -3.多项式加法          -*************     " << endl;
        cout << "      -*********          -4.多项式减法            -*********       " << endl;
        cout << "        -*****            -5.多项式乘法              -*****         " << endl;
        cout << "         -***             -6.对多项式求导              ***          " << endl;
        cout << "          -*              -7.退出程序                  -*           " << endl;
        cout << "-------------------------------------------------------------------" << endl;
        int menu;
        // string input;
        // getline(cin,input);
        // stringstream ss(input);
        // if(ss >> menu && ss.eof()){}
        if(!(cin >> menu)){
            cout << "非法输入！请重新输入(键入回车以弹出菜单)" << endl;
            cin.clear();
            cin.sync();
            cin.ignore(numeric_limits<streamsize>::max(),'\n'); 
            continue;
        }
        else{
            cin.ignore(numeric_limits<streamsize>::max(),'\n'); 
        }
        switch(menu){
            case 1:{
                cout << "请输入您多项式的项数" << endl;
                int num;
                string input;
                getline(cin,input);
                stringstream ss(input);
                if(ss >> num && ss.eof()){}
                else{
                    cerr << "非法输入！创建失败(键入回车以弹出菜单)" << endl;
                    cin.clear();
                    cin.sync();
                    cin.ignore(numeric_limits<streamsize>::max(),'\n'); 
                    break;
                }
                if(num == 0){
                    Node* polynomial = new Node(0,0);
                    Node* zero = new Node(0,0);
                    polynomial -> next = zero;
                    zero -> next = nullptr;
                    Polynomial[n++] = polynomial;
                    cout << "这是您新建的多项式: 0" << endl; 
                    cin.ignore(numeric_limits<streamsize>::max(),'\n'); 
                    break;
                }
                else if(num < 0){
                    cerr << "非法输入！创建失败(键入回车以弹出菜单)" << endl;
                    cin.clear();
                    cin.sync();
                    cin.ignore(numeric_limits<streamsize>::max(),'\n'); 
                    break;
                }
                else{
                    Node* polynomial = Built(num);
                    Polynomial[n++] = polynomial;
                    if(polynomial -> next != nullptr){
                        cout << "这是您新建的多项式：" << endl; 
                        Print(polynomial);
                    }
                    
                    else cout << "创建失败，请重新输入！"<< endl;
                    
                    cin.ignore(numeric_limits<streamsize>::max(),'\n'); 
                    break;
                }
                
                // cin.clear();
                // cin.sync();
                
                break;
            }
            case 2:{
                if(n == 0) {cout << "非法输入！当前计算器内无多项式" << endl; break;}

                cout << "请选择你要赋值的多项式： " << endl;
                cout << "这是目前计算器内的所有多项式："<< endl;
                for(int i = 0;i < n;i++){
                    cout << i+1 << ". ";
                    Print(Polynomial[i]);
                }
                int m;
                cin >> m;
                Node* head = Polynomial[m-1];
                double val;
                cout << "输入你要为其赋的值： "<< endl;
                cin >> val;
                cout << "结果为：" << endl;
                cout << Calcu(head,val) << endl;
                cin.ignore(numeric_limits<streamsize>::max(),'\n'); 
                break;
            }
            case 3:{
                if(n == 0) {cout << "非法输入！当前计算器内无多项式" << endl; break;}
                cout << "请选择你要进行加法的两个多项式："<< endl;
                cout << "这是目前计算器内的所有多项式："<< endl;
                for(int i = 0;i < n;i++){
                    cout << i+1 << ". ";
                    Print(Polynomial[i]);
                }
                int m,p;
                cin >> m >> p;
                Node* head1 = Polynomial[m-1];
                Node* head2 = Polynomial[p-1];
                cout << "您的计算结果将会保留在原多项式"<< m <<"和"<< p <<"的位置，此操作将不可更改"<< endl;
                cout << "这是加法运算后的结果： "<< endl;
                Print(Add(head1,head2));
                cin.ignore(numeric_limits<streamsize>::max(),'\n'); 
                break;
            }
            case 4:{
                if(n == 0) {cout << "非法输入！当前计算器内无多项式" << endl; break;}
                cout << "请选择你要进行减法的两个多项式："<< endl;
                cout << "这是目前计算器内的所有多项式："<< endl;
                for(int i = 0;i < n;i++){
                    cout << i+1 << ". ";
                    Print(Polynomial[i]);
                }
                int m,p;
                cin >> m >> p;
                Node* head1 = Polynomial[m-1];
                Node* head2 = Polynomial[p-1];
                cout << "您的计算结果将会保留在原多项式"<< m <<"和"<<n <<"的位置，此操作将不可更改"<< endl;
                cout << "这是减法运算后的结果： "<< endl;
                Print(Sub(head1,head2));
                cin.ignore(numeric_limits<streamsize>::max(),'\n'); 
                break;
            }
            case 5:{
                if(n == 0) {cout << "非法输入！当前计算器内无多项式" << endl; break;}
                cout << "请选择你要进行乘法的两个多项式："<< endl;
                cout << "这是目前计算器内的所有多项式："<< endl;
                for(int i = 0;i < n;i++){
                    cout << i+1 << ". ";
                    Print(Polynomial[i]);
                }
                int m,p;
                cin >> m >> p;
                Node* head1 = Polynomial[m-1];
                Node* head2 = Polynomial[p-1];
                cout << "您的计算结果将会保留在原多项式"<< m << "的位置，此操作将不可更改"<< endl;
                cout << "这是乘法运算后的结果： "<< endl;
                Print(Mult(head1,head2));
                cin.ignore(numeric_limits<streamsize>::max(),'\n'); 
                break;
            }
            case 6:{
                if(n == 0) {cout << "非法输入！当前计算器内无多项式" << endl; break;}
                cout << "请选择你要进行求导运算的多项式："<< endl;
                cout << "这是目前计算器内的所有多项式："<< endl;
                for(int i = 0;i < n;i++){
                    cout << i+1 << ". ";
                    Print(Polynomial[i]);
                }
                int m;
                cin >> m;
                Node* head = Polynomial[m-1];
                cout << "这是求导运算后的结果： "<<endl;
                Print(Deriv(head));
                cin.ignore(numeric_limits<streamsize>::max(),'\n'); 
                break;
            }
            case 7:{
                cout << "程序已退出，感谢您的使用" << endl;
                for(int i = 0;i < n;i++){
                    Node* temp = Polynomial[i];
                    while(temp -> next){
                        Node * cur = temp;
                        temp = temp -> next;
                        delete cur;
                    }

                }
                _CrtDumpMemoryLeaks();
                return 0;
            }
            default :{
                cout << "这是无效的指令，请重新输入" <<endl;
                cin.ignore(numeric_limits<streamsize>::max(),'\n'); 
                break;
            }
        }
        }
}
    
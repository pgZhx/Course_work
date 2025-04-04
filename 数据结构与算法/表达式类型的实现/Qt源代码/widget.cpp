#include "widget.h"
#include "./ui_widget.h"
#include <iostream>
#include <cmath>
#include <map>
#include <stack>
#include<string>
using namespace std;
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
     this -> setWindowTitle("表达式类型的实现");
    ui -> plus ->setStyleSheet("background:pink");
    ui -> sub ->setStyleSheet("background:pink");
    ui -> mult ->setStyleSheet("background:pink");
    ui -> div ->setStyleSheet("background:pink");
    ui -> miyunsuan ->setStyleSheet("background:pink");
    ui -> clear ->setStyleSheet("background:red");
   //ui -> eq ->setStyleSheet("background:green");
    ui -> W1 ->setStyleSheet("background:lightblue");
    ui -> A1_2 ->setStyleSheet("background:lightblue");
    ui -> S1 ->setStyleSheet("background:lightblue");
    ui -> D1 ->setStyleSheet("background:lightblue");
    ui -> fun1 ->setStyleSheet("background:grey");
    ui -> fun2 ->setStyleSheet("background:grey");
    ui -> fun3 ->setStyleSheet("background:grey");
    ui -> fun4 ->setStyleSheet("background:grey");
    QColor lightpink(216,191,216); // 定义浅紫色
    QPalette palette = ui -> Line ->palette();
    palette.setColor(QPalette::Base, lightpink); // 设置背景色为浅紫色
    ui -> Line ->setPalette(palette);
    ui -> fuzhi -> setPalette(palette);


}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_one_clicked()
{
    expression += '1';
    ui -> Line -> setText(expression);
}


void Widget::on_two_clicked()
{
    expression += '2';
    ui -> Line -> setText(expression);
}


void Widget::on_three_clicked()
{
    expression += '3';
    ui -> Line -> setText(expression);
}


void Widget::on_four_clicked()
{
    expression += '4';
    ui -> Line -> setText(expression);
}


void Widget::on_five_clicked()
{
    expression += '5';
    ui -> Line -> setText(expression);
}


void Widget::on_six_clicked()
{
    expression += '6';
    ui -> Line -> setText(expression);
}


void Widget::on_seven_clicked()
{
    expression += '7';
    ui -> Line -> setText(expression);
}

void Widget::on_eight_clicked()
{
    expression += '8';
    ui -> Line -> setText(expression);
}


void Widget::on_nine_clicked()
{
    expression += '9';
    ui -> Line -> setText(expression);
}


void Widget::on_plus_clicked()
{
    expression += '+';
    ui -> Line -> setText(expression);
}


void Widget::on_sub_clicked()
{
    expression += '-';
    ui -> Line -> setText(expression);
}


void Widget::on_mult_clicked()
{
    expression += '*';
    ui -> Line -> setText(expression);
}


void Widget::on_div_clicked()
{
    expression += '/';
    ui -> Line -> setText(expression);
}


void Widget::on_zero_clicked()
{
    expression += '0';
    ui -> Line -> setText(expression);
}
void Widget::on_del_clicked()
{
    expression .chop(1);
    ui -> Line -> setText(expression);
}


void Widget::on_clear_clicked()
{
    expression.clear();
    ui -> Line -> clear();
    ui -> fuzhi -> clear();
}

void Widget::on_eq_clicked()
{
    //    expression += '=';
    //    ui -> Line -> setText(expression);
}

void Widget::on_miyunsuan_clicked()
{
    expression += '^';
    ui -> Line -> setText(expression);
}


void Widget::on_Q1_clicked()
{
    expression += 'q';
    ui -> Line -> setText(expression);
}


void Widget::on_W1_clicked()
{
    expression += 'w';
    ui -> Line -> setText(expression);
}


void Widget::on_E1_clicked()
{
    expression += 'e';
    ui -> Line -> setText(expression);
}


void Widget::on_R1_clicked()
{
    expression += 'r';
    ui -> Line -> setText(expression);
}


void Widget::on_T1_clicked()
{
    expression += 't';
    ui -> Line -> setText(expression);
}


void Widget::on_Y1_clicked()
{
    expression += 'y';
    ui -> Line -> setText(expression);
}


void Widget::on_U1_clicked()
{
    expression += 'u';
    ui -> Line -> setText(expression);
}


void Widget::on_zero_9_clicked()
{
    expression += 'i';
    ui -> Line -> setText(expression);
}


void Widget::on_O1_clicked()
{
    expression += 'o';
    ui -> Line -> setText(expression);
}


void Widget::on_P1_clicked()
{
    expression += 'p';
    ui -> Line -> setText(expression);
}


void Widget::on_A1_2_clicked()
{
    expression += 'a';
    ui -> Line -> setText(expression);
}


void Widget::on_S1_clicked()
{
    expression += 's';
    ui -> Line -> setText(expression);
}


void Widget::on_D1_clicked()
{
    expression += 'd';
    ui -> Line -> setText(expression);
}


void Widget::on_F1_clicked()
{
    expression += 'f';
    ui -> Line -> setText(expression);
}


void Widget::on_G1_clicked()
{
    expression += 'g';
    ui -> Line -> setText(expression);
}


void Widget::on_H1_clicked()
{
    expression += 'h';
    ui -> Line -> setText(expression);
}


void Widget::on_J1_clicked()
{
    expression += 'j';
    ui -> Line -> setText(expression);
}


void Widget::on_K1_clicked()
{
    expression += 'k';
    ui -> Line -> setText(expression);
}


void Widget::on_L1_clicked()
{
    expression += 'l';
    ui -> Line -> setText(expression);
}


void Widget::on_Z1_clicked()
{
    expression += 'z';
    ui -> Line -> setText(expression);
}


void Widget::on_X1_clicked()
{
    expression += 'x';
    ui -> Line -> setText(expression);
}


void Widget::on_C1_clicked()
{
    expression += 'c';
    ui -> Line -> setText(expression);
}


void Widget::on_V1_clicked()
{
    expression += 'v';
    ui -> Line -> setText(expression);
}


void Widget::on_B1_clicked()
{
    expression += 'b';
    ui -> Line -> setText(expression);
}


void Widget::on_N1_clicked()
{
    expression += 'n';
    ui -> Line -> setText(expression);
}


void Widget::on_M1_clicked()
{
    expression += 'm';
    ui -> Line -> setText(expression);
}
Bitree* Widget::build(string& expr,int&index){
    if(index >= expr.length()) return nullptr;
    Bitree* newnode = new Bitree(expr[index]);
    index++;
    if(!isdigit(newnode->data)){return newnode;}//如果不是操作符则为操作数节点
    newnode -> left = build(expr,index);
    newnode -> right = build(expr,index);//如果是操作数，则递归建立左右子树
    return newnode;

}
int Widget:: getOperPri(char& op){
    switch(op){
    case '^':
        return 3;break;
    case '+':
    case '-':
        return 2; break;
    case '*':
    case '/':
        return 1; break;
    default:
        return 0;
    }
}
void Widget::printorder(Bitree* &root){
    bool judgel = false;//用于判断左子树是否需要加括号
    bool judger = false;//判断右子树是否需要加括号
    //两个判据初始化为false，即默认不用加括号
    if(!root) return;
    else if(root -> left == nullptr && root -> right == nullptr){expression += root -> data;}
    else{
        if(getOperPri(root -> data) < getOperPri(root -> left -> data)) judgel = true;
        if(getOperPri(root -> data) <= getOperPri(root -> right -> data)) judger = true;
        //getoperpri函数中比较了优先级
        if(judgel){
            expression += '(';
            printorder(root -> left);
            expression += ')';
        }
        else printorder(root -> left);
        expression += root -> data;
        if(judger){
            expression += '(';
            printorder(root -> right);
            expression += ')';
        }
        else printorder(root -> right);
    }
}
bool Widget::isdigit(char& a){
    if(a == '+'||a == '-'||a == '*'||a == '/'||a == '^') return true;
    else return false;
}
bool Widget::hasDegreeOneNode(Bitree* root) {
    if (root == nullptr) {
            return false;
    }
    if ((root->left == nullptr && root->right != nullptr) ||
        (root->left != nullptr && root->right == nullptr)) {
            return true;
    }

    return hasDegreeOneNode(root->left) || hasDegreeOneNode(root->right);
}
void Widget::on_fun1_clicked()
{
    string str = expression.toStdString();
    expression.clear();
    int index = 0;
    Bitree*root = build(str,index);
    if(hasDegreeOneNode(root)) ui -> Line -> setText("转换失败，请输入有效的前缀表达式");
    else{
        printorder(root);
        ui -> Line -> setText(expression);
    }
}
void Widget::on_fun3_clicked()
{
    string str = expression.toStdString();
    expression.clear();
    int index = 0;
    Bitree*root = build(str,index);
    if(hasDegreeOneNode(root)) {ui -> Line -> setText("转换失败，请输入有效的前缀表达式");return;}
    int len = str.length()-1;
    stack<double> s;
    for(int i = len;i >= 0; i--){
        if(str[i] >= '0'&& str[i] <= '9') s.push((double)str[i] - '0');
        else if(!isdigit(str[i]))s.push(0);
        else{
                double num1 = s.top();
                s.pop();
                double num2 = s.top();
                s.pop();
                double val;
                switch(str[i]){
                case '+':{val = num1 + num2;break;}
                case '-':{val = num1 - num2;break;}
                case '*':{val = num1 * num2;break;}
                case '/':{
                    if(num2 == 0) {ui -> Line -> setText("分母不能为0，请输入正确的前缀表达式");return;}
                    val = num1 / num2;break;
                }
                case '^':{
                    if(num1 == 0 && num2 <= 0) {ui -> Line -> setText("0的非正数次方无意义，请输入正确的前缀表达式");return;}
                    val = pow(num1,num2);break;
                }
            }
            s.push(val);
        }
    }
    expression =  QString("%1").arg(s.top());
    ui -> Line -> setText(expression);
}


void Widget::on_fun2_clicked()
{
    QString a = ui -> fuzhi -> text();
    string s1 = a.toStdString();
    map<char,double> m;
    int l = s1.length();
    char x;int i = 0;int j,k;
    while(l > i){
        if(s1[i] >= 'a' && s1[i] <= 'z')  x = s1[i];
        if((s1[i] >= '0'&& s1[i] <= '9')||s1[i] == '.'||s1[i] == '-'){
            j = i;
            while((s1[i] >= '0'&& s1[i] <= '9')||s1[i] == '.'||s1[i] == '-') i++;
            k = i;
            int len = k-j+1;
            double d = a.mid(j,len).toDouble();
            m[x] = d;
        }
        i++;
    }
    string str = expression.toStdString();
    expression.clear();
    int index = 0;
    Bitree*root = build(str,index);
    if(hasDegreeOneNode(root)) {ui -> Line -> setText("转换失败，请输入有效的前缀表达式");return;}
    int len = str.length()-1;
    stack<double> s;
    for(int i = len;i >= 0; i--){
        if(str[i] >= '0'&& str[i] <= '9') s.push((double)str[i] - '0');
        else if(!isdigit(str[i]))s.push(m[str[i]]);
        else{
            double num1 = s.top();
            s.pop();
            double num2 = s.top();
            s.pop();
            double val;
            switch(str[i]){
                case '+':{val = num1 + num2;break;}
                case '-':{val = num1 - num2;break;}
                case '*':{val = num1 * num2;break;}
                case '/':{
                    if(num2 == 0) {ui -> Line -> setText("分母不能为0，请输入正确的前缀表达式");return;}
                    val = num1 / num2;break;
                }
                case '^':{
                   if(num1 == 0 && num2 <= 0) {ui -> Line -> setText("0的非正数次方无意义，请输入正确的前缀表达式");return;}
                    val = pow(num1,num2);break;
                }
            }
            s.push(val);
        }
    }
    expression =  QString("%1").arg(s.top());
    ui -> Line -> setText(expression);
}


void Widget::on_fun4_clicked()
{
    string str = expression.toStdString();
    int len = str.length();
    int k = 0;
    QString s1,s2;
    char opr = '+';
    for(int i = 0;i < len;i++){
        if(str[i] == '_'){
            s1 = expression.mid(0,k);
            opr = str[i+1];
            s2 = expression.mid(i+3);
            break;
        }
        else k++;
    }
    if(!isdigit(opr)){ui -> Line -> setText("无效的输入，无法进行合并，请按正确的语法输入");return;}
    string s11 = s1.toStdString();
    string s22 = s2.toStdString();
    expression.clear();
    int index1 = 0;
    int index2 = 0;
    Bitree* root1 = build(s11,index1);
    if(hasDegreeOneNode(root1)) {ui -> Line -> setText("转换失败，请输入有效的前缀表达式");return;}
    Bitree* root2 = build(s22,index2);
    if(hasDegreeOneNode(root2)) {ui -> Line -> setText("转换失败，请输入有效的前缀表达式");return;}
    Bitree* root = new Bitree(opr);
    root -> left = root1;
    root -> right = root2;
    printorder(root);
    ui -> Line -> setText(expression);
}


void Widget::on_SPace_clicked()
{
    expression += '_';
    ui -> Line -> setText(expression);
}


#include<iostream>
#include<stack>
#include<conio.h>
#include<limits>
#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif
using namespace std;
int judge(char c1,char c2){
    switch(c1){
        case '+':{
            switch(c2){
                case '+':return 1;
                case '-':return 1;
                case '*':return -1;
                case '/':return -1; 
                case '(':return -1;
                case ')':return 1;
                case '#':return 1; 
                default: return 100;
            }
        }
        case '-':{
            switch(c2){
                case '+':return 1;
                case '-':return 1;
                case '*':return -1;
                case '/':return -1; 
                case '(':return -1;
                case ')':return 1;
                case '#':return 1; 
                default: return 100;
            }
        }
        case '*':{
            switch(c2){
                case '+':return 1;
                case '-':return 1;
                case '*':return 1;
                case '/':return 1; 
                case '(':return -1;
                case ')':return 1;
                case '#':return 1; 
                default: return 100;
            }
        }
        case '/':{
            switch(c2){
                case '+':return 1;
                case '-':return 1;
                case '*':return 1;
                case '/':return 1; 
                case '(':return -1;
                case ')':return 1;
                case '#':return 1; 
                default: return 100;
            }
        }
        case '(':{
            switch(c2){
                case '+':return -1;
                case '-':return -1;
                case '*':return -1;
                case '/':return -1; 
                case '(':return -1;
                case ')':return 0;
                case '#':return 100; 
                default: return 100;
            }
        }
        case ')':{
            switch(c2){
                case '+':return 1;
                case '-':return 1;
                case '*':return 1;
                case '/':return 1; 
                case '(':return 100;
                case ')':return 1;
                case '#':return 1; 
                default: return 100;
            }
        }
        case '#':{
            switch(c2){
                case '+':return -1;
                case '-':return -1;
                case '*':return -1;
                case '/':return -1; 
                case '(':return -1;
                case ')':return 100;
                case '#':return 0; 
                default: return 100;
            }
        }
        default: return 100;
    }
}//用于判断优先级高低(用1代表大于，0代表等于，-1代表小于,100代表非法输入)
bool isoptr(char c){
    if(c >= '0' && c <= '9') return false;
    else return true;
}
int test(){
    string choice;
    bool error = false;
    do{
        system(CLEAR_SCREEN); // 清屏
        cout << "Enter an expression: " << endl;
        stack<char> OPTR;
        stack<double> OPND;
        OPTR.push('#');
        char c = getchar();
        while(c != '#' || OPTR.top() != '#'){
            if(!isoptr(c)){
                // 数字直接入栈
                double opnum = 0;
                while(!isoptr(c)){
                    opnum = 10 * opnum + (c - '0');
                    c = getchar();
                }
                OPND.push(opnum);
            } else {
                switch(judge(OPTR.top(),c)){
                    case -1: {
                        OPTR.push(c);
                        c = getchar();
                        break;
                    }
                    case 0:{
                        OPTR.pop();
                        c = getchar();
                        break;
                    }
                    case 1:{
                        double a = OPND.top();
                        OPND.pop();
                        double b = OPND.top();
                        OPND.pop();
                        char op = OPTR.top();
                        OPTR.pop();
                        switch(op){
                            case '+':{
                                OPND.push(b+a);
                                break;
                            }
                            case '-':{
                                OPND.push(b-a);
                                break;
                            }
                            case '*':{
                                OPND.push(a*b);
                                break;
                            }
                            case '/':{
                                if(a != 0) OPND.push(b/a);
                                else {
                                    error = true;
                                    cout << "Error: Division by zero" << endl;  // 分母为0时输出"Error"
                                    // cin.clear();
                                    // cin.sync();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // 忽略剩余的字符，直到遇到换行符
                                }
                                break;
                            }              
                            default:{ 
                                error = true;
                                // cin.clear();
                                // cin.sync();  // 清空输入缓冲区
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');  // 忽略剩余的字符，直到遇到换行符
                                break;
                                }      
                        }
                        break;
                    }
                    default : {
                        error = true;
                        cout << "Error: Invalid input" << endl;  // 非法输入时输出"Error: Invalid input"
                        // cin.clear();
                        // cin.sync();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // 忽略剩余的字符，直到遇到换行符
                        break;
                    }
                }
            }
        }
        if(!error){
            cout << "Result: " << OPND.top() << endl; // 输出最终结果
        } else {
            cout << "Calculation failed" << endl;  // 计算失败时输出"Calculation failed"
            error = false;
        }
        ;
        cout << "Do you want to continue?(Yes/No)" << endl;
        cin >> choice;
        cin.clear();
            cin.sync();
    }while(choice == "Yes");
    return 0;
}
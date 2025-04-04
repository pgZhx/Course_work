//
// Created by 26618 on 2023/12/15.
//

#ifndef PROJECT6_MYSTACK_H
#define PROJECT6_MYSTACK_H
#pragma once
class MyStack
{
public:
    int *m_pBuffer;     // 栈空间的指针
    int m_iSize;         // 栈容量
    int m_iTop;          // 栈顶
    MyStack(int size);
    ~MyStack();
    bool stackEmpty();    //判断栈是否为空
    bool stackFull();     //判断栈是否满
    bool push(int elem); // 将元素入栈
    bool pop(); // 出栈
};
MyStack::MyStack(int size)
{
    m_iSize = size;
    m_pBuffer = new int[size];
    m_iTop = 0;
}
MyStack::~MyStack()
{
    delete[] m_pBuffer;
}
bool MyStack::stackEmpty()
{
    if (m_iTop == 0)
        return true;
    else
        return false;
}
bool MyStack::stackFull()
{
    if (m_iTop >= m_iSize)
        return true;
    else
        return false;
}
bool MyStack::push(int elem)
{
    if (stackFull())
    {
        return false;
    }
    m_pBuffer[m_iTop] = elem;
    m_iTop++;
    return true;
}
bool MyStack::pop()
{
    if (stackEmpty())
        return false;
    m_iTop--;
    return true;
}
#endif //PROJECT6_MYSTACK_H

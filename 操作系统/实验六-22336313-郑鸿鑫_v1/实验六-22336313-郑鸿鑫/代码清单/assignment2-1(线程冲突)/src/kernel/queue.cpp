#include "queue.h"
#include "asm_utils.h"
#include "stdio.h"
#include "os_modules.h"
#include "program.h"
bool QUEUE::empty(){
    return tail == front;
}
bool QUEUE:: full(){
    return (tail+1)%max_size == front;
}
bool QUEUE::push(int item){
    if ((tail+1)%max_size == front) return false;
    queue[tail]  = item;
    tail = ( tail+1)%max_size;
    return true;
}
bool QUEUE::pop(){
    if (tail == front) return false;
    front =( front+1)%max_size;
    return true;
}
void QUEUE::show(){
    int i = front;
    printf("Plate:");
    for(; i != tail;){
        printf("%d ",queue[i]);
        i = (i+1)%max_size;
    }
    printf("matcha:1,mango:2\n");
}
QUEUE::QUEUE(){
    init();
}
void QUEUE::init(){
    max_size = 5;
    front = 0;
    tail = 0;
}

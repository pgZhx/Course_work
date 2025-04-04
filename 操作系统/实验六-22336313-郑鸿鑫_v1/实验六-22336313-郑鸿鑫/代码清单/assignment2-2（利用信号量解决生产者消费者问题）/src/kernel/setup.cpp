#include "asm_utils.h"
#include "interrupt.h"
#include "stdio.h"
#include "program.h"
#include "thread.h"
#include "sync.h"
#include "queue.h"
// 屏幕IO处理器
STDIO stdio;
// 中断管理器
InterruptManager interruptManager;
// 程序管理器
ProgramManager programManager;

Semaphore semaphore;
int man ;
int woman ;
Semaphore  matcha;
Semaphore mango;
QUEUE q_cake;
void waiterA(void *arg)
{   
    while(true){
        if (man >= 6) break;
       while(q_cake.full()){}
        q_cake.push(1);
        printf("WaiterA_6313 put a piece of matcha cake on the plate.\n");
        q_cake.show();
        matcha.V();
    }
    
}
void waiterB(void *arg)
{
    while(true){
        if (woman >=4)break; 
        while(q_cake.full()){}
        q_cake.push(2);
        printf("WaiterB_6313 put a piece of mango cake on the plate.\n");
       q_cake.show();
       mango.V();
    }

}
void man_consumer(void *arg)
{
   while(true){
        if(man > 6)break;
        while(q_cake.empty()){}
        matcha.P();
        q_cake.pop();
        printf("man%d consumed\n",man);
        q_cake.show();
        man++;
    }
}
void woman_consumer(void *arg)
{
    while(true){
        if(woman > 4)break;
        while(q_cake.empty()){}
        mango.P();
        q_cake.pop();
        printf("woman%d consumed\n",woman);
        q_cake.show();
        woman++;
    }
}

void first_thread(void *arg)
{
    // 第1个线程不可以返回
    stdio.moveCursor(0);
    for (int i = 0; i < 25 * 80; ++i)
    {
        stdio.print(' ');
    }
    stdio.moveCursor(0);
    semaphore.initialize(1);
   man = 1;
    woman = 1;
    // my_mutex.initalize(1);
    matcha.initialize(0);
    mango.initialize(0);
    q_cake.init();
    programManager.executeThread(waiterA, nullptr, "second thread", 1);
    programManager.executeThread(man_consumer, nullptr, "third thread", 1);
    programManager.executeThread(waiterB, nullptr, "fourth thread", 1);
    programManager.executeThread(woman_consumer, nullptr, "fifth thread",1);

;

    asm_halt();
}

extern "C" void setup_kernel()
{

    // 中断管理器
    interruptManager.initialize();
    interruptManager.enableTimeInterrupt();
    interruptManager.setTimeInterrupt((void *)asm_time_interrupt_handler);

    // 输出管理器
    stdio.initialize();

    // 进程/线程管理器
    programManager.initialize();

    // 创建第一个线程
    int pid = programManager.executeThread(first_thread, nullptr, "first thread", 1);
    if (pid == -1)
    {
        printf("can not execute thread\n");
        asm_halt();
    }

    ListItem *item = programManager.readyPrograms.front();
    PCB *firstThread = ListItem2PCB(item, tagInGeneralList);
    firstThread->status = RUNNING;
    programManager.readyPrograms.pop_front();
    programManager.running = firstThread;
    asm_switch_thread(0, firstThread);

    asm_halt();
}
